/**
 * Simple TCP transmitter for ToTU.  It has two modes, normal
 * pixel mode and presliced mode. 
 *
 * Normal Mode
 *
 * Each frame starts with a 16-bit mask of the screens to
 * write to.  Screens whose bits are unset will stay the
 * same as they are.  This is followed by 320*240 16-bit
 * color values (RRRRRGGGGGGBBBBB). 
 *
 * Presliced Mode
 *
 * In presliced mode the sketch is keeping all the state
 * for the screens and the server is just blindly dumping
 * the data out to the screens.  When using presliced mode
 * only one client can be connected to a given server.
 *
 * Frames start with 0x0000 (zero screen mask activates the
 * mode) followed by 320*240*16*2 bytes.  Each two bytes
 * represent 1 bit of color for 16 screens.  Colors are
 * 16-bit as in Normal mode.
 **/

// Enables presliced mode
final boolean USE_PRESLICE = false;

TotuClient client;
byte buffer[];
PFont font;

void setup() {
  size(320,240);
  client = new TotuClient(this, "192.168.7.2", 9000, USE_PRESLICE);
  
  font = loadFont("Disorient-16.vlw");
  textFont(font,16);
}

void draw() {
  int c = int(millis() / 1000) % 3;
  
  background(
    c==0 ? color(255,0,0) : 
    c==1 ? color(0,255,0) : 
    color(0,0,255)
  );
  
  fill(0);
  textAlign(CENTER,CENTER);
  text(frameCount % 10, width/2, height/2);
 
  client.sendData(frameCount % 2 == 0 ? 0x1FFFF : 0xFFFF);
  
  if (frameCount % 10 == 0) {
    println("Frame rate = " + frameRate);
  }
}

