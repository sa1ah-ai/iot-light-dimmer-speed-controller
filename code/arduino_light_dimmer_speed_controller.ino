#include <LiquidCrystal_I2C.h>
#include <Stepper.h>

// LCD for display
LiquidCrystal_I2C lcd(0x27, 20, 4); // 20x4 LCD display

// Stepper motor setup (adjust stepsPerRevolution based on your motor)
const int stepsPerRevolution = 200;
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

int potentiometer_pin = A0; // Potentiometer connected to A0

// Define buttons
#define bt_F A1 // Clockwise Button
#define bt_S A2 // Stop Button
#define bt_B A3 // Anticlockwise Button

int Mode = 1; // Start in Stop mode
int lastMode = -1;

void setup() {
  pinMode(potentiometer_pin, INPUT);
  pinMode(bt_F, INPUT_PULLUP);
  pinMode(bt_S, INPUT_PULLUP);
  pinMode(bt_B, INPUT_PULLUP);

  myStepper.setSpeed(1);

  lcd.begin(20, 4);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Stepper Motor Ctrl");
}

void loop() {
  // Read potentiometer value and map it to a motor speed range
  int potValue = analogRead(potentiometer_pin); 
  double lightBrightness = potValue / 4;
  double lightLcd = map(lightBrightness, 0,255,0,100);

  lcd.setCursor(0, 1);
  lcd.print("Brightness ");
  lcd.print(lightLcd);
  lcd.print("%   ");
  analogWrite(6, lightBrightness);

  int motorSpeed = map(potValue, 0, 1023, 1, 500);

  lcd.setCursor(0, 2);
  lcd.print("Speed: ");
  lcd.print(map(potValue, 0, 1023, 0, 100));
  lcd.print("%   ");

  if (digitalRead(bt_F) == LOW) {
    Mode = 2; // Clockwise
  } else if (digitalRead(bt_S) == LOW) {
    Mode = 1; // Stop
  } else if (digitalRead(bt_B) == LOW) {
    Mode = 3; // Anticlockwise
  }

  if (Mode != lastMode) {
    lcd.setCursor(0, 3);
    if (Mode == 1) lcd.print("      Stop      ");
    else if (Mode == 2) lcd.print("    Clockwise   ");
    else if (Mode == 3) lcd.print("  Anticlockwise ");
    lastMode = Mode;
  }

  if (motorSpeed > 1) {
    if (Mode == 2) { // Clockwise
      myStepper.setSpeed(motorSpeed);
      myStepper.step(stepsPerRevolution / 10);
    } else if (Mode == 3) { // Anticlockwise
      myStepper.setSpeed(motorSpeed);
      myStepper.step(-(stepsPerRevolution / 10));
    }
  } else {
    myStepper.setSpeed(1);
  }

  delay(50);
}
