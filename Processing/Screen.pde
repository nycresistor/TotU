class Screen {
  Box box;
  
  public Screen(PApplet parent) {
    box = new Box(parent);
    box.setTexture("0-PhotoBoothPhotoSmall.jpg", Box.FRONT);
    box.drawMode(S3D.TEXTURE | S3D.WIRE);
    box.setSize(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_DEPTH);

  }
  
  void draw() {
    //fill(100,255,100);
    //box(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_DEPTH);
    box.draw();
  }
}
