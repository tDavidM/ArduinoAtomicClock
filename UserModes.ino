
// Contains the rountines used to interact with Date/Time/Chrono/Countdown and Dim values using the Keypad
// The refresh rate is about 50ms and mostly controled by the MainClock routine UpdateDisplay()
//   calling routines here in UserModes
// Routine in DisplayModes are then used to output the current state

// Control user interaction with CountDown
//void UserGetCountDown(void *)

// Control user interaction with Chrono
//void UserGetChrono(void *)

// Control user interaction with Date
//void UserChangeDate(void *)

// Control user interaction with Time
//void UserChangeTime(void *)

// Change the brightness of the LED segment display
//void UserChangeDim(void *)

// Get all the values from EEPROM at startup, use only if Addr 1023 (EEPROM_TESTVAL) == 22
//void GetEEPROMVal(void *)

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Control user interaction with CountDown
void UserGetCountDown(void *) {
  lc.setIntensity(0,LedIntensity);
  lc.setIntensity(1,LedIntensity);

  DisplayCurrentTime(-1, false);
  DisplayCurrentCountDown(-1, false);

  // React to a new Keypress
  if (NewKeyPress) {
    NewKeyPress = false; // Consume the Keypress
    if (LockMode < 5) {
      switch (KeyPressVal) {
        case 'U':
        case 'D': // Set the DisplayMode
          if (CountDownDisplayMode == 0)
            CountDownDisplayMode = 1;
          else
            CountDownDisplayMode = 0;
          EEPROM.update(EEPROM_COUNTDOWNDISPLAYMODE, CountDownDisplayMode);
          break;
        case 'R':
        case 'L': // No Action
          NewKeyPress = false;
          break;
        case 'C': // Stop and reset the CountDown to zero
          if (LockMode < 4) {
            CountDownHour   = 0;
            CountDownMinute = 0;
            CountDownSecond = 0;
            EEPROM.update(EEPROM_COUNTDOWNHOUR, 0);
            EEPROM.update(EEPROM_COUNTDOWNHOUR+1, 0);
            EEPROM.update(EEPROM_COUNTDOWNMINUTE, 0);
            EEPROM.update(EEPROM_COUNTDOWNSECOND, 0);
            EEPROM.update(EEPROM_COUNTDOWNSECONDOVER, 0);
            IsCountDownActive = false;
            IsCountDownDone = false;
            EEPROM.update(EEPROM_COUNTDOWNACTIVE, false);
          }
          break;
        case 'E': // Start/Stop the CountDown
          if (LockMode < 4)
            IsCountDownActive = !IsCountDownActive;
            EEPROM.update(EEPROM_COUNTDOWNACTIVE, IsCountDownActive);
          break;
        default: // Number Keys, No Action
          NewKeyPress = false;
      }
    }
  }

}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Control user interaction with Chrono
void UserGetChrono(void *) {
  lc.setIntensity(0,LedIntensity);
  lc.setIntensity(1,LedIntensity);

  int Lap = (ChronoLapMode%10)-1;
  int LapOffset = Lap*5;

  if (ChronoLapMode == 0)
    DisplayCurrentTime(-1, false);
  else
    DisplayCurrentLap(NULL); 

  if (ChronoLapMode < 10)
    DisplayCurrentChrono(NULL);
  else
    DisplayDeltaNextLap(NULL);

  if (NewKeyPress) {
    if (LockMode < 5) {
      NewKeyPress = false;
      switch (KeyPressVal) {
        case 'U':
        case 'D': // Set the DisplayMode
          if (ChronoLapMode == 0) {  
            if (ChronoDisplayMode == 0)
              ChronoDisplayMode = 1;
            else
              ChronoDisplayMode = 0;
            EEPROM.update(EEPROM_CHRONODISPLAYMODE, ChronoDisplayMode);
          } else {
            if (ChronoLapMode < 10)
              ChronoLapMode += 10;
            else
              ChronoLapMode -= 10;
            EEPROM.update(EEPROM_CHRONOLAPMODE, ChronoLapMode);
          }
          break;
        case 'L': // Change the Lap being displayed, if zero, curreent Chrono
          if (ChronoLapMode < 10)
            ChronoLapMode = max(ChronoLapMode-1, 0);
          else
            ChronoLapMode = max(ChronoLapMode-1, 10);
          EEPROM.update(EEPROM_CHRONOLAPMODE, ChronoLapMode);
          break;
        case 'R': // Change the Lap being displayed, if zero, curreent Chrono
          if (ChronoLapMode < 10)
            ChronoLapMode = min(ChronoLapMode+1, 9);
          else
            ChronoLapMode = min(ChronoLapMode+1, 19);
          EEPROM.update(EEPROM_CHRONOLAPMODE, ChronoLapMode);
          break;
        case 'C': // Stop and reset the Chrono to zero, if displaying a Lap, reset it instead
          if (LockMode < 4) {
            if (ChronoLapMode == 0) {
              ChronoHour         = 0;
              ChronoMinute       = 0;
              ChronoSecond       = 0;
              ChronoTenthsSecond = 0;
              ChronoThSecondOffset = 0;
            } else {
              ChronoLap[Lap].Hour         = 0;
              ChronoLap[Lap].Minute       = 0;
              ChronoLap[Lap].Second       = 0;
              ChronoLap[Lap].TenthsSecond = 0;
              EEPROM.update(EEPROM_CHRONOLAPLIST+LapOffset,   0);
              EEPROM.update(EEPROM_CHRONOLAPLIST+LapOffset+1, 0);
              EEPROM.update(EEPROM_CHRONOLAPLIST+LapOffset+2, 0);
              EEPROM.update(EEPROM_CHRONOLAPLIST+LapOffset+3, 0);
              EEPROM.update(EEPROM_CHRONOLAPLIST+LapOffset+4, 0);
            }
          }
          break;
        case 'E': // Start/Stop the Chrono, if displaying a Lap, record Chrono to it instead
          if (LockMode < 4) {
            if (ChronoLapMode == 0) {
              IsChronoActive = !IsChronoActive;
              EEPROM.update(EEPROM_CHRONOACTIVE, IsChronoActive);
              if (IsChronoActive)
                ChronoThSecondOffset = TenthsSecond;
            } else {
              ChronoLap[Lap].Hour         = ChronoHour;
              ChronoLap[Lap].Minute       = ChronoMinute;
              ChronoLap[Lap].Second       = ChronoSecond;
              ChronoLap[Lap].TenthsSecond = ChronoTenthsSecond;
              EEPROM.update(EEPROM_CHRONOLAPLIST+LapOffset,   ChronoHour/100);
              EEPROM.update(EEPROM_CHRONOLAPLIST+LapOffset+1, ChronoHour%100);
              EEPROM.update(EEPROM_CHRONOLAPLIST+LapOffset+2, ChronoMinute);
              EEPROM.update(EEPROM_CHRONOLAPLIST+LapOffset+3, ChronoSecond);
              EEPROM.update(EEPROM_CHRONOLAPLIST+LapOffset+4, ChronoTenthsSecond);
            }
          }
          break;
        default: // Number Keys, change to the appropritate Lap, Zero brings back Chrono
          if (ChronoLapMode < 10)
            ChronoLapMode = KeyPressVal-'0';
          else
            ChronoLapMode = (KeyPressVal-'0') + 10;
          EEPROM.update(EEPROM_CHRONOLAPMODE, ChronoLapMode);
      }
    }
    
    // Bring Lap back inside boundary
    if (ChronoLapMode == 10 || ChronoLapMode == 20) {
      ChronoLapMode = 0;
      EEPROM.update(EEPROM_CHRONOLAPMODE, ChronoLapMode);
    }
  }
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Control user interaction with Date
void UserChangeDate(void *) {
  lc.setIntensity(0,LedIntensity);
  lc.setIntensity(1,LedIntensity);

  DisplayCurrentDate(-1, false);
  DisplayCurrentTime(-1, false);

  // React to a new Keypress
  if (NewKeyPress) {
    NewKeyPress = false;  // Consume the Keypress
    if (LockMode < 5) {
      switch (KeyPressVal) {
        case 'U': // Set the DisplayMode
          DateDisplayMode++;
          if (DateDisplayMode > 5)
            DateDisplayMode = 0;
          EEPROM.update(EEPROM_DATEDISPLAYMODE, DateDisplayMode);
          break;
        case 'D': // Set the DisplayMode
          DateDisplayMode--;
          if (DateDisplayMode < 0)
            DateDisplayMode = 5;
          EEPROM.update(EEPROM_DATEDISPLAYMODE, DateDisplayMode);
          break;
        case 'R': // Remove DayLightSavingTime offset, if not too close to midnight (easier to manage)
          if (LockMode < 3) {
            if (!DSTMode && Hour < 23) {
              Hour += 1;
              DSTMode = true;
              EEPROM.update(EEPROM_HOUR+(Hour%2), Hour);
              EEPROM.update(EEPROM_DTSMODE, DSTMode?1:0);
            }
          }
          break;
        case 'L':
        case 'C': // Toggle DayLightSavingTime offset, if not too close to midnight (easier to manage)
          if (LockMode < 3) {
            if (DSTMode && Hour > 0) {
              Hour -= 1;
              DSTMode = false;
              EEPROM.update(EEPROM_HOUR+(Hour%2), Hour);
              EEPROM.update(EEPROM_DTSMODE, DSTMode?1:0);
            }
          }
          break;
        case 'E': // Shortcut to most visualy usefull Date format
          DateDisplayMode = 1;
          EEPROM.update(EEPROM_DATEDISPLAYMODE, DateDisplayMode);
          break;
        default: // Number Keys, set the DisplayMode
          DateDisplayMode = KeyPressVal - '0';
          if (DateDisplayMode > 5)
            DateDisplayMode = 0;
          else if (DateDisplayMode < 0)
            DateDisplayMode = 5;
          EEPROM.update(EEPROM_DATEDISPLAYMODE, DateDisplayMode);
      }
    }
  }
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Control user interaction with Time
void UserChangeTime(void *) {
  lc.setIntensity(0,LedIntensity);
  lc.setIntensity(1,LedIntensity);

  // 
  if (TimeZoneOffsetHr + TimeZoneOffsetMi == 0)
    DisplayCurrentDate(-1, false);
  else
    DisplayTZAltTime(NULL);
  DisplayCurrentTime(-1, false);

  // React to a new Keypress
  if (NewKeyPress) {
    NewKeyPress = false; // Consume the Keypress
    if (LockMode < 5) {
      switch (KeyPressVal) {
        case 'U': // Set the DisplayMode
          TimeDisplayMode++;
          if (TimeDisplayMode > 5)
            TimeDisplayMode = 0;
          EEPROM.update(EEPROM_TIMEDISPLAYMODE, TimeDisplayMode);
          break;
        case 'D': // Set the DisplayMode
          TimeDisplayMode--;
          if (TimeDisplayMode < 0)
            TimeDisplayMode = 5;
          EEPROM.update(EEPROM_TIMEDISPLAYMODE, TimeDisplayMode);
          break;
        case 'R': // Increment the Alternate TimeZone offset
          if (LockMode < 3) { 
            if (TimeZoneOffsetHr > 0) {
              if (TimeZoneOffsetMi == 30 ) {
                TimeZoneOffsetMi = 0;
                TimeZoneOffsetHr++;
              } if (TimeZoneOffsetMi == -30 ) 
                TimeZoneOffsetMi = 0;
              else
                TimeZoneOffsetMi = 30;
            } else if (TimeZoneOffsetHr == 0) {
              if (TimeZoneOffsetMi == 30 ) {
                TimeZoneOffsetMi = 0;
                TimeZoneOffsetHr++;
              }
              else if (TimeZoneOffsetMi == 0 )
                TimeZoneOffsetMi = 30;
              else 
                TimeZoneOffsetMi = 0;
            } else {
              if (TimeZoneOffsetMi == -30 )
                TimeZoneOffsetMi = 0;
              else {
                TimeZoneOffsetMi = -30;
                TimeZoneOffsetHr++;
              }
            }

            if (TimeZoneOffsetHr >= 13)
              TimeZoneOffsetHr = -11;

            EEPROM.update(EEPROM_TIMEZONEOFFSETHR, TimeZoneOffsetHr+12);
            EEPROM.update(EEPROM_TIMEZONEOFFSETMI, TimeZoneOffsetMi+30);

            UpdateTZAlt(NULL); 
         }
          break;
        case 'L': // Decrement the Alternate TimeZone offset
          if (LockMode < 3) {
            if (TimeZoneOffsetHr > 0) {
              if (TimeZoneOffsetMi == 30 )
                TimeZoneOffsetMi = 0;
              else {
                TimeZoneOffsetMi = 30;
                TimeZoneOffsetHr--;
              }
            } else if (TimeZoneOffsetHr == 0) {
              if (TimeZoneOffsetMi == 30 )
                TimeZoneOffsetMi = 0;
              else if (TimeZoneOffsetMi == 0 )
                TimeZoneOffsetMi = -30;
              else {
                TimeZoneOffsetMi = 0;
                TimeZoneOffsetHr--;
              }
            } else {
              if (TimeZoneOffsetMi == -30 ) {
                TimeZoneOffsetMi = 0;
                TimeZoneOffsetHr--;
              } else
                TimeZoneOffsetMi = -30;
            }

            if (TimeZoneOffsetHr <= -12)
              TimeZoneOffsetHr = 12;

            EEPROM.update(EEPROM_TIMEZONEOFFSETHR, TimeZoneOffsetHr+12);
            EEPROM.update(EEPROM_TIMEZONEOFFSETMI, TimeZoneOffsetMi+30);

            UpdateTZAlt(NULL); 
          }
          break;
        case 'C': // Reset the Alternate TimeZone offset
          if (LockMode < 3) {
            TimeZoneOffsetHr = 0;
            TimeZoneOffsetMi = 0;
            EEPROM.update(EEPROM_TIMEZONEOFFSETHR, TimeZoneOffsetHr+12);
            EEPROM.update(EEPROM_TIMEZONEOFFSETMI, TimeZoneOffsetMi+30);
            MinuteTZAlt = Minute;
            HourTZAlt   = Hour;
          }
          break;
        case 'E': // Shortcut to most visualy usefull Time format
          TimeDisplayMode = 2;
          EEPROM.update(EEPROM_TIMEDISPLAYMODE, TimeDisplayMode);
          break;
        default: // Number Keys, set the DisplayMode
          TimeDisplayMode = KeyPressVal - '0';
          if (TimeDisplayMode > 5)
            TimeDisplayMode = 0;
          else if (TimeDisplayMode < 0)
            TimeDisplayMode = 5;
          EEPROM.update(EEPROM_TIMEDISPLAYMODE, TimeDisplayMode);
      }
    }
  }
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Change the brightness of the LED segment display
void UserChangeDim(void *) {
  DisplayCurrentTime(-1, false);

  //Inline text display on the upper row
  lc.setChar(0,7,'L',false);
  lc.setChar(0,6,'E',false);
  //lc.setChar(0,5,'V',false);
  lc.setRow(0,5,SpecChar[CH_V]);
  lc.setChar(0,4,'E',false);
  lc.setChar(0,3,'L',false);
  lc.setChar(0,2,' ',false);

  // React to a new Keypress
  if (NewKeyPress) {
    NewKeyPress = false; // Consume the Keypress

    switch (KeyPressVal) {
      case 'U': // Increment the brightness, capped at 0xF
        lc.shutdown(0,false);
        lc.shutdown(1,false);
        LedIntensity = min(LedIntensity+1,0xf);
        break;
      case 'R': // Set the brightness at 0xD
        lc.shutdown(0,false);
        lc.shutdown(1,false);
        LedIntensity = 13;
        break;
      case 'D': // Decrement the brightness, floored at 0x0
        lc.shutdown(0,false);
        lc.shutdown(1,false);
        LedIntensity = max(LedIntensity-1,0x0);
        break;
      case 'L': // Set the brightness at 0xB
        lc.shutdown(0,false);
        lc.shutdown(1,false);
        LedIntensity = 11;
        break;
      case 'C': // Set the brightness bellow 0x0, so completely off
        LedIntensity = -1;
        lc.shutdown(0,true);
        lc.shutdown(1,true);
        break;
      case 'E': // Set the brightness at 0xF
        lc.shutdown(0,false);
        lc.shutdown(1,false);
        LedIntensity = 0xf;
        break;
      default: // Number Keys, Set the brightness at the coresponding value
        lc.shutdown(0,false);
        lc.shutdown(1,false);
        LedIntensity = KeyPressVal - '0';
    }
    EEPROM.update(EEPROM_LEDINTENSITY, LedIntensity);
  }

  // Convert single Int to "Char like" for brightness with 2 positions
  int display_acc = LedIntensity;
  int display_step;

  display_step = display_acc-display_acc%10;
  lc.setDigit(0,1,(display_step/10),false);
  display_acc = display_acc-display_step;

  lc.setDigit(0,0,display_acc,false);

  lc.setIntensity(0,LedIntensity);
  lc.setIntensity(1,LedIntensity);
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Get all the values from EEPROM at startup, use only if Addr 1023 (EEPROM_TESTVAL) == 22
void GetEEPROMVal(void *) {
  int HourAlt1, HourAlt2, IsActive, LapOffset;

  // Date values
  Year  = (EEPROM.read(EEPROM_YEAR)*100) + EEPROM.read(EEPROM_YEAR + 1); // Split over 2 bytes
  Month = EEPROM.read(EEPROM_MONTH);
  Day   = EEPROM.read(EEPROM_DAY);
  // Drop to Default if out of bounds
  if (Year > 9999 || Month > 59 || Day > 31) {
    Year  = 2020;
    Month = 1;
    Day   = 1;
  }
  
  // Time values
  // Hour is spread over 2 bytes for wear leveling
  HourAlt1 = EEPROM.read(EEPROM_HOUR);
  HourAlt2 = EEPROM.read(EEPROM_HOUR + 1);
  // Alternate between the 2, so bigger is latest except when rolling back
  if (HourAlt1 == 0 && HourAlt2 == 23)
    Hour = 0;
  else
    Hour = max(HourAlt1, HourAlt2);

  Minute = EEPROM.read(EEPROM_MINUTE + (((Day%4)*(Hour+1))+Hour)); // Spread writes over 96 bytes for wear leveling
  Second = EEPROM.read(EEPROM_SECOND + (((Hour%12)*(Minute+1))+Minute)); // Spread writes over 720 bytes for wear leveling
  // Drop to Default if out of bounds
  if (Hour > 23 || Minute > 59 || Second > 59) {
    Hour   = 0;
    Minute = 0;
    Second = 0;
  }

  //Alternate TimeZone
  TimeZoneOffsetHr = EEPROM.read(EEPROM_TIMEZONEOFFSETHR)-12;
  TimeZoneOffsetMi = EEPROM.read(EEPROM_TIMEZONEOFFSETMI)-30;
  // Drop to Default if out of bounds
  if (TimeZoneOffsetHr > 12 || TimeZoneOffsetHr < -11 
       || !(TimeZoneOffsetMi == 0 || TimeZoneOffsetMi == 30 || TimeZoneOffsetMi == -30) ) {
    TimeZoneOffsetHr = 0;
    TimeZoneOffsetMi = 0;
    EEPROM.update(EEPROM_TIMEZONEOFFSETHR, 0);
    EEPROM.update(EEPROM_TIMEZONEOFFSETMI, 0);
  }
  UpdateTZAlt(NULL);

  // DayLight SavingTime
  IsActive = EEPROM.read(EEPROM_DTSMODE);
  DSTMode  = IsActive==1; // Anything > 1 is invalid
  EEPROM.update(EEPROM_DTSMODE, DSTMode?1:0);

  // Chrono
  IsActive       = EEPROM.read(EEPROM_CHRONOACTIVE);
  IsChronoActive = IsActive==1; // Anything > 1 is invalid
  EEPROM.update(EEPROM_CHRONOACTIVE, IsChronoActive?1:0);
  ChronoHour     = (EEPROM.read(EEPROM_CHRONOHOUR)*100) + EEPROM.read(EEPROM_CHRONOHOUR + 1); // Split over 2 bytes
  ChronoMinute   = EEPROM.read(EEPROM_CHRONOMINUTE + (ChronoHour%10)); // Spread over 10 bytes for wear leveling
  ChronoSecond   = EEPROM.read(EEPROM_CHRONOSECOND + ((ChronoHour%2)*20)+(ChronoMinute%20)); // Spread over 40 bytes for wear leveling
  if (ChronoHour > 999 || ChronoMinute > 59 || ChronoSecond > 59) {
    ChronoHour   = 0;
    ChronoMinute = 0;
    ChronoSecond = 0;
    EEPROM.update(EEPROM_CHRONOHOUR, 0); 
    EEPROM.update(EEPROM_CHRONOHOUR+1, 0); 
    EEPROM.update(EEPROM_CHRONOMINUTE, 0); // CrHour%10
    EEPROM.update(EEPROM_CHRONOSECOND, 0); // ((CrHour%2)*20)+(CrMinute%20)
  }

  // Laps for Chrono
  for(int i=0; i<9; i++) { 
    LapOffset = i*5; 
    ChronoLap[i].Hour         = (EEPROM.read(EEPROM_CHRONOLAPLIST + LapOffset)*100) + EEPROM.read(EEPROM_CHRONOLAPLIST + LapOffset+1); // Split over 2 bytes 
    ChronoLap[i].Minute       = EEPROM.read( EEPROM_CHRONOLAPLIST + LapOffset+2); 
    ChronoLap[i].Second       = EEPROM.read( EEPROM_CHRONOLAPLIST + LapOffset+3); 
    ChronoLap[i].TenthsSecond = EEPROM.read( EEPROM_CHRONOLAPLIST + LapOffset+4);
    if (ChronoLap[i].Hour > 999 || ChronoLap[i].Minute > 59 || ChronoLap[i].Second > 59 || ChronoLap[i].TenthsSecond > 9) {
      ChronoLap[i].Hour         = 0;
      ChronoLap[i].Minute       = 0;
      ChronoLap[i].Second       = 0;
      ChronoLap[i].TenthsSecond = 0;
      EEPROM.update(EEPROM_CHRONOLAPLIST + LapOffset, 0);
      EEPROM.update(EEPROM_CHRONOLAPLIST + LapOffset+1, 0);
      EEPROM.update(EEPROM_CHRONOLAPLIST + LapOffset+2, 0);
      EEPROM.update(EEPROM_CHRONOLAPLIST + LapOffset+3, 0);
      EEPROM.update(EEPROM_CHRONOLAPLIST + LapOffset+4, 0);
    }
  }

  // CountDown
  IsActive          = EEPROM.read(EEPROM_COUNTDOWNACTIVE);
  IsCountDownActive = IsActive==1; // Anything > 1 is invalid
  CountDownHour     = (EEPROM.read(EEPROM_COUNTDOWNHOUR)*100) + EEPROM.read(EEPROM_COUNTDOWNHOUR+1); // Split over 2 bytes 
  CountDownMinute   = EEPROM.read(EEPROM_COUNTDOWNMINUTE+(CountDownHour%10)); // Spread over 10 bytes for wear leveling
  CountDownSecond   = (EEPROM.read(EEPROM_COUNTDOWNSECONDOVER)*100)                                      // Split over 2 bytes
                      + EEPROM.read(EEPROM_COUNTDOWNSECOND+((CountDownHour%2)*20)+(CountDownMinute%20)); // Lower bit is spread over 40 bytes for wear leveling
  if (CountDownHour > 999 || CountDownMinute > 99 || CountDownSecond > 999) {
    CountDownHour   = 0;
    CountDownMinute = 0;
    CountDownSecond = 0;
    EEPROM.update(EEPROM_COUNTDOWNHOUR, 0);
    EEPROM.update(EEPROM_COUNTDOWNHOUR+1, 0);
    EEPROM.update(EEPROM_COUNTDOWNMINUTE, 0); // CdHour%10
    EEPROM.update(EEPROM_COUNTDOWNSECOND, 0); // ((CdHour%2)*20)+(CdMinute%20)
    EEPROM.update(EEPROM_COUNTDOWNSECONDOVER, 0);
  }
  // If everything is Zero, no need to start the timer
  if (CountDownHour == 0 && CountDownMinute == 0 && CountDownSecond == 0)
    IsCountDownActive = false;
  EEPROM.update(EEPROM_COUNTDOWNACTIVE, IsCountDownActive?1:0);

  // Misc display control vars
  LedIntensity         = EEPROM.read(EEPROM_LEDINTENSITY);
  if (LedIntensity > 15) {
    LedIntensity = 8;
    EEPROM.update(EEPROM_LEDINTENSITY, 8);
  }
  DateDisplayMode      = EEPROM.read(EEPROM_DATEDISPLAYMODE);
  if (DateDisplayMode > 5) {
    DateDisplayMode = 0;
    EEPROM.update(EEPROM_DATEDISPLAYMODE, 0);
  }
  TimeDisplayMode      = EEPROM.read(EEPROM_TIMEDISPLAYMODE);
  if (TimeDisplayMode > 5) {
    TimeDisplayMode = 0;
    EEPROM.update(EEPROM_TIMEDISPLAYMODE, 0);
  }
  ChronoDisplayMode    = EEPROM.read(EEPROM_CHRONODISPLAYMODE);
  if (ChronoDisplayMode > 1) {
    ChronoDisplayMode = 0;
    EEPROM.update(EEPROM_CHRONODISPLAYMODE, 0);
  }
  ChronoLapMode        = EEPROM.read(EEPROM_CHRONOLAPMODE);
  if (ChronoLapMode > 19 || ChronoLapMode == 10) {
    ChronoLapMode = 0;
    EEPROM.update(EEPROM_CHRONOLAPMODE, 0);
  }
  CountDownDisplayMode = EEPROM.read(EEPROM_COUNTDOWNDISPLAYMODE);
  if (CountDownDisplayMode > 1) {
    CountDownDisplayMode = 0;
    EEPROM.update(EEPROM_COUNTDOWNDISPLAYMODE, 0);
  }

  //Lock fonctionality
  LockMode           = EEPROM.read(EEPROM_LOCKMODE);
  if (LockMode > 6) { // If bigger than 6, it can't be valid
    LockMode = 0;
    EEPROM.update(EEPROM_LOCKMODE, 0);
  }
  LockPassCodeLength = EEPROM.read(EEPROM_LOCKPASSCODELENGTH);
  if (LockPassCodeLength > 16) { // If longer than 16, it can't be valid
    LockPassCodeLength = 0;
    EEPROM.update(EEPROM_LOCKPASSCODELENGTH, 0);
  }
  for (int i=0; i<LockPassCodeLength; i++)
    LockPassCode[i] = EEPROM.read(EEPROM_LOCKPASSCODE + i);
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

