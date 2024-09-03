#include <Keypad.h>
#include <LiquidCrystal.h>

const int rs = 10, e = 11;
const int d4 = 12, d5 = 13, d6 = A0, d7 = A1;
LiquidCrystal lcd(rs, e, d4, d5, d6, d7);

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 9};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const String correctPasscode = "1234";
String enteredPasscode = "";
int attemptCount = 0;
const int maxAttempts = 3;
unsigned long lockoutStartTime;
const unsigned long lockoutDuration = 10000; 

void setup() {
  lcd.begin(16, 2);
  lcd.print("Enter Password:");
}

void loop() {
  if (attemptCount >= maxAttempts) {
    unsigned long currentTime = millis();
    if (currentTime - lockoutStartTime >= lockoutDuration) {
      attemptCount = 0;
      lcd.clear();
      lcd.print("Enter Password:");
    } else {
      lcd.setCursor(0, 1);
      lcd.print("Locked Out");
      return;
    }
  }

  char key = keypad.getKey();
  
  if (key) {
    if (key == '#') {
      if (enteredPasscode == correctPasscode) {
        lcd.clear();
        lcd.print("Access Granted");
        delay(2000);
        enteredPasscode = "";
      } else {
        lcd.clear();
        lcd.print("Access Denied");
        attemptCount++;
        if (attemptCount >= maxAttempts) {
          lockoutStartTime = millis();
          lcd.clear();
          lcd.print("Locked Out");
        } else {
          delay(2000);
          lcd.clear();
          lcd.print("Enter Password:");
        }
        enteredPasscode = "";
      }
    } else if (key == '*') {
      enteredPasscode = "";
      lcd.clear();
      lcd.print("Enter Password:");
    } else {
      enteredPasscode += key;
      lcd.setCursor(0, 1);
      lcd.print(enteredPasscode);
    }
  }
}

