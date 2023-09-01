


//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

bool DisplayCurrentDate(int SkipChar, int ForceModeZero) {
  int DisplayVal[8];

  int display_acc;
  int display_step;

  lc.setIntensity(0,LedIntensity);

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

  display_acc = Month;

  display_step = display_acc-display_acc%10;
  DisplayVal[4] = display_step/10;
  display_acc = display_acc-display_step;

  DisplayVal[5] = display_acc;

  display_acc = Day;

  display_step = display_acc-display_acc%10;
  DisplayVal[6] = display_step/10;
  display_acc = display_acc-display_step;

  DisplayVal[7] = display_acc;

  
  if (DateDisplayMode == 0 || ForceModeZero) {
    //YYYYMMDD
    if (SkipChar != 7)
      lc.setDigit(0,7,DisplayVal[0],false);
    else
      ToggleCurChar = DisplayVal[0] + '0';

    if (SkipChar != 6)
      lc.setDigit(0,6,DisplayVal[1],false);
    else
      ToggleCurChar = DisplayVal[1] + '0';

    if (SkipChar != 5)
      lc.setDigit(0,5,DisplayVal[2],false);
    else
      ToggleCurChar = DisplayVal[2] + '0';

    if (SkipChar != 4)
      lc.setRow(0,4,SpecChar[DisplayVal[3]+10]);
    else
      ToggleCurChar = DisplayVal[3] + '0';


    if (SkipChar != 3)
      lc.setDigit(0,3,DisplayVal[4],false);
    else
      ToggleCurChar = DisplayVal[4] + '0';

    if (SkipChar != 2)
      lc.setRow(0,2,SpecChar[DisplayVal[5]+10]);
    else
      ToggleCurChar = DisplayVal[5] + '0';

    if (SkipChar != 1)
      lc.setDigit(0,1,DisplayVal[6],false);
    else
      ToggleCurChar = DisplayVal[6] + '0';

    if (SkipChar != 0)
      lc.setDigit(0,0,DisplayVal[7],false);
    else
      ToggleCurChar = DisplayVal[7] + '0';
  } else if (DateDisplayMode == 1 ) {
    //YY-MM-DD
    lc.setDigit(0,7,DisplayVal[2],false);
    lc.setDigit(0,6,DisplayVal[3],false);
    lc.setRow(0,5,SpecChar[BAR]);
    lc.setDigit(0,4,DisplayVal[4],false);
    lc.setDigit(0,3,DisplayVal[5],false);
    lc.setRow(0,2,SpecChar[BAR]);
    lc.setDigit(0,1,DisplayVal[6],false);
    lc.setDigit(0,0,DisplayVal[7],false);
  } else if (DateDisplayMode == 2 ) {
    //YY MM DD
    lc.setDigit(0,7,DisplayVal[2],false);
    lc.setDigit(0,6,DisplayVal[3],false);
    lc.setRow(0,5,SpecChar[BLANK]);
    lc.setDigit(0,4,DisplayVal[4],false);
    lc.setDigit(0,3,DisplayVal[5],false);
    lc.setRow(0,2,SpecChar[BLANK]);
    lc.setDigit(0,1,DisplayVal[6],false);
    lc.setDigit(0,0,DisplayVal[7],false);
  } else if (DateDisplayMode == 3 ) {
    //MMDDYYYY
    lc.setDigit(0,7,DisplayVal[4],false);
    lc.setRow(0,6,SpecChar[DisplayVal[5]+10]);
    lc.setDigit(0,5,DisplayVal[6],false);
    lc.setRow(0,4,SpecChar[DisplayVal[7]+10]);
    lc.setDigit(0,3,DisplayVal[0],false);
    lc.setDigit(0,2,DisplayVal[1],false);
    lc.setDigit(0,1,DisplayVal[2],false);
    lc.setDigit(0,0,DisplayVal[3],false);
  } else if (DateDisplayMode == 4 ) {
    //DDMMYYYY
    lc.setDigit(0,7,DisplayVal[6],false);
    lc.setRow(0,6,SpecChar[DisplayVal[7]+10]);
    lc.setDigit(0,5,DisplayVal[4],false);
    lc.setRow(0,4,SpecChar[DisplayVal[5]+10]);
    lc.setDigit(0,3,DisplayVal[0],false);
    lc.setDigit(0,2,DisplayVal[1],false);
    lc.setDigit(0,1,DisplayVal[2],false);
    lc.setDigit(0,0,DisplayVal[3],false);
  } else if (DateDisplayMode == 5 ) {
    //YYYYDDMM
    lc.setDigit(0,7,DisplayVal[0],false);
    lc.setDigit(0,6,DisplayVal[1],false);
    lc.setDigit(0,5,DisplayVal[2],false);
    lc.setRow(0,4,SpecChar[DisplayVal[3]+10]);
    lc.setDigit(0,3,DisplayVal[6],false);
    lc.setRow(0,2,SpecChar[DisplayVal[7]+10]);
    lc.setDigit(0,1,DisplayVal[4],false);
    lc.setDigit(0,0,DisplayVal[5],false);
  }

  return true;
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

bool DisplayCurrentTime(int SkipChar, bool ForceModeZero) {
  int DisplayVal[7];
  int DisplayValAlt[2];
  bool IndPM = false;

  int display_acc;
  int display_step;

  lc.setIntensity(1,LedIntensity);

  display_acc = Hour;

  display_step = display_acc-display_acc%10;
  DisplayVal[0] = display_step/10;
  display_acc = display_acc-display_step;

  DisplayVal[1] = display_acc;

  if (TimeDisplayMode >= 4) {
    if (Hour == 0)
      display_acc = 12;
    else if (Hour == 12) {
      IndPM = true;
      display_acc = Hour;
    } else if (Hour > 12) {
      IndPM = true;
      display_acc = Hour - 12;
    } else {
      display_acc = Hour;
    }

    display_step = display_acc-display_acc%10;
    DisplayValAlt[0] = display_step/10;
    display_acc = display_acc-display_step;

    DisplayValAlt[1] = display_acc;
  }

  display_acc = Minute;

  display_step = display_acc-display_acc%10;
  DisplayVal[2] = display_step/10;
  display_acc = display_acc-display_step;

  DisplayVal[3] = display_acc;

  display_acc = Second;

  display_step = display_acc-display_acc%10;
  DisplayVal[4] = display_step/10;
  display_acc = display_acc-display_step;

  DisplayVal[5] = display_acc;

  display_acc = MilliSecond;

  display_step = display_acc-display_acc%10;
  DisplayVal[6] = display_step/10;


  if (TimeDisplayMode == 0 || ForceModeZero) {
    //HHMISS X
    if (SkipChar != 7)
      lc.setDigit(1,7,DisplayVal[0],false);
    else
      ToggleCurChar = DisplayVal[0] + '0';

    if (SkipChar != 6)
      lc.setRow(1,6,SpecChar[DisplayVal[1]+10]);
    else
      ToggleCurChar = DisplayVal[1] + '0';


    if (SkipChar != 5)
      lc.setDigit(1,5,DisplayVal[2],false);
    else
      ToggleCurChar = DisplayVal[2] + '0';

    if (SkipChar != 4)
      lc.setRow(1,4,SpecChar[DisplayVal[3]+10]);
    else
      ToggleCurChar = DisplayVal[3] + '0';


    lc.setDigit(1,3,DisplayVal[4],false);
    //lc.setRow(1,2,SpecChar[DisplayVal[5]+10]);
    lc.setDigit(1,2,DisplayVal[5],false);


    lc.setRow(1,1,SpecChar[BLANK]);
    lc.setDigit(1,0,DisplayVal[6],false);
  } else if (TimeDisplayMode == 1 ) {
    //HH_MISSX
    lc.setDigit(1,7,DisplayVal[0],false);
    lc.setDigit(1,6,DisplayVal[1],false);
    lc.setRow(1,5,SpecChar[CH_H]);
    lc.setDigit(1,4,DisplayVal[2],false);
    lc.setRow(1,3,SpecChar[DisplayVal[3]+10]);
    lc.setDigit(1,2,DisplayVal[4],false);
    lc.setRow(1,1,SpecChar[DisplayVal[5]+10]);
    lc.setDigit(1,0,DisplayVal[6],false);
  } else if (TimeDisplayMode == 2 ) {
    //HH_MI SS
    lc.setDigit(1,7,DisplayVal[0],false);
    lc.setDigit(1,6,DisplayVal[1],false);
    lc.setRow(1,5,SpecChar[CH_H]);
    lc.setDigit(1,4,DisplayVal[2],false);
    lc.setDigit(1,3,DisplayVal[3],false);
    lc.setRow(1,2,SpecChar[BLANK]);
    lc.setDigit(1,1,DisplayVal[4],false);
    lc.setDigit(1,0,DisplayVal[5],false);
  } else if (TimeDisplayMode == 3 ) {
    //HH MI SS
    lc.setDigit(1,7,DisplayVal[0],false);
    lc.setDigit(1,6,DisplayVal[1],false);
    lc.setRow(1,5,SpecChar[BLANK]);
    lc.setDigit(1,4,DisplayVal[2],false);
    lc.setDigit(1,3,DisplayVal[3],false);
    lc.setRow(1,2,SpecChar[BLANK]);
    lc.setDigit(1,1,DisplayVal[4],false);
    lc.setDigit(1,0,DisplayVal[5],false);
  } else if (TimeDisplayMode == 4 ) {
    //HHPMISSX
    lc.setDigit(1,7,DisplayValAlt[0],false);
    lc.setDigit(1,6,DisplayValAlt[1],false);
    if (IndPM)
      lc.setRow(1,5,SpecChar[CH_P]);
    else
      lc.setRow(1,5,SpecChar[CH_A]);
    lc.setDigit(1,4,DisplayVal[2],false);
    lc.setDigit(1,3,DisplayVal[3],false);
    lc.setRow(1,2,SpecChar[BLANK]);
    lc.setDigit(1,1,DisplayVal[4],false);
    lc.setDigit(1,0,DisplayVal[5],false);
  } else if (TimeDisplayMode == 5 ) {
    //HHPMI SS
    lc.setDigit(1,7,DisplayValAlt[0],false);
    lc.setDigit(1,6,DisplayValAlt[1],false);
    if (IndPM)
      lc.setRow(1,5,SpecChar[CH_P]);
    else
      lc.setRow(1,5,SpecChar[CH_A]);
    lc.setDigit(1,4,DisplayVal[2],false);
    lc.setRow(1,3,SpecChar[DisplayVal[3]+10]);
    lc.setDigit(1,2,DisplayVal[4],false);
    lc.setRow(1,1,SpecChar[DisplayVal[5]+10]);
    lc.setDigit(1,0,DisplayVal[6],false);
  }

  return true;
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

void DisplayCurrentChrono(void *) {
  int DisplayVal[8];

  int display_acc;
  int display_step;

  display_acc = ChronoHour;

  display_step = display_acc-display_acc%100;
  DisplayVal[0] = display_step/100;
  display_acc = display_acc-display_step;

  display_step = display_acc-display_acc%10;
  DisplayVal[1] = display_step/10;
  display_acc = display_acc-display_step;

  DisplayVal[2] = display_acc;

  display_acc = ChronoMinute;

  display_step = display_acc-display_acc%10;
  DisplayVal[3] = display_step/10;
  display_acc = display_acc-display_step;

  DisplayVal[4] = display_acc;

  display_acc = ChronoSecond;

  display_step = display_acc-display_acc%10;
  DisplayVal[5] = display_step/10;
  display_acc = display_acc-display_step;

  DisplayVal[6] = display_acc;

  display_acc = ChronoMilliSecond;

  display_step = display_acc-display_acc%10;
  DisplayVal[7] = display_step/10;


  if (ChronoDisplayMode == 0) {
    //HHHMISSX
    lc.setDigit(0,7,DisplayVal[0],false);
    lc.setDigit(0,6,DisplayVal[1],false);
    lc.setRow(0,5,SpecChar[DisplayVal[2]+10]);
    lc.setDigit(0,4,DisplayVal[3],false);
    lc.setRow(0,3,SpecChar[DisplayVal[4]+10]);
    lc.setDigit(0,2,DisplayVal[5],false);
    lc.setRow(0,1,SpecChar[DisplayVal[6]+10]);
    lc.setDigit(0,0,DisplayVal[7],false);
  } else if (ChronoDisplayMode == 1 ) {
    //HHHMI_SS
    lc.setDigit(0,7,DisplayVal[0],false);
    lc.setDigit(0,6,DisplayVal[1],false);
    lc.setRow(0,5,SpecChar[DisplayVal[2]+10]);
    lc.setDigit(0,4,DisplayVal[3],false);
    lc.setDigit(0,3,DisplayVal[4],false);
    lc.setRow(0,2,SpecChar[BLANK]);
    lc.setDigit(0,1,DisplayVal[5],false);
    lc.setDigit(0,0,DisplayVal[6],false); 
  }

}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

bool DisplayCurrentCountDown(int SkipChar, bool ForceModeZero) {
  int DisplayVal[8];

  int display_acc;
  int display_step;

  if (IsCountDownDone) {
    if(ToggleCurVal==0) {
      lc.setChar(0,7,'_',false);
      lc.setChar(0,6,'_',false);
      lc.setChar(0,5,'_',false);
      lc.setChar(0,4,'_',false);
      lc.setChar(0,3,'_',false);
      lc.setChar(0,2,'_',false);
      lc.setChar(0,1,'_',false);
      lc.setChar(0,0,'_',false);
    } else {
      lc.setRow(0,7,SpecChar[10]);
      lc.setRow(0,6,SpecChar[10]);
      lc.setRow(0,5,SpecChar[10]);
      lc.setRow(0,4,SpecChar[10]);
      lc.setRow(0,3,SpecChar[10]);
      lc.setRow(0,2,SpecChar[10]);
      lc.setRow(0,1,SpecChar[10]);
      lc.setRow(0,0,SpecChar[10]);
    }
  }else {

    display_acc = CountDownHour;

    display_step = display_acc-display_acc%100;
    DisplayVal[0] = display_step/100;
    display_acc = display_acc-display_step;

    display_step = display_acc-display_acc%10;
    DisplayVal[1] = display_step/10;
    display_acc = display_acc-display_step;

    DisplayVal[2] = display_acc;

    display_acc = CountDownMinute;

    display_step = display_acc-display_acc%10;
    DisplayVal[3] = display_step/10;
    display_acc = display_acc-display_step;

    DisplayVal[4] = display_acc;

    display_acc = CountDownSecond;

    display_step = display_acc-display_acc%100;
    DisplayVal[5] = display_step/100;
    display_acc = display_acc-display_step;

    display_step = display_acc-display_acc%10;
    DisplayVal[6] = display_step/10;
    display_acc = display_acc-display_step;

    DisplayVal[7] = display_acc;


    if (CountDownDisplayMode == 0 || ForceModeZero) {
      //HHHMISSS
      if (SkipChar != 7)
        lc.setDigit(0,7,DisplayVal[0],false);
      else
        ToggleCurChar = DisplayVal[0] + '0';

      if (SkipChar != 6)
        lc.setDigit(0,6,DisplayVal[1],false);
      else
        ToggleCurChar = DisplayVal[1] + '0';

      if (SkipChar != 5)
        lc.setRow(0,5,SpecChar[DisplayVal[2]+10]);
      else
        ToggleCurChar = DisplayVal[2] + '0';

      if (SkipChar != 4)
        lc.setDigit(0,4,DisplayVal[3],false);
      else
        ToggleCurChar = DisplayVal[3] + '0';

      if (SkipChar != 3)
        lc.setRow(0,3,SpecChar[DisplayVal[4]+10]);
      else
        ToggleCurChar = DisplayVal[4] + '0';

      if (SkipChar != 2)
        lc.setDigit(0,2,DisplayVal[5],false);
      else
        ToggleCurChar = DisplayVal[5] + '0';

      if (SkipChar != 1)
        lc.setDigit(0,1,DisplayVal[6],false);
      else
        ToggleCurChar = DisplayVal[6] + '0';
      
      if (SkipChar != 0)
        lc.setDigit(0,0,DisplayVal[7],false);
      else
        ToggleCurChar = DisplayVal[7] + '0';

    } else if (CountDownDisplayMode == 1 ) {
      //HHHMI_SS
      lc.setDigit(0,7,DisplayVal[0],false);
      lc.setDigit(0,6,DisplayVal[1],false);
      lc.setRow(0,5,SpecChar[DisplayVal[2]+10]);
      lc.setDigit(0,4,DisplayVal[3],false);

      if (DisplayVal[5] == 0) {
        lc.setDigit(0,3,DisplayVal[4],false);
        lc.setRow(0,2,SpecChar[BLANK]);
      } else {
        lc.setRow(0,3,SpecChar[DisplayVal[4]+10]);
        lc.setDigit(0,2,DisplayVal[5],false);
      }

      lc.setDigit(0,1,DisplayVal[6],false); 
      lc.setDigit(0,0,DisplayVal[7],false);
    }
  }

  return true;
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

void UserShowCounter(void *) {
/*
  int display_acc = cmp;
  int display_step;

/*
  for (int i=4; i>=0; i--) {
    if(i>0) {
      display_step = display_acc-display_acc%round(pow(10,i));
      lc.setDigit(0,i,display_step/round(pow(10,i)),false);
      display_acc = display_acc-display_step;
    } else {
      lc.setDigit(0,i,display_acc,false);
    }
  }
* /

//Serial.println(cmp);

  display_acc = cmp;

  display_step = display_acc-display_acc%10000;
  lc.setDigit(1,4,display_step/10000,false);
  display_acc = display_acc-display_step;

  display_step = display_acc-display_acc%1000;
  lc.setDigit(1,3,display_step/1000,false);
  display_acc = display_acc-display_step;

  display_step = display_acc-display_acc%100;
  lc.setDigit(1,2,(display_step/100),false);
  display_acc = display_acc-display_step;

  display_step = display_acc-display_acc%10;
  lc.setDigit(1,1,(display_step/10),false);
  display_acc = display_acc-display_step;

  lc.setDigit(1,0,display_acc,false);

  cmp++;
  if(cmp>30000) {
    cmp=0;
  }
*/
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####


