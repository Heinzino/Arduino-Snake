#include <C:\Users\HeinzV\Desktop\repo\GabeItches\Code\Snake\a_GLOBALS\a_GLOBALS.ino>
#include <C:\Users\HeinzV\Desktop\repo\GabeItches\Code\Snake\a_GLOBALS\b_Display.ino>
#include <C:\Users\HeinzV\Desktop\repo\GabeItches\Code\Snake\a_GLOBALS\music.ino>

void createBoard() {
  //Board of the snake game
  //The board will go here, assuming 8x8
  for (int i = 0; i < NUM_OF_TILES; i++) {
    for (int j = 0; j < NUM_OF_TILES; j++) {
      if(((i + j) % 2) == 0) { //Black Tile 9 
        tft.drawRect(TILE_SIZE*i + START_X, TILE_SIZE*j + START_Y, TILE_SIZE, TILE_SIZE, GRASS_DARK_GREEN);
        tft.fillRect(TILE_SIZE*i + START_X, TILE_SIZE*j + START_Y, TILE_SIZE, TILE_SIZE, GRASS_DARK_GREEN);
      } else { //White Tile 10
        tft.drawRect(TILE_SIZE*i + START_X, TILE_SIZE*j + START_Y, TILE_SIZE, TILE_SIZE, GRASS_LIGHT_GREEN);
        tft.fillRect(TILE_SIZE*i + START_X, TILE_SIZE*j + START_Y, TILE_SIZE, TILE_SIZE, GRASS_LIGHT_GREEN);
      }
    }
  }
}


void fill_tile(int x_tile_right, int y_tile_down, uint32_t color){
  //Spawns green rectangle on (x_tile_right + 1, y_tile_down + 1) square on board
  int x = START_X + (x_tile_right* TILE_SIZE);
  int y = START_Y + (y_tile_down * TILE_SIZE);
  
  tft.fillRect(x, y, TILE_SIZE, TILE_SIZE, color);

}

void TailDisplay(){
  int tailX = snake.getTailX();
  int tailY = snake.getTailY();
  if(((tailX + tailY) % 2) == 0){ //dark tile
    fill_tile(tailX, tailY, GRASS_DARK_GREEN);
  }
  else{
    fill_tile(tailX,tailY, GRASS_LIGHT_GREEN);
  }

}

void Display_Apple(int x_tile_right, int y_tile_down){
  
  //made it a bit smaller so it doesn't conflict with the border
  float origin_x = START_X + (x_tile_right*TILE_SIZE) + TILE_SIZE/2.0;
  float origin_y = START_Y + (y_tile_down*TILE_SIZE) + TILE_SIZE/2.0;

  float leaf_xo = origin_x;
  float leaf_yo = origin_y - apple_radius;
  float leaf_x = leaf_xo + apple_radius;
  float leaf_y = leaf_yo - 3;
  
  tft.fillCircle(origin_x, origin_y, apple_radius, RED);
  tft.drawLine(leaf_xo, leaf_yo, leaf_x, leaf_y, DARK_GREEN);
  tft.drawLine(leaf_xo, leaf_yo, leaf_x-1, leaf_y-1, DARK_GREEN);
  tft.drawLine(leaf_xo, leaf_yo, leaf_x+1, leaf_y+1, DARK_GREEN);
  tft.drawLine(leaf_xo, leaf_yo, leaf_x-2, leaf_y-2, DARK_GREEN);
 
}

void Display_Score_Screen(int score){
  Display_Apple(-2,0); //Part of Score_Screen
  String text = String(score);
  tft.setTextSize(2);

//change start position if double digit to look better
  if(score < 10){
  tft.drawString(text, 2*apple_radius, apple_radius+35);
  }
  else{
    tft.drawString(text, apple_radius, apple_radius+35);
  }
}


void Snake_Eye(){

  int snakeX_pos = snake.getHeadX();
  int snakeY_pos = snake.getHeadY();
  int eye_radius = 5;
  int pupil_radius = 3;

  //First eye
  float origin_xo = START_X + (snakeX_pos * TILE_SIZE) + TILE_SIZE*0.5;
  float origin_yo = START_Y + ((snakeY_pos+1) * TILE_SIZE) - TILE_SIZE*0.75;
  tft.fillCircle(origin_xo, origin_yo, eye_radius, WHITE);
  tft.fillCircle(origin_xo, origin_yo, pupil_radius, BLACK);

  //Second eye
  float origin_x = origin_xo;
  float origin_y = START_Y + ((snakeY_pos+1) * TILE_SIZE) - TILE_SIZE*0.25;
  tft.fillCircle(origin_x, origin_y, eye_radius, WHITE);
  tft.fillCircle(origin_x, origin_y, pupil_radius, BLACK);
  
}


void display_snake_head(){
  fill_tile(snake.getHeadX(),snake.getHeadY(), snake_colour);   
  Snake_Eye();
}


void Initialize_Screen_and_Board(){
  //Makes background black
  tft.begin();
  tft.setRotation(1); //Makes starting point at top left corner of the screen when its horizontal
  tft.fillScreen(BLACK);
  createBoard();
}


void highscore_screen(bool new_high_score){

  //Icons
    Display_Apple(2,3);
    
    //Star
    tft.drawLine(280,155,290,130,YELLOW);
    tft.drawLine(290,130,300,155,YELLOW);
    tft.drawLine(300,155, 275, 140, YELLOW);
    tft.drawLine(275,140, 305, 140, YELLOW);
    tft.drawLine(305,140,280,155,YELLOW);

    //Eye
    tft.fillCircle(155+50, 10+20, 7, WHITE);
    tft.fillCircle(155+50, 10+20, 4, BLACK);

  //Text
   if(new_high_score){
      tft.setTextColor(BLUE);
      tft.setTextSize(2);
      tft.drawString("NEW HIGH SCORE",155, 200);
    }

    if(winCondition()){
      tft.setTextSize(4);
      tft.setTextColor(RED);
      tft.drawString("Y",10,110);
      tft.setTextColor(ORANGE);
      tft.drawString("O",10,145);
      tft.setTextColor(YELLOW);
      tft.drawString("U",10,180);
      tft.setTextColor(GREEN);
      tft.drawString("W",450,110);
      tft.setTextColor(BLUE);
      tft.drawString("I",450,145);
      tft.setTextColor(PURPLE);
      tft.drawString("N",450,180);
    }

    tft.setTextSize(6);
    tft.setTextColor(GREEN);
    tft.drawString("COBRA", 155, 10);

    tft.setTextSize(2);
    tft.setTextColor(WHITE);
    tft.drawString("Push joystick to play again", 82, 290);

    tft.drawString(String(score),170,162);
    tft.drawString(String(high_score), 280, 162);

    tft.drawRect(75,285,58,25,WHITE);
    delay(250);
    tft.drawRect(75,285,58,25,BLUE);

}
