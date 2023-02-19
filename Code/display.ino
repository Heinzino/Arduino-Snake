#include <TFT_eSPI.h>

//create the screen variable from the library
TFT_eSPI tft = TFT_eSPI();

// Set up variables for the cursor and counter. Cursor starts in the middle of the screen.
float cursorX = 240.0;
float cursorY = 160.0;
int resetCount = 0;

// Setting the joystick pins here so we can easily change them
#define JOYSTICK_X_PIN A7
#define JOYSTICK_Y_PIN A6
#define JOYSTICK_BUTTON_PIN 34

// Define colours in 4-digit hex                                  
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF


// Board Varaibles
const int BOARD_SIZE = 312;
const int NUM_OF_TILES = 8;
const int TILE_SIZE = BOARD_SIZE / NUM_OF_TILES;
const int START_X = 84;
const int START_Y = 4;

int snakeMap[NUM_OF_TILES][NUM_OF_TILES];
int snakeX[NUM_OF_TILES];
int snakeY[NUM_OF_TILES];


void createBoard() {
  //Board of the snake game
  //The board will go here, assuming 8x8
  for (int i = 0; i < NUM_OF_TILES; i++) {
    for (int j = 0; j < NUM_OF_TILES; j++) {
      if((i + j) % 2 == 0) { //Black Tile 9 
        snakeMap[i][j] = 9;
      } else { //White Tile 10
        snakeMap[i][j] = 10;
      }
    }
  }
}

void displayBoard() {
  //This will display the whole board on the Arduino SPI LCD
  for (int i = 0; i < NUM_OF_TILES; i++) {
    for (int j = 0; j < NUM_OF_TILES; j++) {
      if(snakeMap[i][j] == 10) {//Black Tile 9
      //White Tile 10
        tft.drawRect(TILE_SIZE*i + START_X, TILE_SIZE*j + START_Y, TILE_SIZE, TILE_SIZE, WHITE);
      }
    }
  }
}

void setup() {

  // Initalize the screen and set the orientation correctly, then make sure it's clear.
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(BLACK);
  tft.drawRect(int(START_X), int(START_Y),int(BOARD_SIZE),int(BOARD_SIZE), WHITE);
  displayBoard();

}

void loop(){
 
}