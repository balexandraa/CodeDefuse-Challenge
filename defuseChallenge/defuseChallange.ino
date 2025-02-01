//simon variables
const int MAX_LEVEL = 5;
int sequence[MAX_LEVEL];
int your_sequence[MAX_LEVEL];
int level = 1;
int velocity = 500;
int buzzerPin = 6;

//morse variables
char stringToMorseCode[] = "bomb";
int led12 = 2;    // blink an led on output 12
int led6 = 3;    
//int audio8 = 6;   
int note = 1200;
int startButton = 8;
int dotLen = 300;            // length of the morse code 'dot'
int dashLen = dotLen + 150;  // length of the morse code 'dash'
int elemPause = dotLen;      // length of  the pause between elements of a character
int Spaces = dotLen * 3;     // length  of the spaces between characters
int wordPause = dotLen * 7;  // length of the  pause between words

//number variables
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Change the address (0x27) if your module has a different address
int expectedAnswerFirstSequence = 32;
int expectedAnswerSecondSequence = 25;
int expectedDefuseCode = 14;

int startButtonPin = 8;
bool startButtonPressed = false;
int buttonState = 0;

// Timer
#define USE_TIMER_1 true

#include <LedControl.h>
#include "TimerInterrupt.h"
#include "ISR_Timer.h"

int DIN = 11;
int CS = 7;
int CLK = 13;
int timer = 99;  // Initial value for timer in seconds
LedControl lc = LedControl(DIN, CLK, CS, 0);
//int startButton = 8;
int resetButton = A3;
int resetState = LOW;
int lastResetState = LOW;

int one1[8] = { B0100, B0110, B0100, B0100, B0100, B0100, B1110 };
int one2[8] = { B01000000, B01100000, B01000000, B01000000, B01000000, B01000000, B11100000 };
int zero1[8] = { B0110, B1001, B1001, B1001, B1001, B1001, B0110 };
int zero2[8] = { B01100000, B10010000, B10010000, B10010000, B10010000, B10010000, B01100000 };
int two1[8] = { B0110, B1001, B1000, B0100, B0010, B0001, B1111 };
int two2[8] = { B01100000, B10010000, B10000000, B01000000, B00100000, B00010000, B11110000 };
int three1[8] = { B0110, B1001, B1000, B1110, B1000, B1001, B0110 };
int three2[8] = { B01100000, B10010000, B10000000, B11100000, B10000000, B10010000, B01100000 };
int four1[8] = { B0100, B0110, B0101, B1111, B0100, B0100, B0100 };
int four2[8] = { B01000000, B01100000, B01010000, B11110000, B01000000, B01000000, B01000000 };
int five1[8] = { B0111, B0001, B0001, B0111, B1000, B1000, B0111 };
int five2[8] = { B01110000, B00010000, B00010000, B01110000, B10000000, B10000000, B01110000 };
int six1[8] = { B0110, B1001, B0001, B0111, B1001, B1001, B0110 };
int six2[8] = { B01100000, B10010000, B00010000, B01110000, B10010000, B10010000, B01100000 };
int seven1[8] = { B1111, B1000, B0100, B0010, B0010, B0010, B0010 };
int seven2[8] = { B11110000, B10000000, B01000000, B01000000, B01000000, B01000000, B01000000 };
int eight1[8] = { B0110, B1001, B1001, B0110, B1001, B1001, B0110 };
int eight2[8] = { B01100000, B10010000, B10010000, B01100000, B10010000, B10010000, B01100000 };
int nine1[8] = { B0110, B1001, B1001, B1110, B1000, B1001, B0110 };
int nine2[8] = { B01100000, B10010000, B10010000, B11100000, B10000000, B10010000, B01100000 };

int* digit1[10] = { zero1, one1, two1, three1, four1, five1, six1, seven1, eight1, nine1 };
int* digit2[10] = { zero2, one2, two2, three2, four2, five2, six2, seven2, eight2, nine2 };

void display(int d2, int d1) {
  for (int i = 0; i < 8; i++) {
    lc.setColumn(0, i, digit2[d2][i] | digit1[d1][i]);  // Use bitwise OR to combine the patterns
  }
}

void playDuhDuuhh() {
  tone(buzzerPin, 400);  // Play a 400 Hz tone (duh)
  delay(250);            // Wait for 500 milliseconds
  noTone(buzzerPin);     // Stop the tone
  delay(50);             // Pause for 100 milliseconds
  tone(buzzerPin, 300);  // Play a 300 Hz tone (duuhh)
  delay(1000);           // Wait for 1000 milliseconds (1 second)
  noTone(buzzerPin);     // Stop the tone
}

bool gameRunning = true; 

void victoryBuzzer() // the sound of the buzzer when the team wins
{
  tone(buzzerPin, 1000);
  delay(250);
  noTone(buzzerPin);
  delay(50);
  tone(buzzerPin, 600);
  delay(250);
  noTone(buzzerPin);
  delay(50);
  tone(buzzerPin, 1000);
  delay(1000);
  noTone(buzzerPin);
}

void timerInterrupt() {
  if (timer >= 0) {
    if(gameRunning)
    {
      display(timer % 10, timer / 10);
      --timer;
    }
    else
    {
      display(timer % 10, timer / 10); 

    }
  } else {
    //strikeBuzzer(); 
    display(0, 0);  // Display "00"
  }
}

void timerSetup() {
  lc.shutdown(0, false);
  lc.setIntensity(0, 1);  // Adjust intensity as needed (0-15)
  lc.clearDisplay(0);

  pinMode(resetButton, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);

  // Display the initial value on startup
  display(timer % 10, timer / 10);
}

// SIMON CODE

void simonSetup() {
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  pinMode(buzzerPin, OUTPUT);

  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
}

void simonLoop() {
  while (level < MAX_LEVEL) {
    if (level == 1)
      generate_sequence();  //generate a sequence;
    else                    //If start button is pressed or you're winning
    {
      //delay(100);
      show_sequence();  //show the sequence
      get_sequence();   //wait for your sequence
    }

    level++;
  }
}

void show_sequence() {
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);

  for (int i = 0; i < level; i++) {
    digitalWrite(sequence[i], HIGH);
    delay(velocity);
    digitalWrite(sequence[i], LOW);
    delay(200);
  }
}

void get_sequence() {
  int flag = 0;  //this flag indicates if the sequence is correct

  for (int i = 0; i < level; i++) {
    flag = 0;
    while (flag == 0) {
      if (digitalRead(A0) == LOW) {
        digitalWrite(5, HIGH);
        your_sequence[i] = 5;
        flag = 1;
        delay(200);
        if (your_sequence[i] != sequence[i]) {
          wrong_sequence();
          return;
        }
        digitalWrite(5, LOW);
      }

      if (digitalRead(A1) == LOW) {
        digitalWrite(4, HIGH);
        your_sequence[i] = 4;
        flag = 1;
        delay(200);
        if (your_sequence[i] != sequence[i]) {
          wrong_sequence();
          return;
        }
        digitalWrite(4, LOW);
      }

      if (digitalRead(A2) == LOW) {
        digitalWrite(3, HIGH);
        your_sequence[i] = 3;
        flag = 1;
        delay(200);
        if (your_sequence[i] != sequence[i]) {
          wrong_sequence();
          return;
        }
        digitalWrite(3, LOW);
      }

      if (digitalRead(A3) == LOW) {
        digitalWrite(2, HIGH);
        your_sequence[i] = 2;
        flag = 1;
        delay(200);
        if (your_sequence[i] != sequence[i]) {
          wrong_sequence();
          return;
        }
        digitalWrite(2, LOW);
      }
    }
  }
  right_sequence();
}

void generate_sequence() {
  randomSeed(millis());  //in this way is really random!!!

  for (int i = 0; i < MAX_LEVEL; i++) {
    sequence[i] = random(2, 6);
  }
}

void wrong_sequence() {

  lcd.setCursor(0, 1);
  lcd.print("Try again.");

  for (int i = 0; i < 3; i++) {

    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);

    tone(buzzerPin, 500, 200);

    delay(250);
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    delay(250);
  }
  level = 1;
  velocity = 1000;
  lcd.clear();
}

void right_sequence() {

    lcd.setCursor(0, 1);
    lcd.print("Correct!");

  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  delay(250);

  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);

  tone(buzzerPin, 1000, 500);

  delay(500);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  delay(500);

  lcd.clear();
  velocity -= 50;  //increase difficulty
}

// MORSE CODE

void morseSetup() {
  // initialize the digital pin as an output for LED  lights.
  pinMode(led12, OUTPUT);
  pinMode(led6, OUTPUT);

  pinMode(startButton, INPUT_PULLUP);
}

void strikeBuzzer() // the sound of the buzzer when the team makes a mistake
{
  tone(buzzerPin, 800);
  delay(300);
  noTone(buzzerPin);
  delay(100);

  tone(buzzerPin, 300);
  delay(300);
  noTone(buzzerPin);

}

//  Create a loop of the letters/words you want to output in morse code (defined in  string at top of code)
void morseLoop() {
  delay(500);
  while (1) {
    // Loop through the string and  get each character one at a time until the end is reached
    for (int i = 0; i < sizeof(stringToMorseCode) - 1; i++) {
      // Get the character in the  current position
      char tmpChar = stringToMorseCode[i];
      // Set the case  to lower case
      tmpChar = toLowerCase(tmpChar);
      // Call the subroutine to  get the morse code equivalent for this character
      GetChar(tmpChar);
    }

    //serial mode
    Serial.begin(9600);
    Serial.println("Introdu mesajul auzit!\n");
    while (!Serial.available()) delay(100);

    String input = Serial.readStringUntil('\n');
    //input.trim();
    if (input.equals(stringToMorseCode)) {
      lcd.setCursor(0, 1);
      lcd.print("Correct!");
      digitalWrite(led12, HIGH);
      digitalWrite(led6, HIGH);
      tone(buzzerPin, note);
      LightsOff(0);
      delay(1000);  // Change the delay time according to your preference
      return;
    } else {
      // Incorrect message: Blink LEDs 1 time and play a loser sound
      lcd.setCursor(0, 1);
      lcd.print("Try again.");
      strikeBuzzer();
      digitalWrite(led12, HIGH);
      digitalWrite(led6, HIGH);
      delay(1000);
      LightsOff(0);
    }
  }
}

// DOT
void MorseDot() {
  digitalWrite(led12, HIGH);  // turn the LED on
  digitalWrite(led6, LOW);
  tone(buzzerPin, note, dotLen);  // start playing a tone
  delay(dotLen);               // hold  in this position
}

// DASH
void MorseDash() {
  digitalWrite(led12, HIGH);  // turn the LED on
  digitalWrite(led6, HIGH);
  tone(buzzerPin, note, dashLen);  // start playing a tone
  delay(dashLen);               // hold in  this position
}

// Turn Off
void LightsOff(int delayTime) {
  digitalWrite(led12, LOW);  // turn the LED off
  digitalWrite(led6, LOW);
  noTone(buzzerPin);    // stop playing a tone
  delay(delayTime);  // hold in this position
}

// *** Characters to Morse Code  Conversion *** //
void GetChar(char tmpChar) {
  // Take the passed character  and use a switch case to find the morse code for that character
  switch (tmpChar) {
    case 'a':
      MorseDot();
      LightsOff(elemPause);
      MorseDash();
      LightsOff(Spaces);
      break;
    case 'b':
      MorseDash();
      LightsOff(elemPause);
      MorseDot();
      LightsOff(elemPause);
      MorseDot();
      LightsOff(elemPause);
      MorseDot();
      LightsOff(Spaces);
      break;
    case 'c':
      MorseDash();
      LightsOff(elemPause);
      MorseDot();
      LightsOff(elemPause);
      MorseDash();
      LightsOff(elemPause);
      MorseDot();
      LightsOff(Spaces);
      break;
    case 'd':
      MorseDash();
      LightsOff(elemPause);
      MorseDot();
      LightsOff(elemPause);
      MorseDot();
      LightsOff(Spaces);
      break;
    case 'e':
      MorseDot();
      LightsOff(Spaces);
      break;
    case 'f':
      MorseDot();
      LightsOff(elemPause);
      MorseDot();
      LightsOff(elemPause);
      MorseDash();
      LightsOff(elemPause);
      MorseDot();
      LightsOff(Spaces);
      break;
    case 'g':
      MorseDash();
      LightsOff(elemPause);
      MorseDash();
      LightsOff(elemPause);
      MorseDot();
      LightsOff(Spaces);
      break;
    case 'h':
      MorseDot();
      LightsOff(elemPause);
      MorseDot();
      LightsOff(elemPause);
      MorseDot();
      LightsOff(elemPause);
      MorseDot();
      LightsOff(Spaces);
      break;
    case 'i':
      MorseDot();
      LightsOff(elemPause);
      MorseDot();
      LightsOff(Spaces);
      break;
    case 'j':
      MorseDot();
      LightsOff(elemPause);
      MorseDash();
      LightsOff(elemPause);
      MorseDash();
      LightsOff(elemPause);
      MorseDash();
      LightsOff(Spaces);
      break;
    case 'k':
      MorseDash();
      LightsOff(elemPause);
      MorseDot();
      LightsOff(elemPause);
      MorseDash();
      LightsOff(Spaces);
      break;
    case 'l':
      MorseDot();
      LightsOff(elemPause);
      MorseDash();
      LightsOff(elemPause);
      MorseDot();
      LightsOff(elemPause);
      MorseDot();
      LightsOff(Spaces);
      break;
    case 'm':
      MorseDash();
      LightsOff(elemPause);
      MorseDash();
      LightsOff(Spaces);
      break;
    case 'n':
      MorseDash();
      LightsOff(elemPause);
      MorseDot();
      LightsOff(Spaces);
      break;
    case 'o':
      MorseDash();
      LightsOff(elemPause);
      MorseDash();
      LightsOff(elemPause);
      MorseDash();
      LightsOff(Spaces);
      break;
    case 'p':
      MorseDot();
      LightsOff(elemPause);
      MorseDash();
      LightsOff(elemPause);
      MorseDash();
      LightsOff(elemPause);
      MorseDot();
      LightsOff(Spaces);
      break;
    case 'q':
      MorseDash();
      LightsOff(elemPause);
      MorseDash();
      LightsOff(elemPause);
      MorseDot();
      LightsOff(elemPause);
      MorseDash();
      LightsOff(Spaces);
      break;
    case 'r':
      MorseDot();
      LightsOff(elemPause);
      MorseDash();
      LightsOff(elemPause);
      MorseDot();
      LightsOff(Spaces);
      break;
    case 's':
      MorseDot();
      LightsOff(elemPause);
      MorseDot();
      LightsOff(elemPause);
      MorseDot();
      LightsOff(Spaces);
      break;
    case 't':
      MorseDash();
      LightsOff(Spaces);
      break;
    case 'u':
      MorseDot();
      LightsOff(elemPause);
      MorseDot();
      LightsOff(elemPause);
      MorseDash();
      LightsOff(Spaces);
      break;
    case 'v':
      MorseDot();
      LightsOff(elemPause);
      MorseDot();
      LightsOff(elemPause);
      MorseDot();
      LightsOff(elemPause);
      MorseDash();
      LightsOff(Spaces);
      break;
    case 'w':
      MorseDot();
      LightsOff(elemPause);
      MorseDash();
      LightsOff(elemPause);
      MorseDash();
      LightsOff(Spaces);
      break;
    case 'x':
      MorseDash();
      LightsOff(elemPause);
      MorseDot();
      LightsOff(elemPause);
      MorseDot();
      LightsOff(elemPause);
      MorseDash();
      LightsOff(Spaces);
      break;
    case 'y':
      MorseDash();
      LightsOff(elemPause);
      MorseDot();
      LightsOff(elemPause);
      MorseDash();
      LightsOff(elemPause);
      MorseDash();
      LightsOff(Spaces);
      break;
    case 'z':
      MorseDash();
      LightsOff(elemPause);
      MorseDash();
      LightsOff(elemPause);
      MorseDot();
      LightsOff(elemPause);
      MorseDot();
      LightsOff(Spaces);
      break;
    default:
      // If a matching character was not found it will default to a  blank space
      LightsOff(Spaces);
  }
}

// NUMBER CODE

void numberSetup() {
  Serial.begin(9600);  // Initialize serial communication
  lcd.init();
  lcd.backlight();
}


void numberLoop() {
  // FIRTS SEQ
  while (1) {
    lcd.setCursor(0, 0);
    lcd.print("Complete the sequence");
    lcd.setCursor(0, 1);
    lcd.print("2, 4, 8, 16...");

    if (Serial.available() > 0) {
      int input = Serial.parseInt();
      if (input == expectedAnswerFirstSequence) {
        lcd.clear();
        lcd.print("Correct!");
        delay(1000);
        break;
      } else {
        lcd.clear();
        lcd.print("Try again.");
        delay(1500);  // Display "Incorrect. Try again." for 2 seconds
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Complete the sequence");
        lcd.setCursor(0, 1);
        lcd.print("2, 4, 8, 16...");
      }
    }

    // Clear the serial buffer
    while (Serial.available() > 0) {
      Serial.read();
    }
  }

  // SECOND SEQ
  while (1) {
    //lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Continue the sequence");
    lcd.setCursor(0, 1);
    lcd.print("1, 4, 9, 16...");

    if (Serial.available() > 0) {
      Serial.setTimeout(300000);
      int input = Serial.parseInt();
      if (input == expectedAnswerSecondSequence) {
        lcd.clear();
        lcd.print("Correct!");
        delay(1000);
        lcd.clear();
        break;
      } else {
        lcd.clear();
        lcd.print("Try again.");
        delay(2000);  // Display "Incorrect. Try again." for 2 seconds
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Continue the sequence");
        lcd.setCursor(0, 1);
        lcd.print("1, 4, 9, 16...");
      }
    }

    // Clear the serial buffer
    while (Serial.available() > 0) {
      Serial.read();
    }
  }

  // SERIAL BOMB
  while (1) {
    lcd.setCursor(1, 0);
    lcd.print("Serial number");
    lcd.setCursor(5, 1);
    lcd.print("789246");

    if (Serial.available() > 0) {
      int input = Serial.parseInt();
      if (input == expectedDefuseCode) {
        lcd.clear();
        break;
      } else {
        lcd.clear();
        lcd.print("Try again.");
        delay(2000);
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Serial number");
        lcd.setCursor(5, 1);
        lcd.print("789246");
      }
    }

    while (Serial.available() > 0) {
      Serial.read();
    }
  }
}



void win() {

  victoryBuzzer();

  lcd.setCursor(2, 0);
  lcd.print("Defused Bomb!");
  lcd.setCursor(7, 1);
  lcd.print(":)");

  while (true) {
    delay(1000);
  }
}

void loose() {
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("BOOOOOOOOOOM!");

  playDuhDuuhh();
  delay(1000);

  while (digitalRead(startButton) == HIGH) {
    delay(10);  // You can adjust this delay as needed
  }
}

void setup() {
  simonSetup();
  morseSetup();
  numberSetup();
  timerSetup();

  lcd.setCursor(0, 1);
  lcd.print("Press start...");
  while (digitalRead(startButton) == HIGH) {
    delay(10);  // You can adjust this delay as needed
  }

  lcd.clear();
  delay(500);
  // Start the timer!
  ITimer1.attachInterrupt(0.01, timerInterrupt);
}

void loop() {
  while (1) {
    simonLoop();
    if (timer < 0) break;
    morseLoop();
    if (timer < 0) break;
    numberLoop();
    if (timer < 0) break;
    gameRunning = false;
    win();
  }

  loose();
  timer = 99;
  level = 1;
}
