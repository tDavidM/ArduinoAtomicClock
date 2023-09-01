
void UserGetCountDown(void *) {
  lc.setIntensity(0,LedIntensity);
  lc.setIntensity(1,LedIntensity);

  DisplayCurrentTime(-1, false);
  DisplayCurrentCountDown(-1, false);

  if (NewKeyPress) {
    NewKeyPress = false;
    switch (KeyPressVal) {
      case 'U':
      case 'D':   
        if (CountDownDisplayMode == 0)
          CountDownDisplayMode = 1;
        else
          CountDownDisplayMode = 0;
        break;
      case 'R':
      case 'L': 
        NewKeyPress = false;
        break;
      case 'C':
        CountDownHour   = 0;
        CountDownMinute = 0;
        CountDownSecond = 0;
        IsCountDownActive = false;
        IsCountDownDone = false;
        break;
      case 'E':
        IsCountDownActive = !IsCountDownActive;
        break;
      default:
        NewKeyPress = false;
    }
  }

}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

void UserGetChrono(void *) {
  lc.setIntensity(0,LedIntensity);
  lc.setIntensity(1,LedIntensity);

  DisplayCurrentTime(-1, false);
  DisplayCurrentChrono(NULL);

  if (NewKeyPress) {
    NewKeyPress = false;
    switch (KeyPressVal) {
      case 'U':
      case 'D':   
        if (ChronoDisplayMode == 0)
          ChronoDisplayMode = 1;
        else
          ChronoDisplayMode = 0;
        break;
      case 'R':
      case 'L': 
        NewKeyPress = false;
        break;
      case 'C':
        ChronoHour   = 0;
        ChronoMinute = 0;
        ChronoSecond = 0;
        ChronoMilliSecond = 0;
        break;
      case 'E':
        IsChronoActive = !IsChronoActive;
        break;
      default:
        NewKeyPress = false;
    }
  }

}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

void UserChangeDate(void *) {
  lc.setIntensity(0,LedIntensity);
  lc.setIntensity(1,LedIntensity);

  DisplayCurrentDate(-1, false);
  DisplayCurrentTime(-1, false);

  if (NewKeyPress) {
    NewKeyPress = false;

    switch (KeyPressVal) {
      case 'U':
      case 'R':
        DateDisplayMode++;
        if (DateDisplayMode > 5)
          DateDisplayMode = 0;
        break;
      case 'D':
      case 'L':
        DateDisplayMode--;
        if (DateDisplayMode < 0)
          DateDisplayMode = 5;
        break;
      case 'C':
        DateDisplayMode = 0;
        break;
      case 'E':
        DateDisplayMode = 1;
        break;
      default:
        DateDisplayMode = KeyPressVal - '0';
        if (DateDisplayMode > 5)
          DateDisplayMode = 0;
        else if (DateDisplayMode < 0)
          DateDisplayMode = 5;
    }
  }
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

void UserChangeTime(void *) {
  lc.setIntensity(0,LedIntensity);
  lc.setIntensity(1,LedIntensity);

  DisplayCurrentDate(-1, false);
  DisplayCurrentTime(-1, false);

  if (NewKeyPress) {
    NewKeyPress = false;

    switch (KeyPressVal) {
      case 'U':
      case 'R':
        TimeDisplayMode++;
        if (TimeDisplayMode > 5)
          TimeDisplayMode = 0;
        break;
      case 'D':
      case 'L':
        TimeDisplayMode--;
        if (TimeDisplayMode < 0)
          TimeDisplayMode = 5;
        break;
      case 'C':
        TimeDisplayMode = 0;
        break;
      case 'E':
        TimeDisplayMode = 1;
        break;
      default:
        TimeDisplayMode = KeyPressVal - '0';
        if (TimeDisplayMode > 5)
          TimeDisplayMode = 0;
        else if (TimeDisplayMode < 0)
          TimeDisplayMode = 5;
    }
  }
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

void UserChangeDim(void *) {
  DisplayCurrentTime(-1, false);

  lc.setChar(0,7,'L',false);
  lc.setChar(0,6,'E',false);
  //lc.setChar(0,5,'V',false);
  lc.setRow(0,5,SpecChar[CH_V]);
  lc.setChar(0,4,'E',false);
  lc.setChar(0,3,'L',false);
  lc.setChar(0,2,' ',false);

  if (NewKeyPress) {
    NewKeyPress = false;

    switch (KeyPressVal) {
      case 'U':
      case 'R':
        lc.shutdown(0,false);
        lc.shutdown(1,false);
        LedIntensity = min(LedIntensity+1,0xf);
        break;
      case 'D':
      case 'L':
        lc.shutdown(0,false);
        lc.shutdown(1,false);
        LedIntensity = max(LedIntensity-1,0x0);
        break;
      case 'C':
        LedIntensity = 0x0;
        lc.shutdown(0,true);
        lc.shutdown(1,true);
        break;
      case 'E':
        lc.shutdown(0,false);
        lc.shutdown(1,false);
        LedIntensity = 0xf;
        break;
      default:
        lc.shutdown(0,false);
        lc.shutdown(1,false);
        LedIntensity = KeyPressVal - '0';
    }
  }

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


