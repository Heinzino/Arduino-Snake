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
#define DARK_GREEN 0x0B60
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
const int apple_radius = TILE_SIZE/3;

const int white_tile = 10;
const int black_tile = 9;

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

  Initialze_Screen_and_Board();
  
  //Starts of page
  howToPlayPage();
  //Start position of snake middle of board
  snakeX[NUM_OF_TILES / 2] = 8; //Snake Head
  snakeY[NUM_OF_TILES / 2 - 1] = 8;
  snakeY[NUM_OF_TILES / 2] = 8;
  snakeY[NUM_OF_TILES / 2 + 1] = 8;

  snakeMovement();

  if(snakeCollision()) {
    tft.println("YOU LOSE LMFAO");
    //display screen into all black or something with this on top and then have
    //A button that they can click to restart i.e. playAgain() function
  }
}


void loop() {

}

struct Button {
  int xDir;
  int yDir;
  int width; 
  int length;
  char label[20];
  bool isClicked;
};

//Idk yet tbh
struct fruit {
  int xDir;
  int yDir;
};

//Coordinates are just test points (assumed)
Button next = {0, 0, 90, 45, "NEXT", false};
Button prev = {0, 0, 90, 45, "PREV", false};
Button play = {0, 0, 100, 100, "START", false};
Button retry = {0, 0, 100, 100, "PLAY AGAIN", false};


void Initialize_Screen_and_Board(){
  //Makes background black
  tft.begin();
  tft.setRotation(1); //Makes starting point at top left corner of the screen when its horizontal
  tft.fillScreen(BLACK);
  displayBoard();
  Spawn_Snake(3,3);   //Starts snake at (4,4) square on board
  Display_Score_Screen(0); //Display score of 0  -> Future should be more dyanmic
  
}

void Spawn_Snake(int x_tile_right, int y_tile_down){
  //Spawns green rectangle on (x_tile_right + 1, y_tile_down + 1) square on board
  int snake_spawn_x = START_X + (x_tile_right* TILE_SIZE);
  int snake_spawn_y = START_Y + (y_tile_down * TILE_SIZE);
  
  tft.fillRect(snake_spawn_x, snake_spawn_y, TILE_SIZE, TILE_SIZE, GREEN);

}

void howToPlayPage() {
  //Beginning of the page
  int pageNumber = 1;
  //This screen will show until the player clicks start to play the game
  while(!play.isClicked) {
    switch(pageNumber) {
      case 1:
        tft.println("WELCOME TO SNAKE GAME!");
        tft.println("Before you begin please read through these pages!");
        break;
      case 2:
        //SHOW EVERYTHING IN PAGE 2
        break;
      case 3: 
        //SHOW EVERYTHING IN PAGE 3
        break;
    }
    //To ensure user does not go above page 3
    if(next.isClicked) {
      pageNumber = min(pageNumber + 1, 3);
    }
    //To ensure user does not go below page 1
    if(prev.isClicked) {
      pageNumber = max(pageNumber - 1, 1);
    }
  }
}


void createBoard() {
  //Board of the snake game
  //The board will go here, assuming 8x8
  for (int i = 0; i < NUM_OF_TILES; i++) {
    for (int j = 0; j < NUM_OF_TILES; j++) {
      if((i + j) % 2 == 0) { //Black Tile 9 
        snakeMap[i][j] = black_tile;
      } else { //White Tile 10
        snakeMap[i][j] = white_tile;
      }
    }
  }
}

void displayBoard() {
  //This will display the whole board on the Arduino SPI LCD

  for (int i = 0; i < NUM_OF_TILES; i++) {
    for (int j = 0; j < NUM_OF_TILES; j++) {
      if(snakeMap[i][j] == black_tile) {//Black Tile 9
        tft.drawRect(TILE_SIZE*i + START_X, TILE_SIZE*j + START_Y, TILE_SIZE, TILE_SIZE, BLACK);
      } else { //White Tile 10
        tft.drawRect(TILE_SIZE*i + START_X, TILE_SIZE*j + START_Y, TILE_SIZE, TILE_SIZE, WHITE);
      }
    }
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

void snakeMovement() {
  /*
  if (direction.isClicked == 1) { //Snake Moves Right
    snakeX[0] += 1;
  } else if(direction.isClicked == 2) { //Snake Moves Up
    snakeY[0] += 1; 
  } else if(direction.isClicked == 3) { //Snake Moves Left
    snakeX[0] -= 1;
  } else if(direction.isClicked == 4) { //Snake Moves Down
    snakeY[0] -= 1; 
  }
  */
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
  bool apple = false;
  int counter = 0;

  //Got to be a more efficient way to place the apples this could go on for a long time and crash 
  while(!apple && counter < 50) {
    for (int i = 1; i < snakeLength; i++) {
      //If the apple is on the snake
      if (appleX == snakeX[i] || appleY == snakeY[i]) {
        //It will assign a new positon for the apple to be in
        appleX = random(NUM_OF_TILES);
        appleY = random(NUM_OF_TILES);
        counter++;
      } else {
        //otherwise it will be put on the map 
        snakeMap[appleX][appleY] = 7;
        //apple is now on the map so it gets out of both loops
        apple = true;
        break;
      }
    }
  }
}


//Do i even need this since there will be a button for this?
bool playAgain() {
  if(retry.isClicked) {
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
