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
  lc.setIntensity(0,LedIntensity);
  lc.setIntensity(1,LedIntensity);

  lc.clearDisplay(1);

  // React to a new Keypress
  if (NewKeyPress) {
    NewKeyPress = false; // Consume the Keypress
    switch (KeyPressVal) {
      case 'U': // Increment the number under the cursor, capped at "9"
        ToggleCurChar = min(ToggleCurChar+1,57);
        CommitDateChange(CursorPos,ToggleCurChar-'0');
        break;
      case 'D': // Decrement the number under the cursor, floored at "0"
        ToggleCurChar = max(ToggleCurChar-1,48);
        CommitDateChange(CursorPos,ToggleCurChar-'0');
        break;
      case 'L': // Move cursor to the left, stop at pos7
        CursorPos = min(CursorPos+1, 7);
        break;
      case 'R': // Move cursor to the right, stop at pos0
        CursorPos = max(CursorPos-1, 0);
        break;
      case 'C': // Move cursor pos to first pos
        CursorPos = 7;
        break;
      case 'E': // Move cursor pos to last pos
        CursorPos = 0;
        break;
      default: // Number Keys
        ToggleCurChar = KeyPressVal;
        CommitDateChange(CursorPos,ToggleCurChar-'0');
        CursorPos = max(CursorPos-1, 0);
    }
  }

  // Display the current state
  if(ToggleCurVal) {
    DisplayCurrentDate(CursorPos, true);
    lc.setChar(0,CursorPos,'_',false);
  } else
    DisplayCurrentDate(-1, true);

  //lc.setChar(0,CursorPos,ToggleCurChar,false);
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Used to set the Hour/Minute/Second
void UpdateSetTime(void *) {
  //Lower is 1
  lc.setIntensity(0,LedIntensity);
  lc.setIntensity(1,LedIntensity);

  lc.clearDisplay(0);

  // Bring cursor back inside boundary
  if (CursorPos < 4) 
    CursorPos = 4;

  // React to a new Keypress
  if (NewKeyPress) {
    NewKeyPress = false; // Consume the Keypress
    switch (KeyPressVal) {
      case 'U': // Increment the number under the cursor, capped at "9"
        ToggleCurChar = min(ToggleCurChar+1,57);
        CommitTimeChange(CursorPos,ToggleCurChar-'0');
        break;
      case 'D': // Decrement the number under the cursor, floored at "0"
        ToggleCurChar = max(ToggleCurChar-1,48);
        CommitTimeChange(CursorPos,ToggleCurChar-'0');
        break;
      case 'L': // Move cursor to the left, stop at pos7
        CursorPos = min(CursorPos+1, 7);
        break;
      case 'R': // Move cursor to the right, stop at pos4
        CursorPos = max(CursorPos-1, 4);
        break;
      case 'C': // Set Second to zero
        Second = 0;
        TenthsSecond = 0;
        ChronoThSecondOffset = 0;
        break;
      case 'E': // Increment Second by 10
        Second += 10;
        TenthsSecond = 0;
        ChronoThSecondOffset = 0;
        break;
      default: // Number Keys
        ToggleCurChar = KeyPressVal;
        CommitTimeChange(CursorPos,ToggleCurChar-'0');
        CursorPos = max(CursorPos-1, 4);
    }
  }

  // Display the current state
  if(ToggleCurVal) {
    DisplayCurrentTime(CursorPos, true);
    lc.setChar(1,CursorPos,'_',false);
  } else
    DisplayCurrentTime(-1, true);

  //lc.setChar(1,CursorPos,ToggleCurChar,false);
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// When a number is pressed on the keyboard, this changes the number in the Date under the cursor
void CommitDateChange(int CursorPos, int NewVal) {
  int DisplayVal[4];

  int display_acc;
  int display_step;

  // If the Year is being edited
  if (CursorPos >= 4) {
    // Convert single Int to "Char like" for Year with 4 positions
    display_acc = Year;

    display_step = display_acc-display_acc%1000;
    DisplayVal[0] = display_step/1000;
    display_acc = display_acc-display_step;

    display_step = display_acc-display_acc%100;
    DisplayVal[1] = display_step/100;
    display_acc = display_acc-display_step;

    display_step = display_acc-display_acc%10;
    DisplayVal[2] = display_step/10;
    display_acc = display_acc-display_step;

    DisplayVal[3] = display_acc;

    // Swap the appropriate "Char" and convert back to Int
    if (CursorPos == 7) {
      Year = ((NewVal)*1000)      + (DisplayVal[1]*100) + (DisplayVal[2]*10) + DisplayVal[3];
      EEPROM.update(EEPROM_YEAR, Year/100);   
    } else if (CursorPos == 6) {
      Year = (DisplayVal[0]*1000) + ((NewVal)*100)      + (DisplayVal[2]*10) + DisplayVal[3];
      EEPROM.update(EEPROM_YEAR, Year/100);   
    } else if (CursorPos == 5) {
      Year = (DisplayVal[0]*1000) + (DisplayVal[1]*100) + ((NewVal)*10)      + DisplayVal[3];
      EEPROM.update(EEPROM_YEAR+1, Year%100);
    } else {
      Year = (DisplayVal[0]*1000) + (DisplayVal[1]*100) + (DisplayVal[2]*10) + (NewVal);
      EEPROM.update(EEPROM_YEAR+1, Year%100);
    }
  } else if (CursorPos == 3 || CursorPos == 2) { // If the Month is being edited
    // Convert single Int to "Char like" for Month with 2 positions
    display_acc = Month;

    display_step = display_acc-display_acc%10;
    DisplayVal[0] = display_step/10;
    display_acc = display_acc-display_step;

    DisplayVal[1] = display_acc;

    // Swap the appropriate "Char" and convert back to Int
    if (CursorPos == 3) {
      Month = ((NewVal)*10)      + DisplayVal[1];
    } else {
      Month = (DisplayVal[0]*10) + (NewVal);
    }

    // Bring Month back inside boundary
    if (Month > 12)
      Month = 12;
    EEPROM.update(EEPROM_MONTH, Month);
  } else if (CursorPos == 1 || CursorPos == 0) { // If the Day is being edited
    // Convert single Int to "Char like" for Day with 2 positions
    display_acc = Day;

    display_step = display_acc-display_acc%10;
    DisplayVal[0] = display_step/10;
    display_acc = display_acc-display_step;

    DisplayVal[1] = display_acc;

    // Swap the appropriate "Char" and convert back to Int
    if (CursorPos == 1) {
      Day = ((NewVal)*10)      + DisplayVal[1];
    } else {
      Day = (DisplayVal[0]*10) + (NewVal);
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
  int DisplayVal[2];
  
  int display_acc;
  int display_step;

  // If Hour is being edited
  if (CursorPos == 7 || CursorPos == 6) {
    // Convert single Int to "Char like" for Hour with 2 positions
    display_acc = Hour;

    display_step = display_acc-display_acc%10;
    DisplayVal[0] = display_step/10;
    display_acc = display_acc-display_step;

    DisplayVal[1] = display_acc;

    // Swap the appropriate "Char" and convert back to Int
    if (CursorPos == 7) {
      Hour = ((NewVal)*10)      + DisplayVal[1];
    } else {
      Hour = (DisplayVal[0]*10) + (NewVal);
    }

    // Bring Hour back inside boundary
    if (Hour > 23)
      Hour = 23;
    EEPROM.update(EEPROM_HOUR+(Hour%2), Hour);
    EEPROM.update(EEPROM_HOUR+((Hour+1)%2), 0);
  } else if (CursorPos == 5 || CursorPos == 4) { // If Minute is being edited
    // Convert single Int to "Char like" for Minute with 2 positions
    display_acc = Minute;

    display_step = display_acc-display_acc%10;
    DisplayVal[0] = display_step/10;
    display_acc = display_acc-display_step;

    DisplayVal[1] = display_acc;

    // Swap the appropriate "Char" and convert back to Int
    if (CursorPos == 5) {
      Minute = ((NewVal)*10)      + DisplayVal[1];
    } else {
      Minute = (DisplayVal[0]*10) + (NewVal);
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
  lc.setIntensity(0,LedIntensity);
  lc.setIntensity(1,LedIntensity);

  lc.clearDisplay(1);

  // React to a new Keypress
  if (NewKeyPress) {
    NewKeyPress = false; // Consume the Keypress
    switch (KeyPressVal) {
      case 'U': // Increment the number under the cursor, capped at "9"
        ToggleCurChar = min(ToggleCurChar+1,57);
        CommitCountDownChange(CursorPos,ToggleCurChar-'0');
        break;
      case 'D': // Decrement the number under the cursor, floored at "0"
        ToggleCurChar = max(ToggleCurChar-1,48);
        CommitCountDownChange(CursorPos,ToggleCurChar-'0');
        break;
      case 'L': // Move cursor to the left, stop at pos7
        CursorPos = min(CursorPos+1, 7);
        break;
      case 'R': // Move cursor to the right, stop at pos0
        CursorPos = max(CursorPos-1, 0);
        break;
      case 'C': // Clear the CountDown value
        CountDownHour   = 0;
        CountDownMinute = 0;
        CountDownSecond = 0;
        EEPROM.update(EEPROM_COUNTDOWNHOUR, 0);
        EEPROM.update(EEPROM_COUNTDOWNHOUR+1, 0);
        EEPROM.update(EEPROM_COUNTDOWNMINUTE, 0);
        EEPROM.update(EEPROM_COUNTDOWNSECOND, 0);
        EEPROM.update(EEPROM_COUNTDOWNSECONDOVER, 0);
        break;
      case 'E': // If the Second value is bellow 969, increment it by 30
        if (CountDownSecond < 969)
          CountDownSecond += 30;
        EEPROM.update(EEPROM_COUNTDOWNSECOND, CountDownSecond%100);
        EEPROM.update(EEPROM_COUNTDOWNSECONDOVER, CountDownSecond/100);
        break;
      default: // Number Keys
        ToggleCurChar = KeyPressVal;
        CommitCountDownChange(CursorPos,ToggleCurChar-'0');
        CursorPos = max(CursorPos-1, 0);
    }
  }

  // Display the current state
  if(ToggleCurVal) {
    DisplayCurrentCountDown(CursorPos, true);
    lc.setChar(0,CursorPos,'_',false);
  } else
    DisplayCurrentCountDown(-1, true);

  //lc.setChar(0,CursorPos,ToggleCurChar,false);
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// When a number is pressed on the keyboard, this changes the number in the CountDown under the cursor 
void CommitCountDownChange(int CursorPos, int NewVal) {
  int DisplayVal[3];
  
  int display_acc;
  int display_step;

  // If the Hour is being edited
  if (CursorPos >= 5) {
    // Convert single Int to "Char like" for Hour with 3 positions
    display_acc = CountDownHour;

    display_step = display_acc-display_acc%100;
    DisplayVal[0] = display_step/100;
    display_acc = display_acc-display_step;

    display_step = display_acc-display_acc%10;
    DisplayVal[1] = display_step/10;
    display_acc = display_acc-display_step;

    DisplayVal[2] = display_acc;

    // Swap the appropriate "Char" and convert back to Int
    if (CursorPos == 7) {
      CountDownHour = ((NewVal)*100) + (DisplayVal[1]*10) + DisplayVal[2];
      EEPROM.update(EEPROM_COUNTDOWNHOUR, CountDownHour);
    } else if (CursorPos == 6) {
      CountDownHour = (DisplayVal[0]*100) + ((NewVal)*10) + DisplayVal[2];
      EEPROM.update(EEPROM_COUNTDOWNHOUR+1, CountDownHour);
    } else if (CursorPos == 5) {
      CountDownHour = (DisplayVal[0]*100) + (DisplayVal[1]*10) + (NewVal);
      EEPROM.update(EEPROM_COUNTDOWNHOUR+1, CountDownHour);
    } 

  } else if (CursorPos == 4 || CursorPos == 3) { // If the Minute is being edited
    // Convert single Int to "Char like" for Minute with 2 positions
    display_acc = CountDownMinute;

    display_step = display_acc-display_acc%10;
    DisplayVal[0] = display_step/10;
    display_acc = display_acc-display_step;

    DisplayVal[1] = display_acc;

    // Swap the appropriate "Char" and convert back to Int
    if (CursorPos == 4) {
      CountDownMinute = ((NewVal)*10)      + DisplayVal[1];
    } else {
      CountDownMinute = (DisplayVal[0]*10) + (NewVal);
    }
    EEPROM.update(EEPROM_COUNTDOWNMINUTE+(CountDownHour%10), CountDownMinute);
  } else if (CursorPos <= 2) { // If the Second is being edited
    // Convert single Int to "Char like" for Second with 3 positions
    display_acc = CountDownSecond;

    display_step = display_acc-display_acc%100;
    DisplayVal[0] = display_step/100;
    display_acc = display_acc-display_step;

    display_step = display_acc-display_acc%10;
    DisplayVal[1] = display_step/10;
    display_acc = display_acc-display_step;

    DisplayVal[2] = display_acc;

    // Swap the appropriate "Char" and convert back to Int
    if (CursorPos == 2) {
      CountDownSecond = ((NewVal)*100)      + (DisplayVal[1]*10) + DisplayVal[2];
    } else if (CursorPos == 1) {
      CountDownSecond = (DisplayVal[0]*100) + ((NewVal)*10)      + DisplayVal[2];
    } else if (CursorPos == 0) {
      CountDownSecond = (DisplayVal[0]*100) + (DisplayVal[1]*10) + (NewVal);
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

  lc.setIntensity(0,LedIntensity);
  lc.setIntensity(1,LedIntensity);

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
    if (Unlocked == 2)
      Unlocked = 0; // If PassCode failed, any keypress returns to prompt
    else {
      switch (KeyPressVal) {
        case 'U': // If unlocked, increment the LockMode up to 6
          if (Unlocked == 1) {
            LockMode = min(LockMode+1, 6);
            EEPROM.update(EEPROM_LOCKMODE, LockMode);
          }
          break;
        case 'D': // If unlocked, decrement the LockMode down to 1
          if (Unlocked == 1) {
            LockMode = max(LockMode-1, 1);
            EEPROM.update(EEPROM_LOCKMODE, LockMode);
          }
          break;
        case 'L': // If unlocked, Set the LockMode to 1
          if (Unlocked == 1) {
            LockMode = 1;
            EEPROM.update(EEPROM_LOCKMODE, LockMode);
          }
          break;
        case 'R': // If unlocked, Set the LockMode to 3
          if (Unlocked == 1) {
            LockMode = 3;
            EEPROM.update(EEPROM_LOCKMODE, LockMode);
          }
          break;
        case 'C':  // If locked or while unlocked and typing code, erase current code, if unlock and no pass, Reset PassCode
          if (Unlocked == 0 || LockMode == 0) {
            TestPassCodeLength = 0;
          } else if (Unlocked == 1) {
            if (TestPassCodeLength > 0) { 
              TestPassCodeLength = 0;
            } else {
              LockMode = 0;
              EEPROM.update(EEPROM_LOCKMODE, LockMode);
              LockPassCodeLength = 0;
              EEPROM.update(EEPROM_LOCKPASSCODELENGTH, 0);
              TestPassCodeLength = 0;
              for (int i=0; i<16; i++) {
                LockPassCode[i] = 0;
                EEPROM.update(EEPROM_LOCKPASSCODE + i, 0);
              }
            }
          }
          break;
        case 'E': // If locked , submit current code, if unlock or vener init, change the current PassCode
          if (TestPassCodeLength > 0) {
            if (LockMode == 0 || Unlocked == 1) {
              for (int i=0; i<TestPassCodeLength; i++) {
                LockPassCode[i] = TestPassCode[i];
                EEPROM.update(EEPROM_LOCKPASSCODE + i, LockPassCode[i]);
              }
              LockPassCodeLength = TestPassCodeLength;
              TestPassCodeLength = 0;
              EEPROM.update(EEPROM_LOCKPASSCODELENGTH, LockPassCodeLength);
              if (LockMode == 0) {
                LockMode = 1;
                Unlocked = 1;
                EEPROM.update(EEPROM_LOCKMODE, LockMode);
              }
            } else if (Unlocked == 0) {
               if (TestPassCodeLength == LockPassCodeLength) {
                 for (int i=0; i<TestPassCodeLength; i++) {
                  ItsOk = LockPassCode[i] == TestPassCode[i];
                  if (!ItsOk) {
                    Unlocked = 2;
                    break;
                  }
                }
                if (ItsOk)
                  Unlocked = 1;
              } else
                Unlocked = 2;

              TestPassCodeLength = 0;
            } 
          }
          break;
        default: // Number Keys
          if (TestPassCodeLength < 16) {
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

