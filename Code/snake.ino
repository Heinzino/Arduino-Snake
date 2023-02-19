//Libraries used
#include <TFT_eSPI.h>

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
//MIGHT NEED TO FIND 4-DIGIT HEX FOR DIFF COLOUR OF GREENS -Aidan

//Create the screen variable from the library
TFT_eSPI tft = TFT_eSPI();

//Constants
//Pixels of the screen
const int BOARD_SIZE = 312;
const int NUM_OF_TILES = 8;
const int TILE_SIZE = BOARD_SIZE / NUM_OF_TILES;
const int START_X = 84;
const int START_Y = 4;

//1 is right, 2 is up, 3 is left, 4 is down
//7 is apple, 8 is snake, 9 is black tile, 10 is white tile
int direction = 1;
int score = 0;
int snakeLength = 3;

int snakeMap[NUM_OF_TILES][NUM_OF_TILES];
int snakeX[NUM_OF_TILES];
int snakeY[NUM_OF_TILES];


void setup() {
  Serial.begin(9600);

  //Makes background black and have a green border 
  tft.begin();
  tft.setRotation(1); //Makes starting point at top left corner of the screen when its horizontal
  tft.fillScreen(BLACK);
  tft.drawRect(START_X, START_Y, BOARD_SIZE, BOARD_SIZE, BLUE);
  
  //Starts of page
  howToPlayPage();
  //Creation of the board
  displayBoard();
  //Start position of snake middle of board
  snakeX[NUM_OF_TILES / 2] = 8; //Snake Head
  snakeY[NUM_OF_TILES / 2 - 1] = 8;
  snakeY[NUM_OF_TILES / 2] = 8;
  snakeY[NUM_OF_TILES / 2 + 1] = 8;

  createBoard();
  snakeMovement();

  if(snakeCollision()) {
    tft.println("YOU LOSE LMFAO");
    //display screen into all black or something with this on top and then have
    //A button that they can click to restart i.e. playAgain() function
  }
}


void loop() {

}

struct Button() {
  int xDir;
  int yDir;
  int width; 
  int length;
  char label[20];
};

//Idk yet tbh
struct fruit() {
  int xDir;
  int yDir;
}

//Coordinates are just test points (assumed)
Button next = {0, 0, 90, 45, "NEXT"};
Button prev = {0, 0, 90, 45, "PREV"};
Button play = {0, 0, 100, 100, "START"};
Button retry = {0, 0, 100, 100, "PLAY AGAIN"};

void howToPlayPage() {
  //Beginning of the page
  int pageNumber = 1;
  //This screen will show until the player clicks start to play the game
  while(!play.isClicked()) {

    /* Yea idk where i was going with this here
    if(next.isClicked()) {
      pageNumber += 1;
    }
    if(prev.isClicked()) {
      pageNumber -= 1;
    }
    */

    //Page 1
    if(pageNumber == 1 || (prev.isClicked() == true && pageNumber == 2)) {
      pageNumber += 1;
      tft.println("WELCOME TO SNAKE GAME!");
      tft.println("Before you begin please read through these pages!");
    }

    //Page 2
    //Theres probably a logic error here
    if((next.isClicked() == true && pageNumber == 1) || (prev.isClicked() == true && pageNumber == 3)) {
      //SHOW EVERYTHING IN PAGE TWO HERE
    }

    //Page 3
    if((next.isClicked() == true && pageNumber == 2)) {
      //SHOW EVERYTHING IN PAGE THREE HERE
    }
  }
}


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
      if(snakeMap[i][j] == 9) {//Black Tile 9
        tft.drawRect(TILE_SIZE*i + START_X, TILE_SIZE*j + START_Y, TILE_SIZE, TILE_SIZE, BLACK);
      } else { //White Tile 10
        tft.drawRect(TILE_SIZE*i + START_X, TILE_SIZE*j + START_Y, TILE_SIZE, TILE_SIZE, WHITE);
      }
    }
  }
}

void snakeMovement() {
  if (direction.isClicked() == 1) { //Snake Moves Right
    snakeX[0] += 1;
  } else if(direction.isClicked() == 2) { //Snake Moves Up
    snakeY[0] += 1; 
  } else if(direction.isClicked() == 3) { //Snake Moves Left
    snakeX[0] -= 1;
  } else if(direction.isClicked() == 4) { //Snake Moves Down
    snakeY[0] -= 1; 
  }
}

bool snakeCollision() {
  //Collision with borders
  if (snakeX[0] < 0 || snakeY[0] < 0 || snakeX[0] > NUM_OF_TILES || snakeX[0] > NUM_OF_TILES) {
    return true;
  }
  //Collision with itself
  for (int i = 1; i < snakeLength; i++) {
    if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
      return true;
    }
  }
  return false;
}

bool snakeCollision(int apple) {
  //Only checks if the head of the snake collides with where the apple is on the map  
}

void appleSpawn(int time) {
  int appleX = random(NUM_OF_TILES);
  int appleY = random(NUM_OF_TILES);
  bool noApple = true;

  while(!noApple) {
    for (int i = 1; i < snakeLength; i++) {
      //If the apple is on the snake
      if (appleX == snakeX[i] || appleY == snakeY[i]) {
        //It will assign a new positon for the apple to be in
        appleX = random(NUM_OF_TILES);
        appleY = random(NUM_OF_TILES);
      } else {
        //otherwise it will be put on the map 
        snakeMap[appleX][appleY] = 7;
        //apple is now on the map so it gets out of both loops
        noApple = false;
        break;
      }
    }
  }
}


//Do i even need this since there will be a button for this?
bool playAgain() {
  if(retry.isClicked()) {
    //resets the stats for everything
    snakeLength = 3;
    score = 0;
    direction = 1;
    return true;
  }
  return false;
}

bool winCondition() {
  //Checks score to compare with map of board to see if user won
  if (score == (5 * NUM_OF_TILES * NUM_OF_TILES)) {
    return true;
  } else {
    return false;
  }
}
