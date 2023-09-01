
#include "LedControl.h"
#include <Keypad.h>
#include <arduino-timer.h>

//Pin D13 is connected to Clock (CLK)
//Pin D3 is connected to Load (CS)
//Pin D4 is connected to DataIn (DIN)

//Pins D5-6-7-8 connected to Keypad pins A-D-H-L
//Pins D9-10-11-12 connected to Keypad pins G-BK-CJ-E

int cmp = 0;

int Year  = 2020;
int Month = 01;
int Day   = 01;

int Hour    = 0;
int Minute  = 0;
int Second  = 0;
int MilliSecond = 22;

bool IsChronoActive = false;
int ChronoHour   = 0;
int ChronoMinute = 0;
int ChronoSecond = 0;
int ChronoMilliSecond = 0;

bool IsCountDownActive = false;
int CountDownHour   = 0;
int CountDownMinute = 0;
int CountDownSecond = 0;
bool IsCountDownDone = false;

int DateDisplayMode = 0;
int TimeDisplayMode = 0;
int ChronoDisplayMode = 0;
int CountDownDisplayMode  = 0;

const int KnobPin = A1; //define pin A1 (analog pin)
int ReadKnobValue;   //current knob position
int CurrKnobValue = 0; //help to "debounce" the knob
int NewKnobValue  = 0; 
int DisplayMode   = 9;

char KeyPressVal = NULL;
bool NewKeyPress = false;
int CursorPos = 0;
int ToggleCurVal=0;
char ToggleCurChar='.';

const int ClockLockPin = A0; //define pin A0 (analog pin)
const int ClockTickPin = 2;
const int LedTickPin = A3;
const int LedLockPin = A4;
const int OutTickPin = A5;
const int OutLockPin = A6;

int LedIntensity = 0x8;

//
                       // 0          1          2          3          4          5          6          7          8          9
const int SpecChar[29] = {0b01111110,0b00110000,0b01101101,0b01111001,0b00110011,0b01011011,0b01011111,0b01110000,0b01111111,0b01111011,
                          0b11111110,0b10110000,0b11101101,0b11111001,0b10110011,0b11011011,0b11011111,0b11110000,0b11111111,0b11111011,
                          0b00000000,0b10000000,0b00001000,0b00000001,0b00110111,0b01110111,0b01100111,0b00111110,0b11111111};
                       // *          .          _          -          H          A          P          V          #
const int BLANK = 20;
const int DOT   = 21;
const int UNDER = 22;
const int BAR   = 23;
const int CH_H  = 24;
const int CH_A  = 25;
const int CH_P  = 26;
const int CH_V  = 27;
const int TEST  = 28;

auto timer = timer_create_default();

LedControl lc=LedControl(4,13,3,2);

const int ROW_NUM = 4; //four rows
const int COLUMN_NUM = 4; //four columns

char keys[ROW_NUM][COLUMN_NUM] = {
  {'U','D','L', 'R'},
  {'1','2','3', 'C'},
  {'4','5','6', 'E'},
  {'7','8','9', '0'}
};

byte pin_rows[ROW_NUM] = {12, 11, 10, 9}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {5, 6, 7, 8}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

void setup() {
  attachInterrupt(digitalPinToInterrupt(ClockTickPin), ClockTick, CHANGE);

  Serial.begin(9600); //initialize serial communication

  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(LedTickPin, OUTPUT);
  pinMode(LedLockPin, OUTPUT);

  // The MAX72XX is in power-saving mode on startup, we have to do a wakeup call
  lc.shutdown(0,false);
  lc.shutdown(1,false);
  // Set the brightness to a medium values
  lc.setIntensity(0,4);
  lc.setIntensity(1,8);
  // and clear the display
  lc.clearDisplay(0);
  lc.clearDisplay(1);

  // call the toggle_led function every 1000 millis (1 second)
  timer.every(1000, UpdateSecond);
  timer.every(100, UpdateMilliSec);

  timer.every(50, UpdateDisplay);
  timer.every(30, ReadKeypad);
  
  timer.every(100, ReadKnob);
  timer.every(500, ToggleCursor);
  
}

//     #####     #####     #####     #####     #####
//#####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

void loop() {

  timer.tick();

}

//     #####     #####     #####     #####     #####
//#####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

void ClockTick(void *) {
  digitalWrite(LedTickPin, HIGH);
  digitalWrite(OutTickPin, HIGH);

  MilliSecond = 0;

  if (Second < 59)
    Second++;
  else {
    Second = 0;
    if (Minute < 59)
      Minute++;
    else {
      Minute = 0;
      if (Hour < 23)
        Hour++;
      else {
        Hour = 0;
        if (Month == 1 || Month == 3 || Month == 5 || Month == 7 || Month == 8 || Month == 10) {
          if (Day < 30)
            Day++;
          else {
            Day = 1;
            Month++;
          }
        } else if (Month == 4 || Month == 6 || Month == 9 || Month == 11) {
          if (Day < 29)
            Day++;
          else {
            Day = 1;
            Month++;
          }
        } else if (Month == 12) {
          if (Day < 30)
            Day++;
          else {
            Day = 1;
            Month = 1;
            Year++;
          }
        } else {
          if (Year%4==0 && (Year%100!=0 || Year%400==0)) {
            if (Day < 28)
              Day++;
            else {
              Day = 1;
              Month++;
            }
          } else {
            if (Day < 27)
              Day++;
            else {
              Day = 1;
              Month++;
            }
          }
        }
        Serial.print("DATE:");
        Serial.print(Year);
        Serial.print("-");
        Serial.print(Month);
        Serial.print("-");
        Serial.println(Day);
      }
    }
  }

  if (IsChronoActive) {
    ChronoMilliSecond = 0;

    if (ChronoSecond < 59)
      ChronoSecond++;
    else {
      ChronoSecond = 0;
      if (ChronoMinute < 59)
        ChronoMinute++;
      else {
        ChronoMinute = 0;
        if (ChronoHour < 999)
          ChronoHour++;
        else {
          ChronoHour = 0;
        }
      }
    }
  }

  if (IsCountDownActive) {
    if (CountDownSecond > 0)
      CountDownSecond--;
    else {
      CountDownSecond = 59;
      if (CountDownMinute > 0)
        CountDownMinute--;
      else {
        CountDownMinute = 59;
        if (CountDownHour > 0)
          CountDownHour--;
        else {
          IsCountDownDone = true;
        }
      }
    }
  }

  digitalWrite(LedTickPin, LOW);
  digitalWrite(OutTickPin, LOW);
}

//     #####     #####     #####     #####     #####
//#####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

bool UpdateDisplay(void *) {

  switch(DisplayMode) {
    case 1:
      UserChangeDim(NULL);
    break;
    case 2:
      UserGetChrono(NULL);
    break;
    case 3:
      UserGetCountDown(NULL);
    break;
    case 4:
      UserChangeTime(NULL);
    break;
    case 5:
      UserChangeDate(NULL);
    break;
    case 6:
      UpdateSetDate(NULL);
    break;
    case 7:
      UpdateSetTime(NULL);
    break;
    case 8:
      UpdateSetCountDown(NULL);
    break;
    case 9:
    default:
      DisplayTestPatern(NULL);
  }
  
  return true; // to repeat the action - false to stop
}

//     #####     #####     #####     #####     #####
//#####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

bool UpdateSecond(void *) {

  int ClockLockValue = analogRead(ClockLockPin); // do a sensor reading

  if (ClockLockValue < 512) {
    digitalWrite(LedLockPin, LOW);
    digitalWrite(OutLockPin, LOW);
    ClockTick(NULL);
    Serial.println("SOURCE:INTERNAL");
  } else {
    digitalWrite(LedLockPin, HIGH);
    digitalWrite(OutLockPin, HIGH);
    Serial.println("SOURCE:LOCK");
  }

  Serial.print("TIME:");
  Serial.print(Hour);
  Serial.print("-");
  Serial.print(Minute);
  Serial.print("-");
  Serial.println(Second);

  if (IsChronoActive) {
    Serial.print("CHRONO:");
    Serial.print(ChronoHour);
    Serial.print("-");
    Serial.print(ChronoMinute);
    Serial.print("-");
    Serial.println(ChronoSecond);
  }

  if (IsCountDownActive) {
    Serial.print("COUNTDOWN:");
    Serial.print(CountDownHour);
    Serial.print("-");
    Serial.print(CountDownMinute);
    Serial.print("-");
    Serial.println(CountDownSecond);
  }

  return true; // to repeat the action - false to stop
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

bool UpdateMilliSec(void *) {
  if (MilliSecond < 90)
    MilliSecond += 10;

  if (IsChronoActive && ChronoMilliSecond < 90)
    ChronoMilliSecond += 10;

  return true; // to repeat the action - false to stop
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

bool ReadKeypad(void *) {
  char key = keypad.getKey();

  if (key){
    Serial.print("KEYPRESS:");
    Serial.println(key);
    NewKeyPress = true;
    KeyPressVal = key;
  }

  return true; // to repeat the action - false to stop
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

bool ToggleCursor(void *) {
  if (ToggleCurVal == 0) {
    if (ToggleCurVal <= 0) {
      ToggleCurVal++;
    }
  } else {
    if (ToggleCurVal >= 1) {
     ToggleCurVal--;
    }
  }

  //Serial.println(ToggleCurVal);

  return true; // to repeat the action - false to stop
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

bool ReadKnob(void *) {
  ReadKnobValue = analogRead(KnobPin); // do a sensor reading
  //digitalWrite(LED_BUILTIN, LOW);

  //Serial.print("Knob value is: "); //print a message to the serial monitor
  //Serial.println(ReadKnobValue); //print the value to the serial monitor

  //decode Knob position
  if(ReadKnobValue > 100 && ReadKnobValue < 200) { 
    NewKnobValue = 1;
  } else if (ReadKnobValue > 200 && ReadKnobValue < 300) {
    NewKnobValue = 2;
  } else if (ReadKnobValue > 300 && ReadKnobValue < 400) {
    NewKnobValue = 3;
  } else if (ReadKnobValue > 400 && ReadKnobValue < 500) {
    NewKnobValue = 4;
  } else if (ReadKnobValue > 500 && ReadKnobValue < 600) {
    NewKnobValue = 5;
  } else if (ReadKnobValue > 600 && ReadKnobValue < 700) {
    NewKnobValue = 6;
  } else if (ReadKnobValue > 700 && ReadKnobValue < 800) {
    NewKnobValue = 7;
  } else if (ReadKnobValue > 800 && ReadKnobValue < 900) {
    NewKnobValue = 8;
  } else if (ReadKnobValue > 900 && ReadKnobValue < 1000) {
    NewKnobValue = 9;
  } else {
    //digitalWrite(LED_BUILTIN, HIGH);
    NewKnobValue = 0;
  }

  if(NewKnobValue==CurrKnobValue) {
    if (DisplayMode != CurrKnobValue) {
      Serial.print("KNOB:");
      Serial.println(CurrKnobValue);
    }
    DisplayMode = CurrKnobValue;
  }

  CurrKnobValue = NewKnobValue;

  return true; // to repeat the action - false to stop
}