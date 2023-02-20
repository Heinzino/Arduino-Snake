#include <TFT_eSPI.h>

//create the screen variable from the library
TFT_eSPI tft = TFT_eSPI();

// Set up variables for the cursor and counter. Cursor starts in the middle of the screen.
float cursorX = 240.0;
float cursorY = 160.0;
int resetCount = 0;

// Setting the joystick pins here so we can easily change them
#define JOYSTICK_X_PIN A0
#define JOYSTICK_Y_PIN A1
#define JOYSTICK_BUTTON_PIN 46

// Define colours in 4-digit hex                                  
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define DARK_GREEN 0x0B60
#define GRASS_LIGHT_GREEN 0x0660
#define GRASS_DARK_GREEN 0x0500


// Board Varaibles
const int BOARD_SIZE = 312;
const int NUM_OF_TILES = 8;
const int TILE_SIZE = BOARD_SIZE / NUM_OF_TILES;
const int START_X = 84;
const int START_Y = 4;
const int apple_radius = TILE_SIZE/3.0;

const int white_tile = 10;
const int black_tile = 9;

const int score = 0;
const int apple_counter = 0;

int snakeMap[NUM_OF_TILES][NUM_OF_TILES];
int snakeX[NUM_OF_TILES];
int snakeY[NUM_OF_TILES];

int joystick_button_read;  // 1 is off 0 is on
int joystick_x_read; // 1023 is right, 0 is left
int joystick_y_read; // 1023 is down, 0 is up


void Display_Apple(int x_tile_right, int y_tile_down);
class Apple {
public:
  int x, y;

  Apple(int startX, int startY) {
    x = startX;
    y = startY;
    Display_Apple(x,y);
  }

  bool collidesWith(int otherX, int otherY) {
    return (x == otherX && y == otherY);
  }
};


class Snake {
  private:
    int headX, headY; // Coordinates of the snake's head
    int tailX, tailY; // Coordinates of the snake's tail
    int length; // Length of the snake
    int direction; // Current direction of the snake
    int bodyX[NUM_OF_TILES*NUM_OF_TILES-1]; // X-coordinates of the snake's body segments
    int bodyY[NUM_OF_TILES*NUM_OF_TILES-1]; // Y-coordinates of the snake's body segments

  public:
    // Constructor
    Snake(int startX, int startY) {
      headX = startX;
      headY = startY;
      tailX = startX;
      tailY = startY;
      bodyX[0] = startX;
      bodyY[0] = startY;
      length = 1;
      direction = 0; // 0: right, 1: down, 2: left, 3: up
    }

    // Moves the snake in its current direction
    void move() {
      // Update the coordinates of the head based on the current direction
      switch(direction) {
        case 0: // right
          headX++;
          break;
        case 1: // down
          headY++;
          break;
        case 2: // left
          headX--;
          break;
        case 3: // up
          headY--;
          break;
      }

      // Update the coordinates of the tail and body segments
       tailX = bodyX[0];
       tailY = bodyY[0];

        if (length > 1) {
          // Shift the coordinates of the body segments
          for (int i = 1; i < length; i++) {
            bodyX[i-1] = bodyX[i];
            bodyY[i-1] = bodyY[i];
          }
        }

        // Update the coordinates of the head
        bodyX[length-1] = headX;
        bodyY[length-1] = headY;

      // Check if the snake has collided with the wall or with itself
      if(headX < 0 || headX >= (NUM_OF_TILES-1) || headY < 0 || headY >= (NUM_OF_TILES-1)) {
        // Collided with the wall
        // TODO: Handle collision
      }
      else {
        for(int i = 0; i < length; i++) {
          if(headX == bodyX[i] && headY == bodyY[i]) {
            // Collided with itself
            // Handle collision
            break;
          }
        }
      }
    }

    // Changes the direction of the snake
    void changeDirection(int newDirection) {
      // Only allow changing direction if it's not opposite to the current direction
      if(abs(newDirection - direction) != 2) {
        direction = newDirection;
      }
    }

    // Increases the length of the snake
    void increaseLength() {
      length++;
    }

    // Returns the current direction of the snake
    int getDirection() {
      return direction;
    }

    // Returns the X-coordinate of the head
    int getHeadX() {
      return headX;
    }

    // Returns the Y-coordinate of the head
    int getHeadY() {
      return headY;
    }
    
    int getTailX() {
      return tailX;
    }

    int getTailY() {
      return tailY;
    }

    bool isBodyPart(int x, int y) {
      for (int i = 0; i < length; i++) {
        if (bodyX[i] == x && bodyY[i] == y) {
          return true;
        }
      }
    return false;
  }

    
};

//Snake starts at (4,4) square
Snake snake(3,3);

void setup() {
  Serial.begin(9600);

  // Initalize the screen and set the orientation correctly, then make sure it's clear.
  Initialize_Screen_and_Board();

  //Joystick Initialzie
  pinMode(JOYSTICK_BUTTON_PIN, INPUT);
  digitalWrite(JOYSTICK_BUTTON_PIN, HIGH);


}

void loop(){
 Serial.println("TEST");
 Joystick_Direction();
 snake.move();
 display_snake_head();
 TailDisplay();
 delay(1000);
}


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
  Display_Apple(-2,0);

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
  fill_tile(snake.getHeadX(),snake.getHeadY(), MAGENTA);   
  Snake_Eye();
}


void Initialize_Screen_and_Board(){
  //Makes background black
  tft.begin();
  tft.setRotation(1); //Makes starting point at top left corner of the screen when its horizontal
  tft.fillScreen(BLACK);
  createBoard();
  display_snake_head();
  Display_Score_Screen(score); //Display score of 0 
}

void Joystick_Direction(){
  //Read from Joystick
  joystick_button_read = digitalRead(JOYSTICK_BUTTON_PIN);
  joystick_x_read = analogRead(JOYSTICK_X_PIN);
  joystick_y_read = analogRead(JOYSTICK_Y_PIN);

  int x_equilibrium = 520;
  int y_equilibrium = 501;
  int buffer = 15;

  // Determine the dominant axis of the joystick movement
if((abs(joystick_x_read - x_equilibrium) > abs(joystick_y_read - y_equilibrium)) && ((abs(joystick_x_read - x_equilibrium) - abs(joystick_y_read - y_equilibrium)) > buffer) ) {
  // Move the snake horizontally
  if(joystick_x_read > x_equilibrium && (joystick_x_read - x_equilibrium > buffer)) {
    // Move right
    snake.changeDirection(0);
  }
  else {
    // Move left
    snake.changeDirection(2);
  }
}
else if ((abs(joystick_x_read - x_equilibrium) < abs(joystick_y_read - y_equilibrium)) && ((abs(joystick_y_read - y_equilibrium) - abs(joystick_x_read - x_equilibrium)) > buffer) ) {
  // Move the snake vertically
  if(joystick_y_read > y_equilibrium && (joystick_y_read - y_equilibrium) > buffer) {
    // Move down
    snake.changeDirection(1);
  }
  else {
    // Move up
    snake.changeDirection(3);
  }
}
else{
  //Keep Direction
  snake.changeDirection(snake.getDirection());
}

}

void spawnApple(){

  if(apple_counter == 0){

    int appleX, appleY;

    do{
    appleX = random(NUM_OF_TILES);
    appleY = random(NUM_OF_TILES);
    }
    while(snake.isBodyPart(appleX,appleY)); //Generate new if apple is part of body

    Apple apple(appleX,appleY);

  }
}