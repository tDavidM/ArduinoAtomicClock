
void UpdateSetDate(void *) {
  //Upper is 0
  lc.setIntensity(0,LedIntensity);
  lc.setIntensity(1,LedIntensity);

  lc.clearDisplay(1);

  if (NewKeyPress) {
    NewKeyPress = false;
    switch (KeyPressVal) {
      case 'U':
        ToggleCurChar = min(ToggleCurChar+1,57);
        CommitDateChange(CursorPos,ToggleCurChar);
        break;
      case 'D':
        ToggleCurChar = max(ToggleCurChar-1,48);
        CommitDateChange(CursorPos,ToggleCurChar);
        break;
      case 'L':
        CursorPos = min(CursorPos+1,7);
        break;
      case 'R':
        CursorPos = max(CursorPos-1,0);
        break;
      case 'C':
        CursorPos = 0;
        break;
      case 'E':
        CursorPos = 7;
        break;
      default:
        ToggleCurChar = KeyPressVal;
        CommitDateChange(CursorPos,ToggleCurChar);
    }
  }

  if(ToggleCurVal==0) {
    DisplayCurrentDate(CursorPos, true);
    lc.setChar(0,CursorPos,'_',false);
  } else
    DisplayCurrentDate(-1, true);
    //lc.setChar(0,CursorPos,ToggleCurChar,false);
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

void UpdateSetTime(void *) {
  //Lower is 1
  lc.setIntensity(0,LedIntensity);
  lc.setIntensity(1,LedIntensity);

  lc.clearDisplay(0);

  if (CursorPos < 4) 
    CursorPos = 4;

  if (NewKeyPress) {
    NewKeyPress = false;
    switch (KeyPressVal) {
      case 'U':
        ToggleCurChar = min(ToggleCurChar+1,57);
        CommitTimeChange(CursorPos,ToggleCurChar);
        break;
      case 'D':
        ToggleCurChar = max(ToggleCurChar-1,48);
        CommitTimeChange(CursorPos,ToggleCurChar);
        break;
      case 'L':
        CursorPos = min(CursorPos+1,7);
        break;
      case 'R':
        CursorPos = max(CursorPos-1,4);
        break;
      case 'C':
        Second = 0;
        MilliSecond = 0;
        break;
      case 'E':
        Second += 10;
        MilliSecond = 0;
        break;
      default:
        ToggleCurChar = KeyPressVal;
        CommitTimeChange(CursorPos,ToggleCurChar);
    }
  }

  if(ToggleCurVal==0) {
    DisplayCurrentTime(CursorPos, true);
    lc.setChar(1,CursorPos,'_',false);
  } else
    DisplayCurrentTime(-1, true);
  //  lc.setChar(1,CursorPos,ToggleCurChar,false);
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

void CommitDateChange(int CursorPos, char ToggleCurChar) {
  int DisplayVal[4];

  int display_acc;
  int display_step;

  if (CursorPos >= 4) {
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

    if (CursorPos == 7) {
      Year = ((ToggleCurChar-'0')*1000) + (DisplayVal[1]*100) + (DisplayVal[2]*10) + DisplayVal[3];
    } else if (CursorPos == 6) {
      Year = (DisplayVal[0]*1000) + ((ToggleCurChar-'0')*100) + (DisplayVal[2]*10) + DisplayVal[3];
    } else if (CursorPos == 5) {
      Year = (DisplayVal[0]*1000) + (DisplayVal[1]*100) + ((ToggleCurChar-'0')*10) + DisplayVal[3];
    } else {
      Year = (DisplayVal[0]*1000) + (DisplayVal[1]*100) + (DisplayVal[2]*10) + (ToggleCurChar-'0');
    }
  } else if (CursorPos == 3 || CursorPos == 2) {
    display_acc = Month;

    display_step = display_acc-display_acc%10;
    DisplayVal[0] = display_step/10;
    display_acc = display_acc-display_step;

    DisplayVal[1] = display_acc;

    if (CursorPos == 3) {
      Month = ((ToggleCurChar-'0')*10) + DisplayVal[1];
    } else {
      Month = (DisplayVal[0]*10) + (ToggleCurChar-'0');
    }

    if (Month > 12)
      Month = 12;
  } else if (CursorPos == 1 || CursorPos == 0) {
    display_acc = Day;

    display_step = display_acc-display_acc%10;
    DisplayVal[0] = display_step/10;
    display_acc = display_acc-display_step;

    DisplayVal[1] = display_acc;

    if (CursorPos == 1) {
      Day = ((ToggleCurChar-'0')*10) + DisplayVal[1];
    } else {
      Day = (DisplayVal[0]*10) + (ToggleCurChar-'0');
    }
  }

  if (Month == 1 || Month == 3 || Month == 5 || Month == 7 || Month == 8 || Month == 10 || Month == 12) {
    if (Day > 31)
      Day = 31;
  } else if (Month == 4 || Month == 6 || Month == 9 || Month == 11) {
    if (Day > 30)
      Day = 30;
  } else {
    if (Year%4==0 && (Year%100!=0 || Year%400==0)) {
      if (Day > 29)
        Day = 29;
    } else {
      if (Day > 28)
        Day = 28;
    }
  }

}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

void CommitTimeChange(int CursorPos, char ToggleCurChar) {
  int DisplayVal[2];
  
  int display_acc;
  int display_step;

  if (CursorPos == 7 || CursorPos == 6) {
    display_acc = Hour;

    display_step = display_acc-display_acc%10;
    DisplayVal[0] = display_step/10;
    display_acc = display_acc-display_step;

    DisplayVal[1] = display_acc;

    if (CursorPos == 7) {
      Hour = ((ToggleCurChar-'0')*10) + DisplayVal[1];
    } else {
      Hour = (DisplayVal[0]*10) + (ToggleCurChar-'0');
    }

    if (Hour > 23)
      Hour = 23;
  } else if (CursorPos == 5 || CursorPos == 4) {
    display_acc = Minute;

    display_step = display_acc-display_acc%10;
    DisplayVal[0] = display_step/10;
    display_acc = display_acc-display_step;

    DisplayVal[1] = display_acc;

    if (CursorPos == 5) {
      Minute = ((ToggleCurChar-'0')*10) + DisplayVal[1];
    } else {
      Minute = (DisplayVal[0]*10) + (ToggleCurChar-'0');
    }

    if (Minute > 59)
      Minute = 59;
  }
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

void UpdateSetCountDown(void *) {
  lc.setIntensity(0,LedIntensity);
  lc.setIntensity(1,LedIntensity);

  lc.clearDisplay(1);

  if (NewKeyPress) {
    NewKeyPress = false;
    switch (KeyPressVal) {
      case 'U':
        ToggleCurChar = min(ToggleCurChar+1,57);
        CommitCountDownChange(CursorPos,ToggleCurChar);
        break;
      case 'D':
        ToggleCurChar = max(ToggleCurChar-1,48);
        CommitCountDownChange(CursorPos,ToggleCurChar);
        break;
      case 'L':
        CursorPos = min(CursorPos+1,7);
        break;
      case 'R':
        CursorPos = max(CursorPos-1,0);
        break;
      case 'C':
        CountDownHour   = 0;
        CountDownMinute = 0;
        CountDownSecond = 0;
        break;
      case 'E':
        if (CountDownSecond < 969)
        CountDownSecond += 30;
        break;
      default:
        ToggleCurChar = KeyPressVal;
        CommitCountDownChange(CursorPos,ToggleCurChar);
    }
  }

  if(ToggleCurVal==0) {
    DisplayCurrentCountDown(CursorPos, true);
    lc.setChar(0,CursorPos,'_',false);
  } else
    DisplayCurrentCountDown(-1, true);
  //  lc.setChar(0,CursorPos,ToggleCurChar,false);
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

void CommitCountDownChange(int CursorPos, char ToggleCurChar) {
  int DisplayVal[3];
  
  int display_acc;
  int display_step;

  if (CursorPos >= 5) {
    display_acc = CountDownHour;

    display_step = display_acc-display_acc%100;
    DisplayVal[0] = display_step/100;
    display_acc = display_acc-display_step;

    display_step = display_acc-display_acc%10;
    DisplayVal[1] = display_step/10;
    display_acc = display_acc-display_step;

    DisplayVal[2] = display_acc;

    if (CursorPos == 7) {
      CountDownHour = ((ToggleCurChar-'0')*100) + (DisplayVal[1]*10) + DisplayVal[2];
    } else if (CursorPos == 6) {
      CountDownHour = (DisplayVal[0]*100) + ((ToggleCurChar-'0')*10) + DisplayVal[2];
    } else if (CursorPos == 5) {
      CountDownHour = (DisplayVal[0]*100) + (DisplayVal[1]*10) + (ToggleCurChar-'0');
    } 

  } else if (CursorPos == 4 || CursorPos == 3) {
    display_acc = CountDownMinute;

    display_step = display_acc-display_acc%10;
    DisplayVal[0] = display_step/10;
    display_acc = display_acc-display_step;

    DisplayVal[1] = display_acc;

    if (CursorPos == 4) {
      CountDownMinute = ((ToggleCurChar-'0')*10) + DisplayVal[1];
    } else {
      CountDownMinute = (DisplayVal[0]*10) + (ToggleCurChar-'0');
    }

  } else if (CursorPos <= 2) {
    display_acc = CountDownSecond;

    display_step = display_acc-display_acc%100;
    DisplayVal[0] = display_step/100;
    display_acc = display_acc-display_step;

    display_step = display_acc-display_acc%10;
    DisplayVal[1] = display_step/10;
    display_acc = display_acc-display_step;

    DisplayVal[2] = display_acc;

    if (CursorPos == 2) {
      CountDownSecond = ((ToggleCurChar-'0')*100) + (DisplayVal[1]*10) + DisplayVal[2];
    } else if (CursorPos == 1) {
      CountDownSecond = (DisplayVal[0]*100) + ((ToggleCurChar-'0')*10) + DisplayVal[2];
    } else if (CursorPos == 0) {
      CountDownSecond = (DisplayVal[0]*100) + (DisplayVal[1]*10) + (ToggleCurChar-'0');
    } 

  }
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

void DisplayTestPatern(void *) {
  lc.setIntensity(0,0xf);
  lc.setIntensity(1,0xf);

  lc.shutdown(0,false);
  lc.shutdown(1,false);

  lc.setRow(0,0,SpecChar[TEST]);
  lc.setRow(0,1,SpecChar[TEST]);
  lc.setRow(0,2,SpecChar[TEST]);
  lc.setRow(0,3,SpecChar[TEST]);
  lc.setRow(0,4,SpecChar[TEST]);
  lc.setRow(0,5,SpecChar[TEST]);
  lc.setRow(0,6,SpecChar[TEST]);
  lc.setRow(0,7,SpecChar[TEST]);

  lc.setRow(1,0,SpecChar[TEST]);
  lc.setRow(1,1,SpecChar[TEST]);
  lc.setRow(1,2,SpecChar[TEST]);
  lc.setRow(1,3,SpecChar[TEST]);
  lc.setRow(1,4,SpecChar[TEST]);
  lc.setRow(1,5,SpecChar[TEST]);
  lc.setRow(1,6,SpecChar[TEST]);
  lc.setRow(1,7,SpecChar[TEST]);

  digitalWrite(LedTickPin, HIGH);
  digitalWrite(LedLockPin, HIGH);
}
