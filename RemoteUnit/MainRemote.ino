
//MainRemote.ino
// Contains the core of the remote unit

#include "LedControl.h"
#include <Keypad.h>
#include <arduino-timer.h>
#include <EEPROM.h>

//Pin D13 is connected to Clock (CLK)
//Pin D3 is connected to Load (CS)
//Pin D4 is connected to DataIn (DIN)

//Pins D5-6-7-8-9-10-11-12 connected to rotary knob

//Debug Vars;
int cmp = 0;

//Main variable, names are pretty much self-explanatory 
int Year  = 2020;
int Month = 01;
int Day   = 01;

int Hour    = 0;
int Minute  = 0;
int Second  = 0; 

int HourTZAlt   = 0; // Hour for the TimeZone/Alternate display
int MinuteTZAlt = 0; // Minute for the TimeZone/Alternate display

// Chrono variables, names are pretty much self-explanatory 
bool IsChronoActive   = false;
bool IsChronoVisible = false;
int ChronoHour   = 0;
int ChronoMinute = 0;
int ChronoSecond = 0;  
int ChronoTenthsSecond = 0;           
int ChronoThSecondOffset = 0;

// CountDown variables, names are pretty much self-explanatory 
bool IsCountDownActive  = false;
bool IsCountDownVisible = false;
int CountDownHour      = 0;
int CountDownMinute    = 0;
int CountDownSecond    = 0; 
bool IsCountDownDone   = false;

int LedIntensity      = 0x8; // Segment display intensity
const int EEPROM_LEDINTENSITY = 16;

bool LockIndicatorDot = false; // Blinking dot as a lock indicator

// Inputs
const int ClockTickPin = 2;  // Pin 2 receives the PPS (Pulse Per Second) from the Main Clock and triggers an Interrupt
const int KnobPin1     = 5;  // Pins D5 to D12 are connected to a rotary switch/knob and each pin correspond to a different position
const int KnobPin2     = 6;
const int KnobPin3     = 7;
const int KnobPin4     = 8;
const int KnobPin5     = 9;
const int KnobPin6     = 10;
const int KnobPin7     = 11;
const int KnobPin8     = 12;

// Bit->Segment relation for the bitmask
//  |--2--|
//  7     3
//  |     |
//  ---8---
//  |     |
//  6     4
//  |--5--|  .1.
// 0b12345678

//lookup table for bitmask values for numbers without dot (first row), numbers with dot (second row) and special chars (third row)
                       // 0          1          2          3          4          5          6          7          8          9
const int SpecChar[29] = {0b01111110,0b00110000,0b01101101,0b01111001,0b00110011,0b01011011,0b01011111,0b01110000,0b01111111,0b01111011,
                          0b11111110,0b10110000,0b11101101,0b11111001,0b10110011,0b11011011,0b11011111,0b11110000,0b11111111,0b11111011,
                          0b00000000,0b10000000,0b00001000,0b00000001,0b00110111,0b01110111,0b01100111,0b00111110,0b11111111};
                       // *          .          _          -          H          A          P          V          #
// Alias for indexes in the third row of SpecChar
const int BLANK = 20;
const int DOT   = 21;
const int UNDER = 22;
const int BAR   = 23;
const int CH_H  = 24;
const int CH_A  = 25;
const int CH_P  = 26;
const int CH_V  = 27;
const int TEST  = 28;

// Timer used in setup()
auto timer = timer_create_default();

// Set the pinout for the LED segment display
LedControl lc=LedControl(4,13,3,1);

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Init Timer, Set Pins/Interrupt
void setup() {
  // Run InterruptTick() when ClockTickPin receives the 1 Pulse Per Second from the Frequency Standard
  attachInterrupt(digitalPinToInterrupt(ClockTickPin), InterruptTick, FALLING);

  // Pins D5 to D12 are connected to a rotary switch/knob and each pin correspond to a different position
  pinMode(KnobPin1, INPUT_PULLUP);    //
  pinMode(KnobPin2, INPUT_PULLUP);    //
  pinMode(KnobPin3, INPUT_PULLUP);    //
  pinMode(KnobPin4, INPUT_PULLUP);    //
  pinMode(KnobPin5, INPUT_PULLUP);    //
  pinMode(KnobPin6, INPUT_PULLUP);    //
  pinMode(KnobPin7, INPUT_PULLUP);    //
  pinMode(KnobPin8, INPUT_PULLUP);    //


  // Initialize serial communication
  Serial.begin(9600);
  Serial.setTimeout(20);

  // The MAX72XX is in power-saving mode on startup, we have to do a wakeup call
  lc.shutdown(0, false);
  // Set the brightness to a medium values
  lc.setIntensity(0, LedIntensity);
  // And clear the display
  lc.clearDisplay(0);

  LedIntensity = EEPROM.read(EEPROM_LEDINTENSITY);
  if (LedIntensity > 15) {
    LedIntensity = 8;
    EEPROM.update(EEPROM_LEDINTENSITY, 8);
  }

  lc.setChar(0,7,' ',false);
  lc.setChar(0,6,'L',false);
  lc.setRow(0,5,0b00011101); // o
  lc.setChar(0,4,'d',false);
  lc.setRow(0,3,0b00010000); // i
  lc.setRow(0,2,0b00010101); // n
  lc.setRow(0,1,0b01011110); // G
  lc.setChar(0,0,' ',false);



  delay(1000);

  // Call the functions every x millisecond
  timer.every(10,  UpdateRemote);  // Keep everything up to date by listening on Serial
  timer.every(50,  UpdateDisplay); // Depending on the Knob position, call the appropriate display routine
  timer.every(100, UpdateChrono);  // If Chrono is displayed, keep the second update more accurate then when not displayed

}

//     #####     #####     #####     #####     #####
//#####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Main loop, only used to call various timers at the proper time
void loop() {

  timer.tick();

}

//     #####     #####     #####     #####     #####
//#####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Triggered by pin "ClockTickPin" when PPS received
void InterruptTick(void *) {
  ClockTick(NULL);

  LockIndicatorDot = true;
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Update Second, Minute, Hour, Day, Month and Year in sequence, Chrono and CountDown as well
//   Keep this one as quick as possible, called by InterruptTick()
void ClockTick(void *) {

  // Reset the 10ths of second
  int ChronoTenthsSecond = ChronoThSecondOffset;

  //Remote Clock increment/update
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
        // If January, March, May, July, August or October
        if (Month == 1 || Month == 3 || Month == 5 || Month == 7 || Month == 8 || Month == 10) {
          if (Day <= 30)
            Day++;
          else {
            Day = 1;
            Month++;
          }
        // If April, June, September or November
        } else if (Month == 4 || Month == 6 || Month == 9 || Month == 11) {
          if (Day <= 29)
            Day++;
          else {
            Day = 1;
            Month++;
          }
        // If December
        } else if (Month == 12) {
          if (Day <= 30)
            Day++;
          else {
            Day = 1;
            Month = 1;
            Year++;
          }
        // If February
        } else {
          // One leap year every year divisible by 4, but not if divisible by 100 unless it's divisible by 400
          if (Year%4==0 && (Year%100!=0 || Year%400==0)) {
            if (Day <= 28)
              Day++;
            else {
              Day = 1;
              Month++;
            }
          } else {
            if (Day <= 27)
              Day++;
            else {
              Day = 1;
              Month++;
            }
          }
        }
      }
    }
  }

  //Chrono increment
  if (IsChronoActive && !IsChronoVisible) {
    if (ChronoThSecondOffset != 0)
      ChronoTenthsSecond = ChronoThSecondOffset;
    else {
      ChronoTenthsSecond = 0;

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
          else
            ChronoHour = 0;
        }
      }
    }
  }

  //CountDown decrement
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
        else
          IsCountDownDone = true;
      }
    }
  }

  //Serial.println("TICK #####");
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Runs every 100ms, keep everything up to date by listening on Serial
bool UpdateRemote(void *) {
  String RecvBuff, SubBuff, TertiaryBuff;
  int TokenPos = 0;

  //If any bytes are available from Serial
  if (Serial.available() > 0) {
    RecvBuff = Serial.readString();
    RecvBuff.trim();

    if (RecvBuff != "") {
      TokenPos = RecvBuff.indexOf(':');
      SubBuff  = RecvBuff.substring(TokenPos+1);
      //Serial.println(RecvBuff.substring(TokenPos+1));

      if (RecvBuff.substring(0,TokenPos) == "SOURCE") {
        if (SubBuff == "INTERNAL")
          ClockTick(NULL);
      } else if (RecvBuff.substring(0,TokenPos) == "TIME") {
        //Serial.println(SubBuff);
  
        //Serial.print( SubBuff.substring(0, SubBuff.indexOf('-')) );
        //Serial.print(':');
        //Serial.print( SubBuff.substring(SubBuff.indexOf('-')+1, SubBuff.lastIndexOf('-')) );
        //Serial.print(':');
        //Serial.println( SubBuff.substring(SubBuff.lastIndexOf('-')+1) );

        Hour   = SubBuff.substring(0, SubBuff.indexOf('-')).toInt();
        Minute = SubBuff.substring(SubBuff.indexOf('-')+1, SubBuff.lastIndexOf('-')).toInt();
        Second = SubBuff.substring(SubBuff.lastIndexOf('-')+1).toInt();

      } else if (RecvBuff.substring(0,TokenPos) == "TIMEZONEALT") {
        HourTZAlt   = SubBuff.substring(0, SubBuff.indexOf('-')).toInt();
        MinuteTZAlt = SubBuff.substring(SubBuff.indexOf('-')+1).toInt();

      } else if (RecvBuff.substring(0,TokenPos) == "DATE") {
        Year   = SubBuff.substring(0, SubBuff.indexOf('-')).toInt();
        Month = SubBuff.substring(SubBuff.indexOf('-')+1, SubBuff.lastIndexOf('-')).toInt();
        Day = SubBuff.substring(SubBuff.lastIndexOf('-')+1).toInt();

      } else if (RecvBuff.substring(0,TokenPos) == "CHRONO") {
        ChronoHour   = SubBuff.substring(0, SubBuff.indexOf('-')).toInt();
        ChronoMinute = SubBuff.substring(SubBuff.indexOf('-')+1, SubBuff.lastIndexOf('-')).toInt();
        TertiaryBuff   = SubBuff.substring(SubBuff.lastIndexOf('-')+1).toInt();

        ChronoSecond          = TertiaryBuff.substring(0, TertiaryBuff.indexOf('/')).toInt();
        ChronoThSecondOffset  = TertiaryBuff.substring(TertiaryBuff.indexOf('/')+1).toInt();

      } else if (RecvBuff.substring(0,TokenPos) == "COUNTDOWN") {
        CountDownHour   = SubBuff.substring(0, SubBuff.indexOf('-')).toInt();
        CountDownMinute = SubBuff.substring(SubBuff.indexOf('-')+1, SubBuff.lastIndexOf('-')).toInt();
        CountDownSecond = SubBuff.substring(SubBuff.lastIndexOf('-')+1).toInt();

      } else if (RecvBuff.substring(0,TokenPos) == "ACTION") {
        TertiaryBuff = SubBuff.substring(SubBuff.indexOf('-')+1);

        //Serial.println("act:" + TertiaryBuff + "/" + SubBuff.substring(0, SubBuff.indexOf('-')) );

        if (SubBuff.substring(0, SubBuff.indexOf('-')) == "CHRONO") {
          if (TertiaryBuff.substring(TertiaryBuff.indexOf('/')+1) == "START")
            IsChronoActive = true;
          else if (TertiaryBuff.substring(TertiaryBuff.indexOf('/')+1) == "STOP")
            IsChronoActive = false;
        } else if (SubBuff.substring(0, SubBuff.indexOf('-')) == "COUNTDOWN") {
          if (TertiaryBuff.substring(TertiaryBuff.indexOf('/')+1) == "START")
            IsCountDownActive = true;
          else if (TertiaryBuff.substring(TertiaryBuff.indexOf('/')+1) == "STOP")
            IsCountDownActive = false;
          else if (TertiaryBuff.substring(TertiaryBuff.indexOf('/')+1) == "RESET") {
            IsCountDownActive = false;
            CountDownHour   = 0;
            CountDownMinute = 0;
            CountDownSecond = 0;
          }
        } else if (SubBuff.substring(0, SubBuff.indexOf('-')) == "DIM") {
          LedIntensity = max(TertiaryBuff.substring(TertiaryBuff.indexOf('/')+1).toInt(), 0);
          lc.setIntensity(0,LedIntensity);
          EEPROM.update(EEPROM_LEDINTENSITY, LedIntensity);
        }
      } //else {
        //Serial.println(RecvBuff);
      //}

    }
  }

  return true; // to repeat the action - false to stop
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// If Chrono is displayed, keep the second update more accurate then when not displayed
bool UpdateChrono(void *) {
  // Increment the 10ths of seconds for Chrono and take into acount the offset between it and the clock
  if (IsChronoActive && IsChronoVisible) { 
    if (ChronoTenthsSecond < 9)
      ChronoTenthsSecond++;
    else {
      if (ChronoThSecondOffset != 0) {
        ChronoTenthsSecond = 0;

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
            else
              ChronoHour = 0;
          }
        }
      }
    }
  }

  return true; // to repeat the action - false to stop
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Runs every 50ms, depending on the Knob position, call the appropriate display routine
bool UpdateDisplay(void *) {

  if (digitalRead(KnobPin1) == LOW)
    DisplayBlinkingDot(NULL);

  else if (digitalRead(KnobPin2) == LOW)
    DisplayCurrentTime(NULL);

  else if (digitalRead(KnobPin3) == LOW)
    DisplayTZAltTime(NULL);

  else if (digitalRead(KnobPin4) == LOW) {
    IsChronoVisible = false;
    DisplayCurrentDate(1);
  }
  else if (digitalRead(KnobPin5) == LOW) {
    IsChronoVisible    = false;
    DisplayCurrentDate(0);
  }
  else if (digitalRead(KnobPin6) == LOW) {
    IsChronoVisible    = true;
    DisplayCurrentChrono(NULL);
  }
  else if (digitalRead(KnobPin7) == LOW) {
    IsChronoVisible    = false;
    DisplayCurrentCountDown(NULL);
  }
  else if (digitalRead(KnobPin8) == LOW) {
    IsChronoVisible    = false;
    DisplayTestPatern(NULL);
  }
  else {
    IsChronoVisible    = false;
    DisplayTestPatern(NULL);
  }

  if (LedIntensity == -1)
    lc.shutdown(0,true);
  
  LockIndicatorDot = false;

  return true; // to repeat the action - false to stop
}
