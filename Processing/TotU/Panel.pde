class Panel {
  Box panel;
  Screen[] screens = new Screen[PANEL_SCREENS_X*PANEL_SCREENS_Y];
  
  public Panel(PApplet parent) {
    for (int i=0; i<screens.length; i++) {
      screens[i] = new Screen(parent);
    }
    
    panel = new Box(parent);
    panel.setTexture("seamless-wood-texture-free-6.jpg", Box.ALL_SIDES);
    panel.drawMode(S3D.TEXTURE | S3D.WIRE);
    panel.setSize(PANEL_WIDTH,PANEL_HEIGHT,PANEL_DEPTH);
  }
  
  void draw() {
    fill(200,120,50);
    panel.draw();
    
    pushMatrix();
    translate(-PANEL_WIDTH/2-1,-PANEL_HEIGHT/2+7,PANEL_DEPTH/2);
    for (int y=0; y<PANEL_SCREENS_Y; y++) {

      pushMatrix();
      for (int x=0; x<PANEL_SCREENS_X; x++) {
        translate(SCREEN_WIDTH*2,0,0);
        screens[y*PANEL_SCREENS_X+x].draw();
      }
      popMatrix();

      translate(0,15,0);
    }
    popMatrix();
      
  }
}

