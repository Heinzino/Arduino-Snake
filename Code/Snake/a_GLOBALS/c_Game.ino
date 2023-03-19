#include <C:\Users\HeinzV\Desktop\repo\GabeItches\Code\Snake\a_GLOBALS\b_Display.ino>
#include <C:\Users\HeinzV\Desktop\repo\GabeItches\Code\Snake\a_GLOBALS\a_GLOBALS.ino>
#include <C:\Users\HeinzV\Desktop\repo\GabeItches\Code\Snake\a_GLOBALS\d_Loop_Setup.ino>
#include <C:\Users\HeinzV\Desktop\repo\GabeItches\Code\Snake\a_GLOBALS\music.ino>

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

Apple spawnApple(){
  if(apple_counter == 0){

    long appleX, appleY;

    do{
    appleX = random(NUM_OF_TILES);
    appleY = random(NUM_OF_TILES);
    }
    while(snake.isBodyPart(appleX,appleY)); //Generate new if apple is part of body

    apple_counter++;
    return Apple(appleX,appleY);

  }
}

void eatingApple(Apple& apple){
  if(apple.collidesWith(snake.getHeadX(), snake.getHeadY())){
    apple_sound();
    apple_counter--;
    score++;
    snake.increaseLength();
  }
}


bool winCondition() {
  return (score == (NUM_OF_TILES*NUM_OF_TILES)-1);
}


void lose_game_handle(){
  lose_sound();
  tft.fillScreen(BLACK);
  if(high_score < score){
    high_score = score;
    new_high_score = true;
  }

  while(joystick_button_read == 1){ //joystick not pressed
      highscore_screen(new_high_score);
      joystick_button_read = digitalRead(JOYSTICK_BUTTON_PIN);
  }

  //Restart game if joystick pressed
  snake.reset();
  score = 0;
  new_high_score = false;
  Initialize_Screen_and_Board();
}