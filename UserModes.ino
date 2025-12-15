
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
  lc.setIntensity(UPPERLEDROW, LedIntensity);
  lc.setIntensity(LOWERLEDROW, LedIntensity);

  DisplayCurrentTime(LEDCOLNULL, false);
  DisplayCurrentCountDown(LEDCOLNULL, false);

  // React to a new Keypress
  if (NewKeyPress) {
    NewKeyPress = false; // Consume the Keypress
    if (LockMode < LM_DISPLAY) {
      switch (KeyPressVal) {
        case KEY_UP:
        case KEY_DOWN: // Set the DisplayMode
          if (CountDownDisplayMode == CDM_HHHMISSS)
            CountDownDisplayMode = CDM_HHHMI_SS;
          else
            CountDownDisplayMode = CDM_HHHMISSS;
          EEPROM.update(EEPROM_COUNTDOWNDISPLAYMODE, CountDownDisplayMode);
          break;
        case KEY_RIGHT:
        case KEY_LEFT: // No Action
          NewKeyPress = false;
          break;
        case KEY_CLEAR: // Stop and reset the CountDown to zero
          if (LockMode < LM_CHRONO) {
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
            MessageBuffer = "ACTION:COUNTDOWN-RESET";
            EEPROM.update(EEPROM_COUNTDOWNACTIVE, false);
          }
          break;
        case KEY_ENTER: // Start/Stop the CountDown
          if (LockMode < LM_CHRONO)
            IsCountDownActive = !IsCountDownActive;
            IsCountDownActive ? MessageBuffer = "ACTION:COUNTDOWN-START" : MessageBuffer = "ACTION:COUNTDOWN-STOP";
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
  lc.setIntensity(UPPERLEDROW, LedIntensity);
  lc.setIntensity(LOWERLEDROW, LedIntensity);

  int Lap = (ChronoLapMode%LAP_DELTA)-1;
  int LapOffset = Lap*SIZECHRONOLAP;

  if (ChronoLapMode == LAP_NONE)
    DisplayCurrentTime(LEDCOLNULL, false);
  else
    DisplayCurrentLap(NULL); 

  if (ChronoLapMode < LAP_DELTA)
    DisplayCurrentChrono(NULL);
  else
    DisplayDeltaNextLap(NULL);

  if (NewKeyPress) {
    if (LockMode < LM_DISPLAY) {
      NewKeyPress = false;
      switch (KeyPressVal) {
        case KEY_UP:
        case KEY_DOWN: // Set the DisplayMode
          if (ChronoLapMode == LAP_NONE) {  
            if (ChronoDisplayMode == CM_HHHMISSX)
              ChronoDisplayMode = CM_HHHMI_SS;
            else
              ChronoDisplayMode = CM_HHHMISSX;
            EEPROM.update(EEPROM_CHRONODISPLAYMODE, ChronoDisplayMode);
          } else {
            if (ChronoLapMode < LAP_DELTA)
              ChronoLapMode += LAP_DELTA;
            else
              ChronoLapMode -= LAP_DELTA;
            EEPROM.update(EEPROM_CHRONOLAPMODE, ChronoLapMode);
          }
          break;
        case KEY_LEFT: // Change the Lap being displayed, if zero, curreent Chrono
          if (ChronoLapMode < LAP_DELTA)
            ChronoLapMode = max(ChronoLapMode-1, LAP_NONE);
          else
            ChronoLapMode = max(ChronoLapMode-1, LAP_DELTA);
          EEPROM.update(EEPROM_CHRONOLAPMODE, ChronoLapMode);
          break;
        case KEY_RIGHT: // Change the Lap being displayed, if zero, curreent Chrono
          if (ChronoLapMode < LAP_DELTA)
            ChronoLapMode = min(ChronoLapMode+1, LAP_DELTA-1);
          else
            ChronoLapMode = min(ChronoLapMode+1, LAP_SIZEOF-1);
          EEPROM.update(EEPROM_CHRONOLAPMODE, ChronoLapMode);
          break;
        case KEY_CLEAR: // Stop and reset the Chrono to zero, if displaying a Lap, reset it instead
          if (LockMode < LM_CHRONO) {
            if (ChronoLapMode == LAP_NONE) {
              ChronoHour         = 0;
              ChronoMinute       = 0;
              ChronoSecond       = 0;
              ChronoTenthsSecond = 0;
              ChronoThSecondOffset = 0;
              MessageBuffer = "CHRONO:" + String(ChronoHour) + "-" +
                              String(ChronoMinute) + "-" + String(ChronoSecond) + "/" + 
                              String(ChronoThSecondOffset);
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
        case KEY_ENTER: // Start/Stop the Chrono, if displaying a Lap, record Chrono to it instead
          if (LockMode < LM_CHRONO) {
            if (ChronoLapMode == LAP_NONE) {
              IsChronoActive = !IsChronoActive;
              IsChronoActive ? MessageBuffer = "ACTION:CHRONO-START" : MessageBuffer = "ACTION:CHRONO-STOP";
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
          if (ChronoLapMode < LAP_DELTA)
            ChronoLapMode = KeyPressVal-'0';
          else
            ChronoLapMode = (KeyPressVal-'0') + LAP_DELTA;
          EEPROM.update(EEPROM_CHRONOLAPMODE, ChronoLapMode);
      }
    }
    
    // Bring Lap back inside boundary
    if (ChronoLapMode == LAP_DELTA || ChronoLapMode >= LAP_SIZEOF) {
      ChronoLapMode = LAP_NONE;
      EEPROM.update(EEPROM_CHRONOLAPMODE, ChronoLapMode);
    }
  }
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Control user interaction with Date
void UserChangeDate(void *) {
  lc.setIntensity(UPPERLEDROW, LedIntensity);
  lc.setIntensity(LOWERLEDROW, LedIntensity);

  DisplayCurrentDate(LEDCOLNULL, false);
  DisplayCurrentTime(LEDCOLNULL, false);

  // React to a new Keypress
  if (NewKeyPress) {
    NewKeyPress = false;  // Consume the Keypress
    if (LockMode < LM_DISPLAY) {
      switch (KeyPressVal) {
        case KEY_UP: // Set the DisplayMode
          DateDisplayMode = (enum DateDisplayModes)((int)DateDisplayMode + 1);
          if (DateDisplayMode >= DM_SIZEOF)
            DateDisplayMode = DM_YYYYMMDD;
          EEPROM.update(EEPROM_DATEDISPLAYMODE, DateDisplayMode);
          break;
        case KEY_DOWN: // Set the DisplayMode
          DateDisplayMode = (enum DateDisplayModes)((int)DateDisplayMode - 1);
          if (DateDisplayMode < DM_YYYYMMDD)
            DateDisplayMode = DM_SIZEOF-1;
          EEPROM.update(EEPROM_DATEDISPLAYMODE, DateDisplayMode);
          break;
        case KEY_RIGHT: // Remove DayLightSavingTime offset, if not too close to midnight (easier to manage)
          if (LockMode < LM_TIMEZONE) {
            if (!DSTMode && Hour < 23) {
              Hour += 1;
              DSTMode = true;
              TimeZoneOffsetHr--;
              
              EEPROM.update(EEPROM_HOUR+(Hour%2), Hour);
              EEPROM.update(EEPROM_DTSMODE, DSTMode?1:0);

              EEPROM.update(EEPROM_TIMEZONEOFFSETHR, TimeZoneOffsetHr);
            }
          }
          break;
        case KEY_LEFT:
        case KEY_CLEAR: // Toggle DayLightSavingTime offset, if not too close to midnight (easier to manage)
          if (LockMode < LM_TIMEZONE) {
            if (DSTMode && Hour > 0) {
              Hour -= 1;
              DSTMode = false;
              TimeZoneOffsetHr++;

              EEPROM.update(EEPROM_HOUR+(Hour%2), Hour);
              EEPROM.update(EEPROM_DTSMODE, DSTMode?1:0);

              EEPROM.update(EEPROM_TIMEZONEOFFSETHR, TimeZoneOffsetHr);
            }
          }
          break;
        case KEY_ENTER: // Clear the LockFault flag
          if (LockMode < LM_TIMEZONE) {
            LockFault = false;
            InitPower = false;
          }
          break;
        default: // Number Keys, set the DisplayMode
          DateDisplayMode = KeyPressVal - '0';
          if (DateDisplayMode >= DM_SIZEOF)
            DateDisplayMode = DM_YYYYMMDD;
          else if (DateDisplayMode < DM_YYYYMMDD)
            DateDisplayMode = DM_SIZEOF-1;
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
  lc.setIntensity(UPPERLEDROW, LedIntensity);
  lc.setIntensity(LOWERLEDROW, LedIntensity);

  // 
  if (TimeZoneOffsetHr == 12 && TimeZoneOffsetMi == 00)
    DisplayCurrentDate(LEDCOLNULL, false);
  else
    DisplayTZAltTime(NULL);
  DisplayCurrentTime(LEDCOLNULL, false);

  // React to a new Keypress
  if (NewKeyPress) {
    NewKeyPress = false; // Consume the Keypress
    if (LockMode < LM_DISPLAY) {
      switch (KeyPressVal) {
        case KEY_UP: // Set the DisplayMode
          TimeDisplayMode = (enum TimeDisplayModes)((int)TimeDisplayMode + 1);
          if (TimeDisplayMode >= TM_SIZEOF)
            TimeDisplayMode = TM_HHMISS_X;
          EEPROM.update(EEPROM_TIMEDISPLAYMODE, TimeDisplayMode);
          break;
        case KEY_DOWN: // Set the DisplayMode
          TimeDisplayMode = (enum TimeDisplayModes)((int)TimeDisplayMode - 1);
          if (TimeDisplayMode < TM_HHMISS_X)
            TimeDisplayMode = TM_SIZEOF-1;
          EEPROM.update(EEPROM_TIMEDISPLAYMODE, TimeDisplayMode);
          break;
        case KEY_RIGHT: // Increment the Alternate TimeZone offset
          if (LockMode < LM_TIMEZONE) { 
            if (TimeZoneOffsetMi == 0)
              TimeZoneOffsetMi = 30;
            else {
              TimeZoneOffsetMi = 0;
              TimeZoneOffsetHr++;
            }

            if (TimeZoneOffsetHr > 24)
              TimeZoneOffsetHr = 1;

            EEPROM.update(EEPROM_TIMEZONEOFFSETHR, TimeZoneOffsetHr);
            EEPROM.update(EEPROM_TIMEZONEOFFSETMI, TimeZoneOffsetMi);

            UpdateTZAlt(NULL); 
          }
          break;
        case KEY_LEFT: // Decrement the Alternate TimeZone offset
          if (LockMode < LM_TIMEZONE) {
            if (TimeZoneOffsetMi == 0) {
              TimeZoneOffsetMi = 30;
              TimeZoneOffsetHr--;
            } else
              TimeZoneOffsetMi = 0;

            if (TimeZoneOffsetHr < 0)
              TimeZoneOffsetHr = 23;

            EEPROM.update(EEPROM_TIMEZONEOFFSETHR, TimeZoneOffsetHr);
            EEPROM.update(EEPROM_TIMEZONEOFFSETMI, TimeZoneOffsetMi);

            UpdateTZAlt(NULL); 
          }
          break;
        case KEY_CLEAR: // Reset the Alternate TimeZone offset
          if (LockMode < LM_TIMEZONE) {
            TimeZoneOffsetHr = 12;
            TimeZoneOffsetMi = 00;
            EEPROM.update(EEPROM_TIMEZONEOFFSETHR, TimeZoneOffsetHr);
            EEPROM.update(EEPROM_TIMEZONEOFFSETMI, TimeZoneOffsetMi);
            MinuteTZAlt = Minute;
            HourTZAlt   = Hour;
          }
          break;
        case KEY_ENTER: // Clear the LockFault flag
          if (LockMode < LM_TIMEZONE) {
            LockFault = false;
            InitPower = false;
          }
          break;
        default: // Number Keys, set the DisplayMode
          TimeDisplayMode = KeyPressVal - '0';
          if (TimeDisplayMode >= TM_SIZEOF)
            TimeDisplayMode = TM_HHMISS_X;
          else if (TimeDisplayMode < TM_HHMISS_X)
            TimeDisplayMode = TM_SIZEOF-1;
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
  DisplayCurrentTime(LEDCOLNULL, false);

  //Inline text display on the upper row
  lc.setChar(UPPERLEDROW, LEDCOL0, 'L', false);
  lc.setChar(UPPERLEDROW, LEDCOL1, 'E', false);
  lc.setRow(UPPERLEDROW,  LEDCOL2, SpecChar[CHR_V]);
  lc.setChar(UPPERLEDROW, LEDCOL3, 'E', false);
  lc.setChar(UPPERLEDROW, LEDCOL4, 'L', false);
  lc.setChar(UPPERLEDROW, LEDCOL5, ' ', false);

  // React to a new Keypress
  if (NewKeyPress) {
    NewKeyPress = false; // Consume the Keypress

    switch (KeyPressVal) {
      case KEY_UP: // Increment the brightness, capped at 0xF
        lc.shutdown(UPPERLEDROW, false);
        lc.shutdown(LOWERLEDROW, false);
        LedIntensity = min(LedIntensity+1, LEDMAXINTENSITY);
        break;
      case KEY_RIGHT: // Set the brightness at 0xD
        lc.shutdown(UPPERLEDROW, false);
        lc.shutdown(LOWERLEDROW, false);
        LedIntensity = 13;
        break;
      case KEY_DOWN: // Decrement the brightness, floored at 0x0
        lc.shutdown(UPPERLEDROW, false);
        lc.shutdown(LOWERLEDROW, false);
        LedIntensity = max(LedIntensity-1, 0x0);
        break;
      case KEY_LEFT: // Set the brightness at 0xB
        lc.shutdown(UPPERLEDROW, false);
        lc.shutdown(LOWERLEDROW, false);
        LedIntensity = 11;
        break;
      case KEY_CLEAR: // Set the brightness bellow 0x0, so completely off
        LedIntensity = LEDOFFINTENSITY;
        lc.shutdown(UPPERLEDROW, true);
        lc.shutdown(LOWERLEDROW, true);
        break;
      case KEY_ENTER: // Set the brightness at 0xF
        lc.shutdown(UPPERLEDROW, false);
        lc.shutdown(LOWERLEDROW, false);
        LedIntensity = LEDMAXINTENSITY;
        break;
      default: // Number Keys, Set the brightness at the coresponding value
        lc.shutdown(UPPERLEDROW, false);
        lc.shutdown(LOWERLEDROW, false);
        LedIntensity = KeyPressVal - '0';
    }
    MessageBuffer = "ACTION:DIM-" + String(LedIntensity);  
    EEPROM.update(EEPROM_LEDINTENSITY, LedIntensity);
  }

  // Convert single Int to "Char like" for brightness with 2 positions
  int display_acc = LedIntensity;
  int display_step;

  display_step = display_acc-display_acc%10;
  lc.setDigit(UPPERLEDROW, LEDCOL6, (display_step/10), false);
  display_acc = display_acc-display_step;

  lc.setDigit(UPPERLEDROW, LEDCOL7, display_acc, false);

  lc.setIntensity(UPPERLEDROW, LedIntensity);
  lc.setIntensity(LOWERLEDROW, LedIntensity);
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
  TimeZoneOffsetHr = EEPROM.read(EEPROM_TIMEZONEOFFSETHR);
  TimeZoneOffsetMi = EEPROM.read(EEPROM_TIMEZONEOFFSETMI);
  // Drop to Default if out of bounds
  if (TimeZoneOffsetHr > 24 || TimeZoneOffsetHr < 0 
       || !(TimeZoneOffsetMi == 0 || TimeZoneOffsetMi == 30 ) ) {
    TimeZoneOffsetHr = 12;
    TimeZoneOffsetMi = 0;
    EEPROM.update(EEPROM_TIMEZONEOFFSETHR, 12);
    EEPROM.update(EEPROM_TIMEZONEOFFSETMI, 00);
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
  for(int i=0; i<NBCHRONOLAP; i++) { 
    LapOffset = i*SIZECHRONOLAP; 
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
  if (LedIntensity > LEDMAXINTENSITY) {
    LedIntensity = LEDDEFAULTINTENSITY;
    EEPROM.update(EEPROM_LEDINTENSITY, LEDDEFAULTINTENSITY);
  }
  DateDisplayMode      = EEPROM.read(EEPROM_DATEDISPLAYMODE);
  if (DateDisplayMode >= DM_SIZEOF) {
    DateDisplayMode = DM_YYYYMMDD;
    EEPROM.update(EEPROM_DATEDISPLAYMODE, DM_YYYYMMDD);
  }
  TimeDisplayMode      = EEPROM.read(EEPROM_TIMEDISPLAYMODE);
  if (TimeDisplayMode >= TM_SIZEOF) {
    TimeDisplayMode = TM_HHMISS_X;
    EEPROM.update(EEPROM_TIMEDISPLAYMODE, TimeDisplayMode);
  }
  ChronoDisplayMode    = EEPROM.read(EEPROM_CHRONODISPLAYMODE);
  if (ChronoDisplayMode >= CM_SIZEOF) {
    ChronoDisplayMode = CM_HHHMISSX;
    EEPROM.update(EEPROM_CHRONODISPLAYMODE, ChronoDisplayMode);
  }
  ChronoLapMode        = EEPROM.read(EEPROM_CHRONOLAPMODE);
  if (ChronoLapMode >= LAP_SIZEOF || ChronoLapMode == LAP_DELTA) {
    ChronoLapMode = LAP_NONE;
    EEPROM.update(EEPROM_CHRONOLAPMODE, ChronoLapMode);
  }
  CountDownDisplayMode = EEPROM.read(EEPROM_COUNTDOWNDISPLAYMODE);
  if (CountDownDisplayMode >= CDM_SIZEOF) {
    CountDownDisplayMode = CDM_HHHMISSS;
    EEPROM.update(EEPROM_COUNTDOWNDISPLAYMODE, CountDownDisplayMode);
  }

  //Lock fonctionality
  LockMode    = EEPROM.read(EEPROM_LOCKMODE);
  LockAttempt = EEPROM.read(EEPROM_LOCKATTEMPT);
  if (LockMode >= LM_SIZEOF || LockMode < LM_NONE) { // If smaller then 0 or bigger then 6, it can't be valid
    LockMode   = LM_NONE;
    LockTemper = false;
    EEPROM.update(EEPROM_LOCKMODE, 0);
    EEPROM.update(EEPROM_LOCKATTEMPT, 0);
  }
  LockPassCodeLength = EEPROM.read(EEPROM_LOCKPASSCODELENGTH);
  if (LockPassCodeLength > MAXPWDLENGTH) { // If longer than 16, it can't be valid
    LockPassCodeLength = 0;
    EEPROM.update(EEPROM_LOCKPASSCODELENGTH, 0);
  }
  for (int i=0; i<LockPassCodeLength; i++)
    LockPassCode[i] = EEPROM.read(EEPROM_LOCKPASSCODE + i);
  LockTemper = LockAttempt >= MAXPWDATTEMPT;
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

