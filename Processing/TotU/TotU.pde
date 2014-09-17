import shapes3d.utils.*;
import shapes3d.animation.*;
import shapes3d.*;

import peasy.test.*;
import peasy.org.apache.commons.math.*;
import peasy.*;
import peasy.org.apache.commons.math.geometry.*;

float SCREEN_WIDTH = 7.7;
float SCREEN_HEIGHT = 5.5;
float SCREEN_DEPTH = .2;
float PANEL_DEPTH = 5;
float PANEL_WIDTH = 60;
float PANEL_HEIGHT = 120;
int PANEL_SCREENS_X = 3;
int PANEL_SCREENS_Y = 8;
int PANELS = 8;

PeasyCam cam;
Panel[] panels = new Panel[PANELS];

void setup() {
  size(800, 600, P3D);
  cam = new PeasyCam(this, 100);
  cam.setMinimumDistance(200);
  cam.setMaximumDistance(500);
  
  for (int i=0; i<PANELS; i++) {
    panels[i] = new Panel(this);
  }
}

void draw() {
  background(0);
 
 
  pushMatrix();
 
  translate(0,0,PANEL_WIDTH); 
  pushMatrix();
  
  for (int i=0; i<PANELS; i++) {
    translate(PANEL_WIDTH/2,0,0);
    rotateY(TWO_PI/PANELS);   
    translate(PANEL_WIDTH/2,0,0);

    panels[i].draw();
  }
  
  popMatrix();
  popMatrix();
}
