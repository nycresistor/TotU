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
PImage groundTexture;
PImage background;

void setup() {
  size(800, 600, P3D);
  cam = new PeasyCam(this, 200);
  cam.setMinimumDistance(200);
  cam.setMaximumDistance(500);
  cam.setYawRotationMode();
  
  for (int i=0; i<PANELS; i++) {
    panels[i] = new Panel(this);
  }
  
  groundTexture = loadImage("Lost Lake.jpg");
  background = loadImage("BMan11.jpg");
}

void draw() {
  background(background);
    
  //drawGround();
 
  pushMatrix();
 
  translate(0,0,PANEL_WIDTH); 
  pushMatrix();
  
  for (int i=0; i<PANELS; i++) {
    translate(PANEL_WIDTH,0,0);
    rotateY(TWO_PI/PANELS);   
    translate(PANEL_WIDTH,0,0);

    panels[i].draw();
  }
  
  popMatrix();
  popMatrix();
  
}

void drawGround() {
  stroke(92, 51);
  fill(92, 51);
  
  int tilefactor = 20;
  float bound = 1000;

  for (int x = 0; x < tilefactor; x++) {
    for (int y = 0; y < tilefactor; y++) {
      pushMatrix();
      translate(0, 0.5, 0);
      
      translate(bound/tilefactor*x-bound/2, PANEL_HEIGHT/2, bound/tilefactor*y-bound/2);
      
      beginShape();
      texture(groundTexture);
      textureMode(NORMAL);
      
      vertex(0,                .5, 0,                0, 0);
      vertex(bound/tilefactor, .5, 0,                1, 0);
      vertex(bound/tilefactor, .5, bound/tilefactor, 1, 1);
      vertex(0,                .5, bound/tilefactor, 0, 1);
      endShape();
      popMatrix();
    }
  }
}

