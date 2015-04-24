#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include "tft.h"

#define MAXEVENTS 64
#define WIDTH 320
#define HEIGHT 240
#define SCREENS 16
#define MODE_PIXEL 1
#define MODE_PRESLICE 2

struct con_state {
	int mode;
	int offset;
	uint16_t mask;
};

uint16_t display_buffer[WIDTH*HEIGHT*SCREENS];
//char xfer_buffer[38400];
char xfer_buffer[2457602];

static int make_socket_non_blocking(int sfd)
{
    int flags, s;
    flags = fcntl(sfd, F_GETFL, 0);
    if (flags == -1)
    {
        perror("fcntl");
        return -1;
    }
    flags |= O_NONBLOCK;
    s = fcntl(sfd, F_SETFL, flags);
    if (s == -1)
    {
        perror("fcntl");
        return -1;
    }
    return 0;
}

static int create_and_bind(char *port)
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int s, sfd;

    memset(&hints, 0, sizeof (struct addrinfo));
    hints.ai_family = AF_UNSPEC; /* Return IPv4 and IPv6 choices */
    hints.ai_socktype = SOCK_STREAM; /* We want a TCP socket */
    hints.ai_flags = AI_PASSIVE; /* All interfaces */

    s = getaddrinfo(NULL, port, &hints, &result);
    if (s != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        return -1;
    }

    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1)
        continue;
        s = bind(sfd, rp->ai_addr, rp->ai_addrlen);
        if (s == 0)
        {
            /* We managed to bind successfully! */
            break;
        }
        close(sfd);
    }

    if (rp == NULL)
    {
        fprintf(stderr, "Could not bind\n");
        return -1;
    }

    freeaddrinfo(result);
    return sfd;
}

void write_pixel(uint16_t screens, int offset, uint16_t color)
{
	int i;
	uint16_t *db = display_buffer + offset * 16;
	uint16_t mask;

	for (i=0; i<16; i++) {
		if (color & (1<<(15-i))) {
			*db |= screens;
		}
		else {
			*db &= ~screens;
		}
		db++;
	}
}

void process_presliced(int count, int fd, struct con_state *states)
{
	int bufsize = WIDTH*HEIGHT*SCREENS*2;
	int copy_count = count;
	int newofs = states[fd].offset + copy_count;
	char *xb = xfer_buffer;
	char *db = (char *)display_buffer + states[fd].offset;
	
	// Skip screens mask
	if (states[fd].offset == 0) {
		count -= 2;
		copy_count -= 2;
		xb += 2;
		newofs -= 2;
	}

	// Break at end of buffer
	if (newofs >= bufsize) {
		copy_count = bufsize - states[fd].offset;
		newofs = 0;
	}

	//printf("Copying from %d to %d count=%d ofs=%d newofs=%d\n",(int)xb,(int)db,copy_count,states[fd].offset,newofs);
	memcpy(db, xb, copy_count);
	states[fd].offset = newofs;

	if (newofs == 0)
	{
		writeFramePregenerated(display_buffer, WIDTH * HEIGHT * 16);
	}
	
	// If the frame ended in the middle of this xfer_buffer we'll have
	// leftover, just copy that to the begining and overwite offset
	// since it definitely won't happen more than once.
	if (count > copy_count) {
		db = (char *)display_buffer;
		xb = xfer_buffer + copy_count + 2;
		memcpy(db, xb, count-copy_count-2);
		states[fd].offset = count-copy_count-2;
	}
}

void process_bytes(int count, int fd, struct con_state *states)
{
	int i;
	for (i=0; i<count; i+=2) 
	{
		uint16_t val = (uint16_t) xfer_buffer[i]<<8 | xfer_buffer[i+1];

		if (states[fd].offset == 0) 
		{
			states[fd].mask = val;
		}
		else 
		{
			write_pixel(states[fd].mask, (states[fd].offset-2)/2, val);
		}

		states[fd].offset += 2;

		if (states[fd].offset >= (WIDTH * HEIGHT * 2 + 2)) 
		{
			// Frame complete
			states[fd].offset = 0;
			writeFramePregenerated(display_buffer, WIDTH * HEIGHT * 16);
		}
	}
}

void read_data(struct epoll_event *events, int i, struct con_state *states)
{
	/* We have data on the fd waiting to be read. Read and
	   display it. We must read whatever data is available
	   completely, as we are running in edge-triggered mode
	   and won't get a notification again for the same
	   data. */
	int done = 0;
	ssize_t count;

	while (1)
	{
		count = read(events[i].data.fd, xfer_buffer, sizeof xfer_buffer);
		if (count == -1)
		{
			/* If errno == EAGAIN, that means we have read all
			data. So go back to the main loop. */
			if (errno != EAGAIN)
			{
				perror("read");
				done = 1;
			}
			break;
		}
		else if (count == 0)
		{
			/* End of file. The remote has closed the
			connection. */
			done = 1;
			break;
		}

		// If we haven't selected a mode, check the first packet to see if there
		// is an screen mask.  A mask of zero indicates presliced data.
		int fd = events[i].data.fd;
		if (states[fd].mode == 0) {
			states[fd].mode = (xfer_buffer[0] || xfer_buffer[1]) ?
				MODE_PIXEL :
				MODE_PRESLICE;

			printf("Connection %d mode set to %s\n", fd, states[fd].mode == MODE_PIXEL ? "pixel" : "preslice");
		}

		// Send the data
		if (states[fd].mode == MODE_PRESLICE)
			process_presliced(count, fd, states);
		else
			process_bytes(count, fd, states);
	}

	if (done)
	{
		printf("Closed connection on descriptor %d\n",
		events[i].data.fd);
		
		/* Closing the descriptor will make epoll remove it
	       from the set of descriptors which are monitored. */
		close(events[i].data.fd);
	}
}

// TODO FIXME I shouldn't need to pass all this in, needs refactor.
void accept_connection(struct epoll_event *events, int i, int efd, struct epoll_event *event, struct con_state *states)
{
	int s;
	struct sockaddr in_addr;
	socklen_t in_len;
	int infd;
	char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];

	while (1)
	{
		in_len = sizeof in_addr;
		infd = accept(events[i].data.fd, &in_addr, &in_len);
                    
		if (infd == -1)
		{
			if ((errno == EAGAIN) ||
				(errno == EWOULDBLOCK))
			{
				/* We have processed all incoming
				connections. */
				break;
			}
			else
			{
				perror("accept");
				break;
			}
		}
		
		s = getnameinfo(&in_addr, in_len,
			hbuf, sizeof hbuf,
			sbuf, sizeof sbuf,
			NI_NUMERICHOST | NI_NUMERICSERV);
			
		if (s == 0)
		{
			printf("Accepted connection on descriptor %d "
			"(host=%s, port=%s)\n", infd, hbuf, sbuf);
		}
		
		/* Make the incoming socket non-blocking and add it to the
		list of fds to monitor. */
		s = make_socket_non_blocking(infd);
		if (s == -1)
			abort();
			
		event->data.fd = infd;
		event->events = EPOLLIN | EPOLLET;
		s = epoll_ctl(efd, EPOLL_CTL_ADD, infd, event);
		
		if (s == -1)
		{
			perror("epoll_ctl");
			abort();
		}
	
		states[infd].offset = 0;
		states[infd].mask = 0;
		states[infd].mode = 0;
	}
}

void setup_display()
{
	printf("Setup gpmc\n");
	setup_gpmc();

	printf("Setup tft\n");
	setup_tft();

	setRotation(3);
	setAddrWindow(0,0,319,239);
	activateBank(0);
	
}

int main (int argc, char *argv[])
{
    int sfd, s;
    int efd;
    struct epoll_event event;
    struct epoll_event *events;
	struct con_state state;
	struct con_state *states;

    if(argc != 2)
    {
        fprintf(stderr, "Usage: %s [port]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

	setup_display();

    sfd = create_and_bind(argv[1]);
    if (sfd == -1)
    	abort();

    s = make_socket_non_blocking(sfd);
    if (s == -1)
    	abort();

    s = listen(sfd, SOMAXCONN);
    if (s == -1)
    {
        perror("listen");
        abort();
    }

    efd = epoll_create1(0);
    if (efd == -1)
    {
        perror("epoll_create");
        abort();
    }
    event.data.fd = sfd;
    event.events = EPOLLIN | EPOLLET;
    s = epoll_ctl(efd, EPOLL_CTL_ADD, sfd, &event);
    if (s == -1)
    {
        perror("epoll_ctl");
        abort();
    }

    /* Buffer where events are returned */
    events = (struct epoll_event *) calloc(MAXEVENTS, sizeof(event));
	states = (struct con_state *) calloc(MAXEVENTS, sizeof(state));

    /* The event loop */
    while(1)
    {
        int n, i;
        n = epoll_wait(efd, events, MAXEVENTS, -1);
        for (i = 0; i < n; i++)
        {
            if ((events[i].events & EPOLLERR) ||
            (events[i].events & EPOLLHUP) ||
            (!(events[i].events & EPOLLIN)))
            {
                /* An error has occured on this fd, or the socket is not
                ready for reading (why were we notified then?) */
                fprintf(stderr, "epoll error\n");
                close(events[i].data.fd);
                continue;
            }
            /* We have a notification on the listening socket, which
               means one or more incoming connections. */
            else if (sfd == events[i].data.fd)
            {
				accept_connection(events, i, efd, &event, states);
            }
            else
            {
				read_data(events, i, states);
            }
        }
    }
    free(events);
    close(sfd);
    return EXIT_SUCCESS;
}
