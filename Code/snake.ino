const int TILE_SIZE = 8;
int score = 0;
int snakeMap[TILE_SIZE][TILE_SIZE];
int snakeLength = 3;
//1 is right, 2 is up, 3 is left, 4 is down
//7 is apple, 8 is snake, 9 is the tile of board
int direction = 1;
int snakeX[TILE_SIZE];
int snakeY[TILE_SIZE];


void setup() {
  //Start position of snake middle of board
  snakeX[TILE_SIZE / 2] = 8;
  snakeY[TILE_SIZE / 2 - 1] = 8;
  snakeY[TILE_SIZE / 2] = 8;
  snakeY[TILE_SIZE / 2 + 1] = 8;




  Serial.begin(9600);
  howToPlayPage();
  createBoard();
  snakeMovement();


  if(snakeCollision()) {
    Serial.println("YOU LOSE LMFAO");
    //display screen into all black or something with this on top and then have
    //A button that they can click to restart i.e. playAgain() function
  }
}


void loop() {
}


void howToPlayPage() {
  Serial.println("WELCOME TO SNAKE GAME!");
  Serial.println("Before you begin please read through these page for instructions!");
  //include 3 pages blah blah a next and prev button and start button at the end
  /*
  if (nextButton.isClicked() == true && nextButton == 2) {
    //do whatever here blah blah
  }
  if (nextButton.isClicked() == true && nextButton == 3) {
    //do whatever here blah blah
    if (startButton.isClicked() == true) {
      return;
    }
  }
  */
}


void createBoard() {
  //Board of the snake game


  //The board will go here, assuming 8x8
  for (int i = 0; i < TILE_SIZE; i++) {
    for (int j = 0; j < TILE_SIZE; j++) {
      snakeMap[i][j] = 9;
    }
  }
}


void snakeMovement() {
}


bool snakeCollision() {
  //Collision with borders
  if (snakeX[0] < 0 || snakeY[0] < 0 || snakeX[0] > TILE_SIZE || snakeX[0] > TILE_SIZE) {
    return true;
  }
  //Collision with itself
  for (int i = 1; i < snakeLength; i++) {
    if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
      return true;
    }
  }
  return false;
  //YHUHHH
}


bool snakeCollision(int apple) {
  if(snakeX[0] == apple) {


  }
}


void appleSpawn(int number) {
  snakeMap[random(TILE_SIZE)][random(TILE_SIZE)] = 7;
}
bool playAgain() {
  return true;
}


bool winCondition() {
  //Checks score to compare with map of board to see if user won
  if (score == (5 * TILE_SIZE * TILE_SIZE)) {
    return true;
  } else {
    return false;
  }
}
