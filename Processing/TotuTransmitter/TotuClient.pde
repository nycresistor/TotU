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
import processing.net.*;

public class TotuClient {
  PApplet parent;
  Client client;
  boolean usePresliced;
  byte buffer[];

  public TotuClient(PApplet parent, String host, int port, boolean usePresliced) {
    this.parent = parent;
    this.client = new Client(parent, host, port);
    this.usePresliced = usePresliced;
    
    int bufsize = usePresliced ? 320*240*16*2+2 : 320*240*2+2;
    this.buffer = new byte[bufsize];
    for (int i=0; i<buffer.length; i++) buffer[i] = 0;
  }
  
  public void sendData(int screens) {
    if (usePresliced) {
      sendDataPresliced(screens);
    }
    else {
      sendDataNormal(screens);
    }
  }
  
  /** 
   * Sends data in normal pixel mode.
   *
   * Each frame is two bytes specifying a bitmask of
   * screens to write to, followed by 320x240x2 bytes
   * of color data.  Colors are 16-bit RRRRRGGGGGGBBBBB.
   **/
  void sendDataNormal(int screens) {
    parent.loadPixels();
    
    buffer[0] = (byte) (screens & 0xFF00 >> 8);
    buffer[1] = (byte) (screens & 0xFF);
    
    int idx = 2;
    int r, g, b;
    for (int i=0; i<parent.pixels.length; i++) {
      
      r = int(red(parent.pixels[i]));
      g = int(green(parent.pixels[i]));
      b = int(blue(parent.pixels[i]));
      
      buffer[idx] = (byte) (((r >> 3) << 3) + (g >> 5));
      buffer[idx+1] = (byte) (((g >> 5) << 5) + (b >> 3));
      
      idx+=2;
    }
    
    client.write(buffer);
  }
  
  /**
   * Sends data in presliced mode.
   *
   * Each frame is 0x0000 followed by 320*240*16*2 bytes
   * of presliced color data.  Colors are 16-bit,
   * RRRRRGGGGGGBBBBB.  Each two bytes represents 1 bit
   * of color data for 16 screens.
   *
   * The code is a bit messy because the Processing 
   * client requires an array of signed bytes[] 
   * rather than 16-bit uints.
   **/ 
  void sendDataPresliced(int screens) {
    int idx = 2;
    int r, g, b;
    int hi, lo;
    byte mask;
    byte shi, slo;
    
    parent.loadPixels();
    
    // First two bytes are always zero, signals
    // presliced mode.
    buffer[0] = 0;
    buffer[1] = 0;
    
    // Calculate the high and low screen masks
    shi = (byte) ((screens & 0xFF00) >> 8);
    slo = (byte) (screens & 0xFF); 
      
    for (int i=0; i<parent.pixels.length; i++) {
      r = int(red(parent.pixels[i]));
      g = int(green(parent.pixels[i]));
      b = int(blue(parent.pixels[i]));
      
      // High and low bytes of the 16-bit color  
      hi = ((r >> 3) << 3) + (g >> 5);
      lo = ((g >> 5) << 5) + (b >> 3);
  
      // First 8 bytes are the high color
      for (byte j=0; j<8; j++) {
        // First byte are the high screens
        // If the bit is set we bitwise or
        // the screen bits, otherwise we 
        // bitwise and the negative screen
        // bits.
        buffer[idx] = (hi & 1<<7-j) > 0 ?
          byte(buffer[idx] | shi) :
          byte(buffer[idx] & ~shi);        
        idx++;
        
        // Second byte are the low screens
        buffer[idx] = (hi & 1<<7-j) > 0 ?
          byte(buffer[idx] | slo) :
          byte(buffer[idx] & ~slo);       
        idx++;
      }
      
      // Second 8 bytes are the low color
      for (int j=0; j<8; j++) {
        // High screens
        buffer[idx] = (lo & 1<<7-j) > 0 ?
          byte(buffer[idx] | shi) :
          byte(buffer[idx] & ~shi);        
        idx++;
  
        // Low screens
        buffer[idx] = (lo & 1<<7-j) > 0 ?
          byte(buffer[idx] | slo) :
          byte(buffer[idx] & ~slo);
        idx++;
      }
    }
    
    // Send the data.
    client.write(buffer);
  }
}

