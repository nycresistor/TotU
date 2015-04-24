import processing.net.*;

final boolean USE_PRESLICE = false;
final int BUFSIZE = USE_PRESLICE ? 320*240*16*2+2 : 320*240*2+2;

Client client;
byte buffer[];
PFont font;

void setup() {
  size(320,240);
  client = new Client(this, "192.168.7.2", 9000);
  buffer = new byte[BUFSIZE];
  for (int i=0; i<buffer.length; i++) buffer[i] = 0;
  
  font = loadFont("Disorient-16.vlw");
  textFont(font,16);
//  frameRate(1);
}

// This is messy because write needs bytes (not shorts)
void sendDataPresliced(int screens) {
  int idx = 2;
  int r, g, b;
  int hi, lo;
  byte mask;
  byte shi, slo;
  
  loadPixels();
  
  buffer[0] = 0;
  buffer[1] = 0;
  
  shi = (byte) ((screens & 0xFF00) >> 8);
  slo = (byte) (screens & 0xFF); 
    
  for (int i=0; i<pixels.length; i++) {
    r = int(red(pixels[i]));
    g = int(green(pixels[i]));
    b = int(blue(pixels[i]));
      
    hi = ((r >> 3) << 3) + (g >> 5);
    lo = ((g >> 5) << 5) + (b >> 3);

    // First 8 bytes are the high color
    for (byte j=0; j<8; j++) {
      // First byte is the high screens
      buffer[idx] = (hi & 1<<7-j) > 0 ?
        byte(buffer[idx] | shi) :
        byte(buffer[idx] & ~shi);        
      idx++;
      
      // Second byte is the low screens
      buffer[idx] = (hi & 1<<7-j) > 0 ?
        byte(buffer[idx] | slo) :
        byte(buffer[idx] & ~slo);       
      idx++;
    }
    
    // Second 8 bytes are the low color
    for (int j=0; j<8; j++) {
      buffer[idx] = (lo & 1<<7-j) > 0 ?
        byte(buffer[idx] | shi) :
        byte(buffer[idx] & ~shi);        
      idx++;

      buffer[idx] = (lo & 1<<7-j) > 0 ?
        byte(buffer[idx] | slo) :
        byte(buffer[idx] & ~slo);
      idx++;
    }
  }
  client.write(buffer);
}

void sendData(int screens) {
  loadPixels();
  
  buffer[0] = (byte) (screens & 0xFF00 >> 8);
  buffer[1] = (byte) (screens & 0xFF);
  
  int idx = 2;
  int r, g, b;
  for (int i=0; i<pixels.length; i++) {
    
    r = int(red(pixels[i]));
    g = int(green(pixels[i]));
    b = int(blue(pixels[i]));
    
    buffer[idx] = (byte) (((r >> 3) << 3) + (g >> 5));
    buffer[idx+1] = (byte) (((g >> 5) << 5) + (b >> 3));
    
    idx+=2;
  }
  
  client.write(buffer);
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
  
  if (USE_PRESLICE) {
    sendDataPresliced(frameCount % 2 == 0 ? 0xAAAA : 0x5555);
  }
  else {
    sendData(frameCount % 2 == 0 ? 0xAAAA : 0x5555);
  }
  
  if (frameCount % 10 == 0) {
    println("Frame rate = " + frameRate);
  }
 
  //if (frameCount > 1) exit();
}

