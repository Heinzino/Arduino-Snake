#define BUZZER_PIN 8


int crunch_frequency = 100;
int crunch_duration = 100;

void setup() {
  // put your setup code here, to run once:
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  tone(BUZZER_PIN,500,200);
  delay(200);
  tone(BUZZER_PIN,500,200);
  delay(200);
  tone(BUZZER_PIN,500,200);
  delay(200);
  tone(BUZZER_PIN,800,150);
  delay(150);
  tone(BUZZER_PIN,500,500);
  delay(500);
  tone(BUZZER_PIN,600,1000);
  delay(10000);
}