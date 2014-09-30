class Screen {
  Box box;
  JSONObject json;
  
  public Screen(PApplet parent) {
    println("Loading json");
    json = loadJSONObject("http://uifaces.com/api/v1/random");
    
    println(json);
    box = new Box(parent);
    box.setTexture(json.getJSONObject("image_urls").getString("epic"), Box.FRONT);
    box.drawMode(S3D.TEXTURE | S3D.WIRE);
    box.setSize(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_DEPTH);
    delay(100);
  }
  
  void draw() {
    //fill(100,255,100);
    //box(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_DEPTH);
    box.draw();
  }
}
