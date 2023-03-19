#include <TFT_eSPI.h>
#include <C:\Users\HeinzV\AppData\Local\Temp\.arduinoIDE-unsaved2023219-19316-11pw6wt.liroi\sketch_mar19c\music.h>
#include <C:\Users\HeinzV\Desktop\repo\GabeItches\Code\Snake\a_GLOBALS\c_Game.ino>
#include <C:\Users\HeinzV\Desktop\repo\GabeItches\Code\Snake\a_GLOBALS\b_Display.ino>

//create the screen variable from the library
TFT_eSPI tft = TFT_eSPI();

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
#define ORANGE 0xF4C6
#define PURPLE 0x78F6
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
uint32_t snake_colour = MAGENTA;

int score = 0;
int high_score = 0;
bool new_high_score = false;
int apple_counter = 0;
int snake_start_x = 3;
int snake_start_y = 3; //snake starts at (4,4) square

int snakeMap[NUM_OF_TILES][NUM_OF_TILES];
int snakeX[NUM_OF_TILES];
int snakeY[NUM_OF_TILES];

int joystick_button_read;  // 1 is off 0 is on
int joystick_x_read; // 1023 is right, 0 is left
int joystick_y_read; // 1023 is down, 0 is up


void Display_Apple(int x_tile_right, int y_tile_down);
void lose_game_handle();
void fill_tile(int x_tile_right, int y_tile_down, uint32_t colour);

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

      for(int i =0; i<(NUM_OF_TILES*NUM_OF_TILES-1);i++){
        bodyY[i] = -3; //Initialize out of screen so body doesn't bug 
      }

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

    
        // Shift the coordinates of the body segments
        for (int i = 1; i < length; i++) {
          bodyX[i-1] = bodyX[i];
          bodyY[i-1] = bodyY[i];
          //Update body display
          fill_tile(bodyX[i], bodyY[i], snake_colour);
        }
  

        // Update the coordinates of the head
        bodyX[length-1] = headX;
        bodyY[length-1] = headY;

      // Check if the snake has collided with the wall or with itself
      if(headX < 0 || headX >= (NUM_OF_TILES) || headY < 0 || headY >= (NUM_OF_TILES)) {
        lose_game_handle();
      }
      else if (length > 1){
        for(int i = 0; i < length; i++) {
          if(headX == bodyX[i] && headY == bodyY[i]) {
            // Collided with itself
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

  void reset(){

     //reset body list
    for(int i = 0; i<length; i++){
      bodyY[i] = -3; //out of screen
    }
  
    //reset variables 
    headX = snake_start_x;
    headY = snake_start_y;
    tailX = snake_start_x;
    tailY = snake_start_y;
    bodyX[0] = snake_start_x;
    bodyY[0] = snake_start_y;
    length = 1;
    direction = 0;

  }

    
};
