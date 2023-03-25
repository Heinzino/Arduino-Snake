#include <C:\Users\HeinzV\Desktop\repo\GabeItches\Code\Snake\a_GLOBALS\a_GLOBALS.ino>
#include <C:\Users\HeinzV\Desktop\repo\GabeItches\Code\Snake\a_GLOBALS\b_Display.ino>
#include <C:\Users\HeinzV\Desktop\repo\GabeItches\Code\Snake\a_GLOBALS\c_Game.ino>
#include <C:\Users\HeinzV\Desktop\repo\GabeItches\Code\Snake\a_GLOBALS\music.ino>


//Snake starts at (4,4) square
Snake snake(snake_start_x,snake_start_y);

void setup() {
  Serial.begin(9600);

  // Initalize the screen and set the orientation correctly, then make sure it's clear.
  Initialize_Screen_and_Board();

  //Joystick Initialzie
  pinMode(JOYSTICK_BUTTON_PIN, INPUT);
  digitalWrite(JOYSTICK_BUTTON_PIN, HIGH);

  //Intitialze Buzzers
  pinMode(buzzer, OUTPUT);
  pinMode(fx_buzzer, OUTPUT);


}

void loop(){
 Joystick_Direction();
 snake.move();
 display_snake_head();
 TailDisplay();
 Apple apple = spawnApple();
 eatingApple(apple);
 Display_Score_Screen(score);
 delay(1000);
}
