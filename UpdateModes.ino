// Contains the rountines used to update Date/Time/Countdown and LockMode values using the Keypad
// The refresh rate is about 50ms and mostly controled by the MainClock routine UpdateDisplay()
//   calling routines here in UpdateModes
// Routine in DisplayModes are then used to output the current state

// Used to set the Year/Month/Day
//void UpdateSetDate(void *)

// Used to set the Hour/Minute/Second
//void UpdateSetTime(void *)

// When a number is pressed on the keyboard, this changes the number in the Date under the cursor
//void CommitDateChange(int CursorPos, int NewVal)

// When a number is pressed on the keyboard, this changes the number in the Time under the cursor 
//void CommitTimeChange(int CursorPos, int NewVal)

// Keep AlternateTimeZone up to date when Time is changed or if TimeZone edited
//void UpdateTZAlt(void *)

// Used to set the CountDown value
//void UpdateSetCountDown(void *)

// When a number is pressed on the keyboard, this changes the number in the CountDown under the cursor 
//void CommitCountDownChange(int CursorPos, int NewVal)

// Set the LockMode used to prevent modifications, see list bellow
//void UpdateSetLock(void *)
// 0 = None
// 1 = Lock PassCode Change
// 2 = Date/Hour/Second Change
// 3 = TimeZone/DST Change
// 4 = Countdown/Chrono/Laps Change/Set
// 5 = Display Format Set
// 6 = Dim Set

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Used to set the Year/Month/Day
void UpdateSetDate(void *) {
  lc.setIntensity(UPPERLEDROW, LedIntensity);
  lc.setIntensity(LOWERLEDROW, LedIntensity);

  lc.clearDisplay(LOWERLEDROW);

  // React to a new Keypress
  if (NewKeyPress) {
    NewKeyPress = false; // Consume the Keypress
    switch (KeyPressVal) {
      case KEY_UP: // Increment the number under the cursor, capped at "9"
        ToggleCurChar = min(ToggleCurChar+1, '9');
        CommitDateChange(CursorPos,ToggleCurChar-'0');
        break;
      case KEY_DOWN: // Decrement the number under the cursor, floored at "0"
        ToggleCurChar = max(ToggleCurChar-1, '0');
        CommitDateChange(CursorPos, ToggleCurChar-'0');
        break;
      case KEY_LEFT: // Move cursor to the left, stop at pos7
        CursorPos = min(CursorPos+1, LEDCOL0);
        break;
      case KEY_RIGHT: // Move cursor to the right, stop at pos0
        CursorPos = max(CursorPos-1, LEDCOL7);
        break;
      case KEY_CLEAR: // Move cursor pos to first pos
        CursorPos = LEDCOL0;
        break;
      case KEY_ENTER: // Move cursor pos to last pos
        CursorPos = LEDCOL7;
        break;
      default: // Number Keys
        ToggleCurChar = KeyPressVal;
        CommitDateChange(CursorPos, ToggleCurChar-'0');
        CursorPos = max(CursorPos-1, LEDCOL7);
    }
  }

  // Display the current state
  if(ToggleCurVal) {
    DisplayCurrentDate(CursorPos, true);
    lc.setChar(UPPERLEDROW, CursorPos, '_', false);
  } else
    DisplayCurrentDate(LEDCOLNULL, true);

  //lc.setChar(UPPERLEDROW, CursorPos, ToggleCurChar, false);
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Used to set the Hour/Minute/Second
void UpdateSetTime(void *) {
  lc.setIntensity(UPPERLEDROW, LedIntensity);
  lc.setIntensity(LOWERLEDROW, LedIntensity);

  lc.clearDisplay(UPPERLEDROW);

  // Bring cursor back inside boundary
  if (CursorPos < LEDCOL3) 
    CursorPos = LEDCOL3;

  // React to a new Keypress
  if (NewKeyPress) {
    NewKeyPress = false; // Consume the Keypress
    switch (KeyPressVal) {
      case KEY_UP: // Increment the number under the cursor, capped at "9"
        ToggleCurChar = min(ToggleCurChar+1, '9');
        CommitTimeChange(CursorPos, ToggleCurChar-'0');
        break;
      case KEY_DOWN: // Decrement the number under the cursor, floored at "0"
        ToggleCurChar = max(ToggleCurChar-1, '0');
        CommitTimeChange(CursorPos, ToggleCurChar-'0');
        break;
      case KEY_LEFT: // Move cursor to the left, stop at pos7
        CursorPos = min(CursorPos+1, LEDCOL0);
        break;
      case KEY_RIGHT: // Move cursor to the right, stop at pos4
        CursorPos = max(CursorPos-1, LEDCOL3);
        break;
      case KEY_CLEAR: // Set Second to zero
        Second = 0;
        TenthsSecond = 0;
        ChronoThSecondOffset = 0;
        break;
      case KEY_ENTER: // Increment Second by 10
        Second += 10;
        TenthsSecond = 0;
        ChronoThSecondOffset = 0;
        break;
      default: // Number Keys
        ToggleCurChar = KeyPressVal;
        CommitTimeChange(CursorPos, ToggleCurChar-'0');
        CursorPos = max(CursorPos-1, LEDCOL3);
    }
  }

  // Display the current state
  if(ToggleCurVal) {
    DisplayCurrentTime(CursorPos, true);
    lc.setChar(LOWERLEDROW, CursorPos, '_', false);
  } else
    DisplayCurrentTime(LEDCOLNULL, true);

  //lc.setChar(LOWERLEDROW, CursorPos, ToggleCurChar, false);
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// When a number is pressed on the keyboard, this changes the number in the Date under the cursor
void CommitDateChange(int CursorPos, int NewVal) {
  int CalcVal[4];

  int calc_acc;
  int calc_step;

  // If the Year is being edited
  if (CursorPos >= LEDCOL3) {
    // Convert single Int to "Char like" for Year with 4 positions
    calc_acc = Year;

    calc_step = calc_acc-calc_acc%1000;
    CalcVal[0] = calc_step/1000;
    calc_acc = calc_acc-calc_step;

    calc_step = calc_acc-calc_acc%100;
    CalcVal[1] = calc_step/100;
    calc_acc = calc_acc-calc_step;

    calc_step = calc_acc-calc_acc%10;
    CalcVal[2] = calc_step/10;
    calc_acc = calc_acc-calc_step;

    CalcVal[3] = calc_acc;

    // Swap the appropriate "Char" and convert back to Int
    if (CursorPos == LEDCOL0) {
      Year = ((NewVal)*1000)      + (CalcVal[1]*100) + (CalcVal[2]*10) + CalcVal[3];
      EEPROM.update(EEPROM_YEAR, Year/100);   
    } else if (CursorPos == LEDCOL1) {
      Year = (CalcVal[0]*1000) + ((NewVal)*100)      + (CalcVal[2]*10) + CalcVal[3];
      EEPROM.update(EEPROM_YEAR, Year/100);   
    } else if (CursorPos == LEDCOL2) {
      Year = (CalcVal[0]*1000) + (CalcVal[1]*100) + ((NewVal)*10)      + CalcVal[3];
      EEPROM.update(EEPROM_YEAR+1, Year%100);
    } else {
      Year = (CalcVal[0]*1000) + (CalcVal[1]*100) + (CalcVal[2]*10) + (NewVal);
      EEPROM.update(EEPROM_YEAR+1, Year%100);
    }
  } else if (CursorPos == LEDCOL4 || CursorPos == LEDCOL5) { // If the Month is being edited
    // Convert single Int to "Char like" for Month with 2 positions
    calc_acc = Month;

    calc_step = calc_acc-calc_acc%10;
    CalcVal[0] = calc_step/10;
    calc_acc = calc_acc-calc_step;

    CalcVal[1] = calc_acc;

    // Swap the appropriate "Char" and convert back to Int
    if (CursorPos == LEDCOL4) {
      Month = ((NewVal)*10)      + CalcVal[1];
    } else {
      Month = (CalcVal[0]*10) + (NewVal);
    }

    // Bring Month back inside boundary
    if (Month > 12)
      Month = 12;
    EEPROM.update(EEPROM_MONTH, Month);
  } else if (CursorPos == LEDCOL6 || CursorPos == LEDCOL7) { // If the Day is being edited
    // Convert single Int to "Char like" for Day with 2 positions
    calc_acc = Day;

    calc_step = calc_acc-calc_acc%10;
    CalcVal[0] = calc_step/10;
    calc_acc = calc_acc-calc_step;

    CalcVal[1] = calc_acc;

    // Swap the appropriate "Char" and convert back to Int
    if (CursorPos == LEDCOL6) {
      Day = ((NewVal)*10)      + CalcVal[1];
    } else {
      Day = (CalcVal[0]*10) + (NewVal);
    }
    EEPROM.update(EEPROM_DAY, Day);
  }

  // Bring Day back inside boundary
  // If January, March, May, July, August, October or December
  if (Month == 1 || Month == 3 || Month == 5 || Month == 7 || Month == 8 || Month == 10 || Month == 12) { 
    if (Day > 31) {
      Day = 31;
      EEPROM.update(EEPROM_DAY, Day);
    }
  // If April, June, September or November
  } else if (Month == 4 || Month == 6 || Month == 9 || Month == 11) {
    if (Day > 30) {
      Day = 30;
      EEPROM.update(EEPROM_DAY, Day);
    }
  } else {
    // If February, one leap year every year divisible by 4, but not if divisible by 100 unless it's divisible by 400
    if (Year%4==0 && (Year%100!=0 || Year%400==0)) {
      if (Day > 29) {
        Day = 29;
        EEPROM.update(EEPROM_DAY, Day);
      }
    } else {
      if (Day > 28) {
        Day = 28;
        EEPROM.update(EEPROM_DAY, Day);
      }
    }
  }

}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// When a number is pressed on the keyboard, this changes the number in the Time under the cursor 
void CommitTimeChange(int CursorPos, int NewVal) {
  int CalcVal[2];
  
  int calc_acc;
  int calc_step;

  // If Hour is being edited
  if (CursorPos == LEDCOL0 || CursorPos == LEDCOL1) {
    // Convert single Int to "Char like" for Hour with 2 positions
    calc_acc = Hour;

    calc_step = calc_acc-calc_acc%10;
    CalcVal[0] = calc_step/10;
    calc_acc = calc_acc-calc_step;

    CalcVal[1] = calc_acc;

    // Swap the appropriate "Char" and convert back to Int
    if (CursorPos == LEDCOL0) {
      Hour = ((NewVal)*10)      + CalcVal[1];
    } else {
      Hour = (CalcVal[0]*10) + (NewVal);
    }

    // Bring Hour back inside boundary
    if (Hour > 23)
      Hour = 23;
    EEPROM.update(EEPROM_HOUR+(Hour%2), Hour);
    EEPROM.update(EEPROM_HOUR+((Hour+1)%2), 0);
  } else if (CursorPos == LEDCOL2 || CursorPos == LEDCOL3) { // If Minute is being edited
    // Convert single Int to "Char like" for Minute with 2 positions
    calc_acc = Minute;

    calc_step = calc_acc-calc_acc%10;
    CalcVal[0] = calc_step/10;
    calc_acc = calc_acc-calc_step;

    CalcVal[1] = calc_acc;

    // Swap the appropriate "Char" and convert back to Int
    if (CursorPos == LEDCOL2) {
      Minute = ((NewVal)*10)      + CalcVal[1];
    } else {
      Minute = (CalcVal[0]*10) + (NewVal);
    }

    // Bring Minute back inside boundary
    if (Minute > 59)
      Minute = 59;
    EEPROM.update(EEPROM_MINUTE+(((Day%4)*(Hour+1))+Hour), Minute);
  }

  // Keep AlternateTimeZone up to date
  UpdateTZAlt(NULL); 
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Keep AlternateTimeZone up to date when Time is changed or if TimeZone edited
void UpdateTZAlt(void *) {
  // Compute new Alt values
  HourTZAlt = Hour + (TimeZoneOffsetHr-12);
  MinuteTZAlt = Minute + TimeZoneOffsetMi;

  // Keep those in boundaries
  if (HourTZAlt < 0) 
    HourTZAlt += 24;
  else if (HourTZAlt >= 24) 
    HourTZAlt -= 24;

  if (MinuteTZAlt < 0) {
    MinuteTZAlt += 60;
    HourTZAlt--;
    if (HourTZAlt < 0)
      HourTZAlt = 23;
  }else if (MinuteTZAlt >= 60) {
    MinuteTZAlt -= 60;
    HourTZAlt++;
    if (HourTZAlt >= 24)
      HourTZAlt = 0;
  }
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Used to set the CountDown value
void UpdateSetCountDown(void *) {
  lc.setIntensity(UPPERLEDROW, LedIntensity);
  lc.setIntensity(LOWERLEDROW, LedIntensity);

  lc.clearDisplay(LOWERLEDROW);

  // React to a new Keypress
  if (NewKeyPress) {
    NewKeyPress = false; // Consume the Keypress
    switch (KeyPressVal) {
      case KEY_UP: // Increment the number under the cursor, capped at "9"
        ToggleCurChar = min(ToggleCurChar+1, '9');
        CommitCountDownChange(CursorPos, ToggleCurChar-'0');
        break;
      case KEY_DOWN: // Decrement the number under the cursor, floored at "0"
        ToggleCurChar = max(ToggleCurChar-1, '0');
        CommitCountDownChange(CursorPos, ToggleCurChar-'0');
        break;
      case KEY_LEFT: // Move cursor to the left, stop at pos7
        CursorPos = min(CursorPos+1, LEDCOL0);
        break;
      case KEY_RIGHT: // Move cursor to the right, stop at pos0
        CursorPos = max(CursorPos-1, LEDCOL7);
        break;
      case KEY_CLEAR: // Clear the CountDown value
        CountDownHour   = 0;
        CountDownMinute = 0;
        CountDownSecond = 0;
        EEPROM.update(EEPROM_COUNTDOWNHOUR, 0);
        EEPROM.update(EEPROM_COUNTDOWNHOUR+1, 0);
        EEPROM.update(EEPROM_COUNTDOWNMINUTE, 0);
        EEPROM.update(EEPROM_COUNTDOWNSECOND, 0);
        EEPROM.update(EEPROM_COUNTDOWNSECONDOVER, 0);
        break;
      case KEY_ENTER: // If the Second value is bellow 970, increment it by 30 (keeping it below 999 total)
        if (CountDownSecond < 970)
          CountDownSecond += 30;
        EEPROM.update(EEPROM_COUNTDOWNSECOND, CountDownSecond%100);
        EEPROM.update(EEPROM_COUNTDOWNSECONDOVER, CountDownSecond/100);
        break;
      default: // Number Keys
        ToggleCurChar = KeyPressVal;
        CommitCountDownChange(CursorPos, ToggleCurChar-'0');
        CursorPos = max(CursorPos-1, LEDCOL7);
    }
  }

  // Display the current state
  if(ToggleCurVal) {
    DisplayCurrentCountDown(CursorPos, true);
    lc.setChar(UPPERLEDROW, CursorPos, '_', false);
  } else
    DisplayCurrentCountDown(LEDCOLNULL, true);

  //lc.setChar(UPPERLEDROW, CursorPos, ToggleCurChar, false);
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// When a number is pressed on the keyboard, this changes the number in the CountDown under the cursor 
void CommitCountDownChange(int CursorPos, int NewVal) {
  int CalcVal[3];
  
  int calc_acc;
  int calc_step;

  // If the Hour is being edited
  if (CursorPos >= LEDCOL2) {
    // Convert single Int to "Char like" for Hour with 3 positions
    calc_acc = CountDownHour;

    calc_step = calc_acc-calc_acc%100;
    CalcVal[0] = calc_step/100;
    calc_acc = calc_acc-calc_step;

    calc_step = calc_acc-calc_acc%10;
    CalcVal[1] = calc_step/10;
    calc_acc = calc_acc-calc_step;

    CalcVal[2] = calc_acc;

    // Swap the appropriate "Char" and convert back to Int
    if (CursorPos == LEDCOL0) {
      CountDownHour = ((NewVal)*100) + (CalcVal[1]*10) + CalcVal[2];
      EEPROM.update(EEPROM_COUNTDOWNHOUR, CountDownHour);
    } else if (CursorPos == LEDCOL1) {
      CountDownHour = (CalcVal[0]*100) + ((NewVal)*10) + CalcVal[2];
      EEPROM.update(EEPROM_COUNTDOWNHOUR+1, CountDownHour);
    } else if (CursorPos == LEDCOL2) {
      CountDownHour = (CalcVal[0]*100) + (CalcVal[1]*10) + (NewVal);
      EEPROM.update(EEPROM_COUNTDOWNHOUR+1, CountDownHour);
    } 

  } else if (CursorPos == LEDCOL3 || CursorPos == LEDCOL4) { // If the Minute is being edited
    // Convert single Int to "Char like" for Minute with 2 positions
    calc_acc = CountDownMinute;

    calc_step = calc_acc-calc_acc%10;
    CalcVal[0] = calc_step/10;
    calc_acc = calc_acc-calc_step;

    CalcVal[1] = calc_acc;

    // Swap the appropriate "Char" and convert back to Int
    if (CursorPos == LEDCOL3) {
      CountDownMinute = ((NewVal)*10)      + CalcVal[1];
    } else {
      CountDownMinute = (CalcVal[0]*10) + (NewVal);
    }
    EEPROM.update(EEPROM_COUNTDOWNMINUTE+(CountDownHour%10), CountDownMinute);
  } else if (CursorPos <= LEDCOL5) { // If the Second is being edited
    // Convert single Int to "Char like" for Second with 3 positions
    calc_acc = CountDownSecond;

    calc_step = calc_acc-calc_acc%100;
    CalcVal[0] = calc_step/100;
    calc_acc = calc_acc-calc_step;

    calc_step = calc_acc-calc_acc%10;
    CalcVal[1] = calc_step/10;
    calc_acc = calc_acc-calc_step;

    CalcVal[2] = calc_acc;

    // Swap the appropriate "Char" and convert back to Int
    if (CursorPos == LEDCOL5) {
      CountDownSecond = ((NewVal)*100)      + (CalcVal[1]*10) + CalcVal[2];
    } else if (CursorPos == LEDCOL6) {
      CountDownSecond = (CalcVal[0]*100) + ((NewVal)*10)      + CalcVal[2];
    } else if (CursorPos == LEDCOL7) {
      CountDownSecond = (CalcVal[0]*100) + (CalcVal[1]*10) + (NewVal);
    } 
    EEPROM.update(EEPROM_COUNTDOWNSECOND+((CountDownHour%2)*20)+(CountDownMinute%20), CountDownSecond%100);
    EEPROM.update(EEPROM_COUNTDOWNSECONDOVER, CountDownSecond/100);
  }
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Set the LockMode used to prevent modifications, see list bellow
void UpdateSetLock(void *) {
  bool ItsOk = false;

  lc.setIntensity(UPPERLEDROW, LedIntensity);
  lc.setIntensity(LOWERLEDROW, LedIntensity);

  ShowSetLock(NULL);

// 0 = None
// 1 = Lock PassCode Change
// 2 = Date/Hour/Second Change
// 3 = TimeZone/DST Change
// 4 = Countdown/Chrono/Laps Change/Set
// 5 = Display Format Set
// 6 = Dim Set

  // React to a new Keypress
  if (NewKeyPress) {
    NewKeyPress = false; // Consume the Keypress
    if (Unlocked == UM_FAILED)
      Unlocked = UM_LOCKED; // If PassCode failed, any keypress returns to prompt
    else {
      switch (KeyPressVal) {
        case KEY_UP: // If unlocked, increment the LockMode up to 6
          if (Unlocked == UM_UNLOCKED) {
            LockMode = min(LockMode+1, LM_SIZEOF);
            EEPROM.update(EEPROM_LOCKMODE, LockMode);
          }
          break;
        case KEY_DOWN: // If unlocked, decrement the LockMode down to 1
          if (Unlocked == UM_UNLOCKED) {
            LockMode = max(LockMode-1, LM_LOCK);
            EEPROM.update(EEPROM_LOCKMODE, LockMode);
          }
          break;
        case KEY_LEFT: // If unlocked, Set the LockMode to 1
          if (Unlocked == UM_UNLOCKED) {
            LockMode = LM_LOCK;
            EEPROM.update(EEPROM_LOCKMODE, LockMode);
          }
          break;
        case KEY_RIGHT: // If unlocked, Set the LockMode to 3
          if (Unlocked == UM_UNLOCKED) {
            LockMode = LM_TIMEZONE;
            EEPROM.update(EEPROM_LOCKMODE, LockMode);
          }
          break;
        case KEY_CLEAR:  // If locked or while unlocked and typing code, erase current code, if unlock and no pass, Reset PassCode
          if (Unlocked == UM_LOCKED || LockMode == LM_NONE) {
            TestPassCodeLength = 0;
          } else if (Unlocked == UM_UNLOCKED) {
            if (TestPassCodeLength > 0) { 
              TestPassCodeLength = 0;
            } else {
              LockMode = LM_NONE;
              EEPROM.update(EEPROM_LOCKMODE, LockMode);
              LockPassCodeLength = 0;
              EEPROM.update(EEPROM_LOCKPASSCODELENGTH, 0);
              TestPassCodeLength = 0;
              for (int i=0; i<MAXPWDLENGTH; i++) {
                LockPassCode[i] = 0;
                EEPROM.update(EEPROM_LOCKPASSCODE + i, 0);
              }
            }
          }
          break;
        case KEY_ENTER: // If locked , submit current code, if unlock or never init, change the current PassCode
          if (TestPassCodeLength > 0) {
            if (LockMode == LM_NONE || Unlocked == UM_UNLOCKED) {
              for (int i=0; i<TestPassCodeLength; i++) {
                LockPassCode[i] = TestPassCode[i];
                EEPROM.update(EEPROM_LOCKPASSCODE + i, LockPassCode[i]);
              }
              LockPassCodeLength = TestPassCodeLength;
              TestPassCodeLength = 0;
              EEPROM.update(EEPROM_LOCKPASSCODELENGTH, LockPassCodeLength);
              if (LockMode == LM_NONE) {
                LockMode = LM_LOCK;
                Unlocked = UM_UNLOCKED;
                EEPROM.update(EEPROM_LOCKMODE, LockMode);
              }
            } else if (Unlocked == UM_LOCKED) {
              if (TestPassCodeLength == LockPassCodeLength) {
                for (int i=0; i<TestPassCodeLength; i++) {
                  ItsOk = LockPassCode[i] == TestPassCode[i];
                  if (!ItsOk) { // PassCode verif failed
                    Unlocked = UM_FAILED;
                    LockAttempt += 1;
                    break;
                  }
                }
                if (ItsOk) { // PassCode verif success
                  Unlocked    = UM_UNLOCKED;
                  LockAttempt = 0;
                  LockTemper  = false;
                }

              } else { // If PassCode length different, verif failed
                Unlocked = UM_FAILED;
                LockAttempt += 1;
              }

              LockTemper = LockAttempt >= MAXPWDATTEMPT;
              EEPROM.update(EEPROM_LOCKATTEMPT, LockAttempt);
              TestPassCodeLength = 0;
            } 
          }
          break;
        default: // Number Keys
          if (TestPassCodeLength < MAXPWDLENGTH) {
            TestPassCode[TestPassCodeLength] = KeyPressVal - '0';
            TestPassCodeLength++;
          }
      }
    }
  }
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

