
//MainClock.ino
// Contains the core of the clock, all the timer used to initiate sensor read and the main Interrupt

// UserModes.ino
// Contains the rountines used to interact with Date/Time/Chrono/Countdown and Dim values using the Keypad
// The refresh rate is about 50ms and mostly controled by the MainClock routine UpdateDisplay()
//   calling routines here in UserModes
// Routine in DisplayModes are then used to output the current state

//UpdateModes.ino
// Contains the rountines used to update Date/Time/Countdown and LockMode values using the Keypad
// The refresh rate is about 50ms and mostly controled by the MainClock routine UpdateDisplay()
//   calling routines here in UpdateModes
// Routine in DisplayModes are then used to output the current state

//DisplayModes.ino
// Contains the rountines used to display information on the LED segment display
// Display 0 is on top, display 1 is bellow
// Usually 0 is for current mode in context and 1 is for current time
// Individual char are numbered from 7 to 0, fromt the left to the right
// The refresh rate is about 50ms and mostly controled by the MainClock routine UpdateDisplay()
//   calling intermediate routines in UpdateModes or UserModes that then call the routine here in DisplayModes

// ######################### ######################### ######################### ######################### #########################

// Init Timer, Load values from EEPROM, Set Pins/Interrupt/WatchDogTimer
//void setup()

// Main loop, only used to call various timers at the proper time
//void loop()

// Triggered by pin "ClockTickPin" when PPS received
//void InterruptTick(void *)

// Update Second, Minute, Hour, Day, Month and Year in sequence, Chrono and CountDown as well
//   Keep this one as quick as possible, called by InterruptTick()
//void ClockTick(void *)

// Runs every 100ms, many low priority actions are taken over the course of one second,
//   kept syncronized by the interupt reseting TenthsSecond to zero every seconds
//bool UpdateTenthsSec(void *)

// Runs every 1000ms, check the Frequency Standard lock status and call ClockTick() if not locked
//bool UpdateSecond(void *)

// Runs every 50ms, depending on the Knob position, call the appropriate display routine
//bool UpdateDisplay(void *)

// Runs every 25ms, scan the KeyPad matrix for any new keypress
//bool ReadKeypad(void *)

// Runs every 500ms, control blinking of the cursor
//bool ToggleCursor(void *)

// Runs every 100ms, convert analog Knob value to CurrKnobValue
//bool ReadKnob(void *)

#include "LedControl.h"
#include <Keypad.h>
#include <arduino-timer.h>
#include <avr/wdt.h>
#include <EEPROM.h>

//Pin D13 is connected to Clock (CLK)
//Pin D3 is connected to Load (CS)
//Pin D4 is connected to DataIn (DIN)

//Pins D5-6-7-8 connected to Keypad pins A-D-H-L
//Pins D9-10-11-12 connected to Keypad pins G-BK-CJ-E

//Debug Vars;
int cmp = 0;

// Addr x means the storage adress in EEPROM (0 to 1024)

//Main clock variable, names are pretty much self-explanatory 
int Year  = 2020;    // Addr 0-1       // Split in 2 integers
int Month = 01;      // Addr 2
int Day   = 01;      // Addr 3

int Hour    = 0;     // Addr 4-5       // wear leveling between Addr 4 and 5 to limit write under 100K over ~20Y, based on Hour%2
int Minute  = 0;     // Addr 100-195   // wear leveling between Addr 100 and 196 to limit write near 100K over ~20Y, based on ((Day%4)*(Hour+1))+Hour
int Second  = 0;     // Addr 200-919   // wear leveling between Addr 200 and 919 to limit write near 100K over ~20Y, limited to 10s precision, based on ((Hour%12)*(Minute+1))+Minute
int TenthsSecond = 2;

int HourTZAlt   = 0;                    // Hour for the TimeZone/Alternate display
int MinuteTZAlt = 0;                    // Minute for the TimeZone/Alternate display
int TimeZoneOffsetHr  = 12;  // Addr 7   // Offset between Hour and HourTZAlt
int TimeZoneOffsetMi  = 00;  // Addr 8   // Offset between Minute and MinuteTZAlt

bool DSTMode = false;       // Addr 9   // If the DaylightSavingTime is set

// Chrono variables, names are pretty much self-explanatory 
bool IsChronoActive = false; // Addr 10
int ChronoHour   = 0;        // Addr 11
int ChronoMinute = 0;        // Addr 920-929   // wear leveling between Addr 920 and 929 to limit write near 100K over ~2Y, limited to 10s precision, based on CrHour%10
int ChronoSecond = 0;        // Addr 940-979   // wear leveling between Addr 940 and 979 to limit write near 100K over ~2Y, limited to 15s precision, based on ((CrHour%2)*20)+(CrMinute%20)
int ChronoTenthsSecond   = 0;                 
int ChronoThSecondOffset = 0;

// Structure for storing a lap
struct ChronoLapStruct {
  int Hour   = 0;
  int Minute = 0;
  int Second = 0;
  int TenthsSecond = 0;
} ;
// Array to store 9 laps of Chrono
ChronoLapStruct ChronoLap[9] ; /*= { // Addr 50-94
  {0,0,0,0}, {0,0,0,0}, {0,0,0,0},
  {0,0,0,0}, {0,0,0,0}, {0,0,0,0},
  {0,0,0,0}, {0,0,0,0}, {0,0,0,0}
};*/

// CountDown variables, names are pretty much self-explanatory 
bool IsCountDownActive = false; // Addr 12
int CountDownHour      = 0;     // Addr 13
int CountDownMinute    = 0;     // Addr 930-939         // wear leveling between Addr 930 and 939 to limit write near 100K over ~2Y, limited to 10s precision, based on CdHour%10
int CountDownSecond    = 0;     // Addr 980-1019 [1020] // wear leveling between Addr 980 and 1020 to limit write near 100K over ~2Y, limited to 10s precision, based on ((CdHour%2)*20)+(CdMinute%20)
bool IsCountDownDone   = false; //                           * portion above 100 for second is stored at 1020

int LedIntensity      = 0x8;    // Addr 14   // Segment display intensity

//Display format variables
int DateDisplayMode      = 0;   // Addr 15   // Date format for the display
int TimeDisplayMode      = 0;   // Addr 16   // Time format for the display
int ChronoDisplayMode    = 0;   // Addr 17   // Time format for the Chrono
int ChronoLapMode        = 0;   // Addr 18   // Which Lap is displayed and if the delta with previous lap is shown (0=none) (>10=with delta)
int CountDownDisplayMode = 0;   // Addr 19   // Time format for the CountDown

// When LockMode is >= to the listed values, the following fonctionalities are unavailable
// 0 = None
// 1 = Lock/PassCode Change
// 2 = Date/Hour/Second Change
// 3 = TimeZone/DST Set
// 4 = Countdown/Chrono/Laps Change/Set
// 5 = Display Format Set
// 6 = Dim Set
int LockMode = 0;             // Addr 20      // See above
int Unlocked = 0;                             // If the correct PassCode had been typed under "UpdateSetLock()" (2==failed)
int LockPassCode[16];         // Addr 22-38   // Stored PassCode to be compared with TestPassCode 
int TestPassCode[16];                         // PassCode currently being typed
int LockPassCodeLength = 0;   // Addr 39      // Number of "char" stored in LockPassCode
int TestPassCodeLength = 0;                   // Number of "char" typed in TestPassCode

// Control Knob related variables
int CurrKnobValue = 0;  // Most recent value from ReadKnob(), once "debounced" it is used to set DisplayMode
int DisplayMode   = 9;  // The end result of the knob position is available here

// Keypad related variables
char KeyPressVal   = NULL;   // Value of the current key being pressed, set by ReadKeypad() (see variable char AvailableKeys)
bool NewKeyPress   = false;  // Set by ReadKeypad(), resetted by the fonction consuming the keypress
int  CursorPos     = 7;      // Position of the edit cursor
bool ToggleCurVal  = false;  // Set by ToggleCursor() to control blinking of the char under the cursor
byte ToggleCurChar = '.';    // Stores the char at CursorPos to be edited in Update Modes

// Serial output out of sync buffer
String MessageBuffer;

// Inputs
const int ClockLockPin = A0; // Pin Analog 0 receives the lock signal from the Frequency Standard
const int ClockTickPin = 2;  // Pin 2 receives the PPS (Pulse Per Second) from the Frequency Standard and triggers an Interrupt
const int KnobPin      = A1; // Pin A1 is connected to a Resistor Network making every 9 knob positions correspond to a different voltage value
// Outputs
const int LedTickPin = A3;   // Drives the LED to display the PPS
const int LedLockPin = A4;   // Drives the LED to indicate the lock status
const int OutTickPin = A5;   // Drives the pin E on the rear Data jack (2J2)
const int OutLockPin = A6;   // Drives the pin F on the rear Data jack (2J2)

//All Addr constants for EEPROM
const int EEPROM_YEAR                 = 0;   // Size 2
const int EEPROM_MONTH                = 2;
const int EEPROM_DAY                  = 3;
const int EEPROM_HOUR                 = 4;   // Size 2
const int EEPROM_MINUTE               = 100; // Size 96
const int EEPROM_SECOND               = 200; // Size 720
const int EEPROM_TIMEZONEOFFSETHR     = 7;
const int EEPROM_TIMEZONEOFFSETMI     = 8;
const int EEPROM_DTSMODE              = 9;
const int EEPROM_CHRONOACTIVE         = 10;
const int EEPROM_CHRONOHOUR           = 11;  // Size 2
const int EEPROM_CHRONOMINUTE         = 920; // Size 10
const int EEPROM_CHRONOSECOND         = 940; // Size 40
const int EEPROM_CHRONOLAPLIST        = 50;  // Size 45
const int EEPROM_COUNTDOWNACTIVE      = 13;
const int EEPROM_COUNTDOWNHOUR        = 14;  // Size 2
const int EEPROM_COUNTDOWNMINUTE      = 930; // Size 10
const int EEPROM_COUNTDOWNSECOND      = 980; // Size 40
const int EEPROM_COUNTDOWNSECONDOVER  = 1020;
const int EEPROM_LEDINTENSITY         = 16;
const int EEPROM_DATEDISPLAYMODE      = 17;
const int EEPROM_TIMEDISPLAYMODE      = 18;
const int EEPROM_CHRONODISPLAYMODE    = 19;
const int EEPROM_CHRONOLAPMODE        = 20;
const int EEPROM_COUNTDOWNDISPLAYMODE = 21;
const int EEPROM_LOCKMODE             = 22;
const int EEPROM_LOCKPASSCODE         = 23;  // Size 16
const int EEPROM_LOCKPASSCODELENGTH   = 40;
const int EEPROM_TESTVAL              = 1023; 

//Space Left: 6, 41-49, 95-99, 196-199, 1021, 1022 = 21 bytes

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
LedControl lc=LedControl(4,13,3,2);

// Keypad size
const int ROW_NUM = 4;    //4 rows
const int COLUMN_NUM = 4; //4 columns

// Matrix of keys on the keypad, used to set the global variable char KeyPressVal by ReadKeypad()
char AvailableKeys[ROW_NUM][COLUMN_NUM] = {
  {'U','D','L', 'R'},
  {'1','2','3', 'C'},
  {'4','5','6', 'E'},
  {'7','8','9', '0'}
};

// Set the pinout for the keypad
byte pin_rows[ROW_NUM] = {12, 11, 10, 9};
byte pin_column[COLUMN_NUM] = {5, 6, 7, 8};
Keypad keypad = Keypad( makeKeymap(AvailableKeys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Init Timer, Load values from EEPROM, Set Pins/Interrupt/WatchDogTimer
void setup() {
  wdt_reset();
  // Disable WatchDogTimer
  wdt_disable();

  int MemTestValue = 0;

  // Run InterruptTick() when ClockTickPin receives the 1 Pulse Per Second from the Frequency Standard
  attachInterrupt(digitalPinToInterrupt(ClockTickPin), InterruptTick, FALLING);

  // Initialize serial communication
  Serial.begin(9600);

  pinMode(LedTickPin, OUTPUT);
  pinMode(LedLockPin, OUTPUT);
  pinMode(OutTickPin, OUTPUT);
  pinMode(OutLockPin, OUTPUT);

  // The MAX72XX is in power-saving mode on startup, we have to do a wakeup call
  lc.shutdown(0, false);
  lc.shutdown(1, false);
  // Set the brightness to a medium values
  lc.setIntensity(0, LedIntensity);
  lc.setIntensity(1, LedIntensity);
  // And clear the display
  lc.clearDisplay(0);
  lc.clearDisplay(1);

  lc.setChar(0,7,'L',false);
  lc.setRow(0,6,0b00011101); // o
  lc.setChar(0,5,'A',false); 
  lc.setChar(0,4,'d',false);
  lc.setRow(0,3,0b00010000); // i
  lc.setRow(0,2,0b00010101); // n
  lc.setRow(0,1,0b01011110); // G
  lc.setChar(0,0,' ',false);

  delay(1000);

  // Fetch the values saved to EEPROM if 
  MemTestValue = EEPROM.read(EEPROM_TESTVAL);
  if (MemTestValue == 22)
    GetEEPROMVal(NULL);
   else {
    // Set the array to zero
    for(int i=0; i<9; i++) {
      ChronoLap[i].Hour         = 0;
      ChronoLap[i].Minute       = 0;
      ChronoLap[i].Second       = 0;
      ChronoLap[i].TenthsSecond = 0;
      for(int j=0; j<5; j++)
        EEPROM.update(EEPROM_CHRONOLAPLIST+i+j, 0);
    }
    // Init all EEPROM Addrs
    EEPROM.update(EEPROM_YEAR, 20);
    EEPROM.update(EEPROM_YEAR + 1, 20);
    EEPROM.update(EEPROM_MONTH, 1);
    EEPROM.update(EEPROM_DAY, 1);
    EEPROM.update(EEPROM_HOUR, 0);
    EEPROM.update(EEPROM_HOUR + 1, 23);
    EEPROM.update(EEPROM_MINUTE + 1, 0); // ((Day%4)*(Hour+1))+Hour
    EEPROM.update(EEPROM_SECOND + 0, 0); // ((Hour%12)*(Minute+1))+Minute
    EEPROM.update(EEPROM_TIMEZONEOFFSETHR, 12);
    EEPROM.update(EEPROM_TIMEZONEOFFSETMI, 00);
    EEPROM.update(EEPROM_DTSMODE, 0);
    EEPROM.update(EEPROM_CHRONOACTIVE, 0);
    EEPROM.update(EEPROM_CHRONOHOUR, 0); 
    EEPROM.update(EEPROM_CHRONOHOUR+1, 0); 
    EEPROM.update(EEPROM_CHRONOMINUTE, 0); // CrHour%10
    EEPROM.update(EEPROM_CHRONOSECOND, 0); // ((CrHour%2)*20)+(CrMinute%20)
    EEPROM.update(EEPROM_COUNTDOWNACTIVE, 0);
    EEPROM.update(EEPROM_COUNTDOWNHOUR, 0);
    EEPROM.update(EEPROM_COUNTDOWNHOUR+1, 0);
    EEPROM.update(EEPROM_COUNTDOWNMINUTE, 0); // CdHour%10
    EEPROM.update(EEPROM_COUNTDOWNSECOND, 0); // ((CdHour%2)*20)+(CdMinute%20)
    EEPROM.update(EEPROM_COUNTDOWNSECONDOVER, 0);
    EEPROM.update(EEPROM_LEDINTENSITY, 8);
    EEPROM.update(EEPROM_DATEDISPLAYMODE, 0);
    EEPROM.update(EEPROM_TIMEDISPLAYMODE, 0);
    EEPROM.update(EEPROM_CHRONODISPLAYMODE, 0);
    EEPROM.update(EEPROM_CHRONOLAPMODE, 0);
    EEPROM.update(EEPROM_COUNTDOWNDISPLAYMODE, 0);
    EEPROM.update(EEPROM_LOCKMODE, 0);
    EEPROM.update(EEPROM_LOCKPASSCODELENGTH, 0);
    EEPROM.update(EEPROM_TESTVAL, 22);
  }

  // Call the functions every x millisecond
  timer.every(1000, UpdateSecond);    // Checks the Frequency Standard lock status and calls ClockTick() if not locked
  timer.every(100,  UpdateTenthsSec); // Many low priority actions are taken over the course of one second,
                                      //   kept syncronized by the interupt reseting TenthsSecond to zero every seconds,
                                      //   also updates the 10ths of seconds
  timer.every(50,  UpdateDisplay);    // Depending on the Knob position, call the appropriate display routine
  timer.every(25,  ReadKeypad);
  timer.every(100, ReadKnob);
  timer.every(500, ToggleCursor);     // Controls blinking of the cursor
  
  // Watchdog Timer set to 8 sec, reset in ClockTick() every 1 sec.
  wdt_enable(WDTO_4S);
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
}

//     #####     #####     #####     #####     #####
//#####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Update Second, Minute, Hour, Day, Month and Year in sequence, Chrono and CountDown as well
//   Keep this one as quick as possible, called by InterruptTick()
void ClockTick(void *) {

  digitalWrite(OutTickPin, HIGH);
  if (LedIntensity < 0)
    digitalWrite(LedTickPin, HIGH);

  // Poke the WatchDogTimer
  wdt_reset();

  // Reset the 10ths of second
  TenthsSecond = 0;

  //Main Clock increment/update
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

  //Chrono increment, take into acount the offset between it and the main clock for the 10ths of seconds
  if (IsChronoActive) {
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
        else{
          IsCountDownDone   = true;
          IsCountDownActive = false;
          CountDownSecond = 0;
          CountDownMinute = 0;
        }
      }
    }
  }

  if (LedIntensity < 0)
    digitalWrite(LedTickPin, LOW);

  digitalWrite(OutTickPin, LOW);
}

//     #####     #####     #####     #####     #####
//#####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Runs every 100ms, many low priority actions are taken over the course of one second,
//   kept syncronized by the interupt reseting TenthsSecond to zero every seconds
bool UpdateTenthsSec(void *) {
  // "switch(TenthsSecond)" allows the computational load to be spread over the full second
  switch(TenthsSecond) {
    case 0: // Update Second and Minute for the TimeZone Alternative values (KEEP FIRST!)
      if (LedIntensity >= 0)
        digitalWrite(LedTickPin, HIGH);

      if (Second == 0)
        UpdateTZAlt(NULL); 
    break;
    case 1: // Write Time to EEPROM, updated every 10 seconds
      digitalWrite(LedTickPin, LOW);

      if (Second%10 == 0) {
        EEPROM.write(EEPROM_SECOND+(((Hour%12)*(Minute+1))+Minute), Second); // Spread writes over 720 bytes overtime to limit wear on non-volatil memory
        EEPROM.update(EEPROM_MINUTE+(((Day%4)*(Hour+1))+Hour), Minute);  // Spread writes over 96 bytes overtime to limit wear on non-volatil memory
        if(Minute == 0) { 
          EEPROM.update(EEPROM_HOUR+(Hour%2), Hour);
        }
      }
    break;   
    case 2: // Output status over Serial, available on the pin B of the rear Data jack (2J2)
      Serial.print("TIME:");
      Serial.print(Hour);
      Serial.print("-");
      Serial.print(Minute);
      Serial.print("-");
      Serial.println(Second);
    break;
    case 3: // Write Date to EEPROM, updated every 4 hours
      if(Minute == 0 && Hour%4 == 0) {
        if (Second == 0) {
          EEPROM.update(EEPROM_DAY, Day);
          EEPROM.update(EEPROM_MONTH, Month);
        } else if (Second == 1) {
          EEPROM.update(EEPROM_YEAR+1, Year%100);
          EEPROM.update(EEPROM_YEAR, Year/100);
        }
      }
    break;  
    case 4: // Output status over Serial, available on the pin B of the rear Data jack (2J2)
      if (IsCountDownActive || (Second+2)%5 == 0) {
        Serial.print("COUNTDOWN:");
        Serial.print(CountDownHour);
        Serial.print("-");
        Serial.print(CountDownMinute);
        Serial.print("-");
        Serial.println(CountDownSecond);
      }
    break;     
    case 5: // Output status over Serial, available on the pin B of the rear Data jack (2J2)
      if (IsChronoActive || (Second+4)%5 == 0) {
        Serial.print("CHRONO:");
        Serial.print(ChronoHour);
        Serial.print("-");
        Serial.print(ChronoMinute);
        Serial.print("-");
        Serial.print(ChronoSecond);
        Serial.print("/");
        Serial.println(ChronoThSecondOffset);
      }
    break;    
    case 6: // Write chrono to EEPROM, every ~15 seconds, but 1~2 seconds out of sync with main clock
      if ((Second+1)%15 == 0) {
        EEPROM.update(EEPROM_CHRONOSECOND+((ChronoHour%2)*20)+(ChronoMinute%20), ChronoSecond);
        EEPROM.update(EEPROM_CHRONOMINUTE+(ChronoHour%10), ChronoMinute);
      } else if((Second+2)%15 == 0 && ChronoMinute == 0) {
        EEPROM.update(EEPROM_CHRONOHOUR, ChronoHour/100);
        EEPROM.update(EEPROM_CHRONOHOUR+1, ChronoHour%100);
      }
    break;
    case 7: // Output status over Serial, available on the pin B of the rear Data jack (2J2)
      if (Second%5 == 0) {
        Serial.print("DATE:");
        Serial.print(Year);
        Serial.print("-");
        Serial.print(Month);
        Serial.print("-");
        Serial.println(Day);
      } else if ((Second+1)%5 == 0) {
        Serial.print("TIMEZONEALT:");
        Serial.print(HourTZAlt);
        Serial.print("-");
        Serial.println(MinuteTZAlt);
      } else if ((Second+2)%5 == 0)
        IsCountDownActive ? Serial.println("ACTION:COUNTDOWN-START") :  Serial.println("ACTION:COUNTDOWN-STOP");
      else if ((Second+4)%5 == 0) 
        IsChronoActive ? Serial.println("ACTION:CHRONO-START") :  Serial.println("ACTION:CHRONO-STOP");
      else if ((Second+3)%5 == 0) 
        Serial.println("ACTION:DIM-" + String(LedIntensity));      
    break;
    case 8: // Write countdown to EEPROM, every ~15 seconds for CountDown, but 3~4 seconds out of sync with main clock
      if ((Second+3)%15 == 0) {
        EEPROM.update(EEPROM_COUNTDOWNSECONDOVER, CountDownSecond/100);
        EEPROM.update(EEPROM_COUNTDOWNSECOND+((CountDownHour%2)*20)+(CountDownMinute%20), CountDownSecond%100);
        
        EEPROM.update(EEPROM_COUNTDOWNMINUTE+(CountDownHour%10), CountDownMinute);
      } else if((Second+4)%15 == 0 && CountDownMinute == 0) {
        EEPROM.update(EEPROM_COUNTDOWNHOUR, CountDownHour/100);
        EEPROM.update(EEPROM_COUNTDOWNHOUR+1, CountDownHour%100);
      }
    break;  
    case 9: // Output message buffer over Serial, available on the pin B of the rear Data jack (2J2)
      if (MessageBuffer != "") {
        Serial.println(MessageBuffer);
        MessageBuffer = "";
      }
    break;
  }

  // Increment the 10ths of seconds for main clock
  if (TenthsSecond < 9)
    TenthsSecond++;

  // Increment the 10ths of seconds for Chrono and take into acount the offset between it and the main clock
  if (IsChronoActive) { 
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
//#####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Runs every 1000ms, check the Frequency Standard lock status and call ClockTick() if not locked
bool UpdateSecond(void *) {
  // This function may slowly drift away/out of sync from the ClockTick() called by InterruptTick()
  //   Do not use for anything that need to be in sync with the proper PPS, use UpdateTenthsSec()

  int ClockLockValue = analogRead(ClockLockPin); // do a sensor reading

  // Indicate if the clock is runing on internal oscillator or if the Frequency Standard as a lock
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
  
  return true; // to repeat the action - false to stop
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Runs every 50ms, depending on the Knob position, call the appropriate display routine
bool UpdateDisplay(void *) {
  //Relock when leaving Mode 9 (Lock settings)
  if (DisplayMode != 9)
    Unlocked = 0;

  switch(DisplayMode) {
    case 1:
      if (LockMode < 6)
        UserChangeDim(NULL);
      else
        ShowLockScreen(NULL);
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
      if (LockMode < 2)
        UpdateSetDate(NULL);
      else
        ShowLockScreen(NULL);
    break;
    case 7:
      if (LockMode < 2)
        UpdateSetTime(NULL);
      else
        ShowLockScreen(NULL);
    break;
    case 8:
      if (LockMode < 4)
        UpdateSetCountDown(NULL);
      else
        ShowLockScreen(NULL);
    break;
    case 9:
      UpdateSetLock(NULL);
    break;
    default:
      DisplayTestPatern(NULL);
  }

  if (LedIntensity == -1) {
    lc.shutdown(0,true);
    lc.shutdown(1,true);
  }
  
  return true; // to repeat the action - false to stop
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Runs every 25ms, scan the KeyPad matrix for any new keypress
bool ReadKeypad(void *) {
  char KeyPress = keypad.getKey();

  if (KeyPress){
    Serial.print("KEYPRESS:");
    Serial.println(KeyPress);
    NewKeyPress = true; // Resetted by the fonction consuming the keypress
    KeyPressVal = KeyPress;
  }

  return true; // to repeat the action - false to stop
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Runs every 500ms, control blinking of the cursor
bool ToggleCursor(void *) {
  // Control blinking of the char under the cursor
  ToggleCurVal=!ToggleCurVal;

  return true; // to repeat the action - false to stop
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Runs every 100ms, convert analog Knob value to CurrKnobValue
bool ReadKnob(void *) {
  int ReadKnobValue = analogRead(KnobPin); // Raw value from sensor reading for knob position
  int NewKnobValue  = 0;

  //Serial.print("Knob value is: ");
  //Serial.println(ReadKnobValue);

  // Decode Knob position from voltage value ranges
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
    NewKnobValue = 0; // Short or Open circuit condition, but usually open when in between 2 contacts
  }

  // New val compared with previous val to "debounce" the knob
  if(NewKnobValue == CurrKnobValue) {
    if (DisplayMode != CurrKnobValue) {
      Serial.print("KNOB:");
      Serial.println(CurrKnobValue);
    }
    DisplayMode = CurrKnobValue;
  }
  CurrKnobValue = NewKnobValue;

  return true; // to repeat the action - false to stop
}