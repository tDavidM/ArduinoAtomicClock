
// Contains the rountines used to display information on the LED segment display
// Display 0 is on top, display 1 is bellow
// Usually 0 is for current mode in context and 1 is for current time
// Individual char are numbered from 7 to 0, fromt the left to the right
// The refresh rate is about 50ms and mostly controled by the MainClock routine UpdateDisplay()
//   calling intermediate routines in UpdateModes or UserModes that then call the routine here in DisplayModes

// Bit->Segment relation for the bitmask
//  |--2--|
//  7     3
//  |     |
//  ---8---
//  |     |
//  6     4
//  |--5--|  .1.
// 0b12345678

// Display the current Date in the specified Mode(format)
//bool DisplayCurrentDate(int SkipChar, int ForceModeZero)

// Display the current Time in the specified Mode(format)
//bool DisplayCurrentTime(int SkipChar, bool ForceModeZero)

// Display the current AlternateTime (TimeZone) in the specified Mode(format)
//void DisplayTZAltTime(void *)

// Display the current Chrono in the specified Mode(format)
//void DisplayCurrentChrono(void *)

// Display the current ChronoLap in the specified Mode(format)
//void DisplayCurrentLap(void *)

// Compute and display absolute delta value between current lap and the next (main clock if at the last lap)
//void DisplayDeltaNextLap(void *)

// Display the current CountDown in the specified Mode(format)
//bool DisplayCurrentCountDown(int SkipChar, bool ForceModeZero)

// When some DisplayMode are not avaiable under the current LockMode, this is displayed
//void ShowLockScreen(void *)

// Set every single segments to ON at full brightness, used only if the Knob is not correctly set
//void DisplayTestPatern(void *)

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Display the current Date in the specified Mode(format)
bool DisplayCurrentDate(int SkipChar, int ForceModeZero) {
  int DisplayVal[8];

  int display_acc;
  int display_step;

  lc.setIntensity(0,LedIntensity);

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

  // Convert single Int to "Char like" for Month with 2 positions
  display_acc = Month;

  display_step = display_acc-display_acc%10;
  DisplayVal[4] = display_step/10;
  display_acc = display_acc-display_step;

  DisplayVal[5] = display_acc;

  // Convert single Int to "Char like" for Day with 2 positions
  display_acc = Day;

  display_step = display_acc-display_acc%10;
  DisplayVal[6] = display_step/10;
  display_acc = display_acc-display_step;

  DisplayVal[7] = display_acc;

  // Display the "Char like" values in their respective position depending on the display mode
  // In UpdateMode, Mode Zero is forced and one of the position is skiped to display the cursor
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

// Display the current Time in the specified Mode(format)
bool DisplayCurrentTime(int SkipChar, bool ForceModeZero) {
  int DisplayVal[7];
  int DisplayValAlt[2];
  bool IndPM = false;

  int display_acc;
  int display_step;

  lc.setIntensity(1,LedIntensity);

  // Convert single Int to "Char like" for Hour with 2 positions
  display_acc = Hour;

  display_step = display_acc-display_acc%10;
  DisplayVal[0] = display_step/10;
  display_acc = display_acc-display_step;

  DisplayVal[1] = display_acc;

  // If in one of the 12H modes, compute the AM/PM values
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

  // Convert single Int to "Char like" for Minute with 2 positions
  display_acc = Minute;

  display_step = display_acc-display_acc%10;
  DisplayVal[2] = display_step/10;
  display_acc = display_acc-display_step;

  DisplayVal[3] = display_acc;

  // Convert single Int to "Char like" for Second with 2 positions
  display_acc = Second;

  display_step = display_acc-display_acc%10;
  DisplayVal[4] = display_step/10;
  display_acc = display_acc-display_step;

  DisplayVal[5] = display_acc;

  // 1 position for TenthsSecond
  DisplayVal[6] = TenthsSecond;

  // Display the "Char like" values in their respective position depending on the display mode
  // In UpdateMode, Mode Zero is forced and one of the position is skiped to display the cursor
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

// Display the current AlternateTime (TimeZone) in the specified Mode(format)
void DisplayTZAltTime(void *) {
  int DisplayVal[7];
  int DisplayValAlt[2];
  bool IndPM = false;

  int display_acc;
  int display_step;

  lc.setIntensity(0,LedIntensity);

  // Convert single Int to "Char like" for Hour with 2 positions
  display_acc = HourTZAlt;

  display_step = display_acc-display_acc%10;
  DisplayVal[0] = display_step/10;
  display_acc = display_acc-display_step;

  DisplayVal[1] = display_acc;

  // If in one of the 12H modes, compute the AM/PM values
  if (TimeDisplayMode >= 4) {
    if (HourTZAlt == 0)
      display_acc = 12;
    else if (HourTZAlt == 12) {
      IndPM = true;
      display_acc = HourTZAlt;
    } else if (HourTZAlt > 12) {
      IndPM = true;
      display_acc = HourTZAlt - 12;
    } else {
      display_acc = HourTZAlt;
    }

    display_step = display_acc-display_acc%10;
    DisplayValAlt[0] = display_step/10;
    display_acc = display_acc-display_step;

    DisplayValAlt[1] = display_acc;
  }

  // Convert single Int to "Char like" for Minute with 2 positions
  display_acc = MinuteTZAlt;

  display_step = display_acc-display_acc%10;
  DisplayVal[2] = display_step/10;
  display_acc = display_acc-display_step;

  DisplayVal[3] = display_acc;

  // Display the "Char like" values in their respective position depending on the display mode
  if (TimeDisplayMode == 0 ) {
    //HHMI
    lc.setDigit(0,7,DisplayVal[0],false);
    lc.setRow(0,6,SpecChar[DisplayVal[1]+10]);
    lc.setDigit(0,5,DisplayVal[2],false);
    lc.setRow(0,4,SpecChar[DisplayVal[3]+10]);
    lc.setRow(0,3,SpecChar[BLANK]);
    lc.setRow(0,2,SpecChar[BLANK]);
    lc.setRow(0,1,SpecChar[BLANK]);
    lc.setRow(0,0,SpecChar[BLANK]);
  } else if (TimeDisplayMode == 1 || TimeDisplayMode == 2) {
    //HH_MI
    lc.setDigit(0,7,DisplayVal[0],false);
    lc.setDigit(0,6,DisplayVal[1],false);
    lc.setRow(0,5,SpecChar[CH_H]);
    lc.setDigit(0,4,DisplayVal[2],false);
    lc.setDigit(0,3,DisplayVal[3],false);
    lc.setRow(0,2,SpecChar[BLANK]);
    lc.setRow(0,1,SpecChar[BLANK]);
    lc.setRow(0,0,SpecChar[BLANK]);
  } else if (TimeDisplayMode == 3 ) {
    //HH MI
    lc.setDigit(0,7,DisplayVal[0],false);
    lc.setDigit(0,6,DisplayVal[1],false);
    lc.setRow(0,5,SpecChar[BLANK]);
    lc.setDigit(0,4,DisplayVal[2],false);
    lc.setDigit(0,3,DisplayVal[3],false);
    lc.setRow(0,2,SpecChar[BLANK]);
    lc.setRow(0,1,SpecChar[BLANK]);
    lc.setRow(0,0,SpecChar[BLANK]);
  } else if (TimeDisplayMode == 4 || TimeDisplayMode == 5) {
    //HHPMI
    lc.setDigit(0,7,DisplayValAlt[0],false);
    lc.setDigit(0,6,DisplayValAlt[1],false);
    if (IndPM)
      lc.setRow(0,5,SpecChar[CH_P]);
    else
      lc.setRow(0,5,SpecChar[CH_A]);
    lc.setDigit(0,4,DisplayVal[2],false);
    lc.setDigit(0,3,DisplayVal[3],false);
    lc.setRow(0,2,SpecChar[BLANK]);
    lc.setRow(0,1,SpecChar[BLANK]);
    lc.setRow(0,0,SpecChar[BLANK]);
  }
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Display the current Chrono in the specified Mode(format)
void DisplayCurrentChrono(void *) {
  int DisplayVal[8];

  int display_acc;
  int display_step;

  // Convert single Int to "Char like" for Hour with 3 positions
  display_acc = ChronoHour;

  display_step = display_acc-display_acc%100;
  DisplayVal[0] = display_step/100;
  display_acc = display_acc-display_step;

  display_step = display_acc-display_acc%10;
  DisplayVal[1] = display_step/10;
  display_acc = display_acc-display_step;

  DisplayVal[2] = display_acc;

  // Convert single Int to "Char like" for Minute with 2 positions
  display_acc = ChronoMinute;

  display_step = display_acc-display_acc%10;
  DisplayVal[3] = display_step/10;
  display_acc = display_acc-display_step;

  DisplayVal[4] = display_acc;

  // Convert single Int to "Char like" for Second with 2 positions
  display_acc = ChronoSecond;

  display_step = display_acc-display_acc%10;
  DisplayVal[5] = display_step/10;
  display_acc = display_acc-display_step;

  DisplayVal[6] = display_acc;

  // 1 position for TenthsSecond
  DisplayVal[7] = ChronoTenthsSecond;

  // Display the "Char like" values in their respective position depending on the display mode
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

// Display the current ChronoLap in the specified Mode(format)
void DisplayCurrentLap(void *) {
  int DisplayVal[8];

  int display_acc;
  int display_step;

  int Lap = (ChronoLapMode%10)-1;

  // Convert single Int to "Char like" for Hour with 3 positions
  display_acc = ChronoLap[Lap].Hour;

  display_step = display_acc-display_acc%100;
  DisplayVal[0] = display_step/100;
  display_acc = display_acc-display_step;

  display_step = display_acc-display_acc%10;
  DisplayVal[1] = display_step/10;
  display_acc = display_acc-display_step;

  DisplayVal[2] = display_acc;

  // Convert single Int to "Char like" for Minute with 2 positions
  display_acc = ChronoLap[Lap].Minute;

  display_step = display_acc-display_acc%10;
  DisplayVal[3] = display_step/10;
  display_acc = display_acc-display_step;

  DisplayVal[4] = display_acc;

  // Convert single Int to "Char like" for Second with 2 positions
  display_acc = ChronoLap[Lap].Second;

  display_step = display_acc-display_acc%10;
  DisplayVal[5] = display_step/10;
  display_acc = display_acc-display_step;

  DisplayVal[6] = display_acc;

  // 1 position for TenthsSecond
  DisplayVal[7] = ChronoLap[Lap].TenthsSecond;

  // Display the "Char like" values in their respective position depending on the display mode
  if (ChronoDisplayMode == 0) {
    //HHHMISSX
    lc.setDigit(1,7,DisplayVal[0],false);
    lc.setDigit(1,6,DisplayVal[1],false);
    lc.setRow(1,5,SpecChar[DisplayVal[2]+10]);
    lc.setDigit(1,4,DisplayVal[3],false);
    lc.setRow(1,3,SpecChar[DisplayVal[4]+10]);
    lc.setDigit(1,2,DisplayVal[5],false);
    lc.setRow(1,1,SpecChar[DisplayVal[6]+10]);
    lc.setDigit(1,0,DisplayVal[7],false);
  } else if (ChronoDisplayMode == 1 ) {
    //HHHMI_SS
    lc.setDigit(1,7,DisplayVal[0],false);
    lc.setDigit(1,6,DisplayVal[1],false);
    lc.setRow(1,5,SpecChar[DisplayVal[2]+10]);
    lc.setDigit(1,4,DisplayVal[3],false);
    lc.setDigit(1,3,DisplayVal[4],false);
    lc.setRow(1,2,SpecChar[BLANK]);
    lc.setDigit(1,1,DisplayVal[5],false);
    lc.setDigit(1,0,DisplayVal[6],false); 
  }

}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Compute and display absolute delta value between current lap and the next (main clock if at the last lap)
void DisplayDeltaNextLap(void *) {
  int DisplayVal[8];

  int display_acc;
  int display_step;

  int Lap = (ChronoLapMode%10)-1;
  bool MakeSwap = false;

  // _Gr for Greater and _Sm for Smaller
  int DeltaHourGr, DeltaMinuteGr, DeltaSecondGr,
      DeltaHourSm, DeltaMinuteSm, DeltaSecondSm;

  // Init _Gr with next Lap or if on the last, with main clock
  DeltaHourGr   = Lap<8 ? ChronoLap[Lap+1].Hour   : ChronoHour;
  DeltaMinuteGr = Lap<8 ? ChronoLap[Lap+1].Minute : ChronoMinute;
  DeltaSecondGr = Lap<8 ? (ChronoLap[Lap+1].Second*10) + ChronoLap[Lap+1].TenthsSecond :
                          (ChronoSecond*10) + ChronoTenthsSecond; //DeltaSecond combines Seconds and TenthsSecond

  //How about NO !
  //MakeSwap = DeltaHourGr < ChronoLap[Lap].Hour ? true : (DeltaMinuteGr < ChronoLap[Lap].Minute ? true : (DeltaSecondGr < (ChronoLap[Lap].Second*10) + ChronoLap[Lap].TenthsSecond ? true : false))

  // That's better, if _Gr smaller than _Sm, a swap must be made
  if (DeltaHourGr < ChronoLap[Lap].Hour)
    MakeSwap = true;
  else if (DeltaMinuteGr < ChronoLap[Lap].Minute)
    MakeSwap = true;
  else if (DeltaSecondGr < (ChronoLap[Lap].Second*10) + ChronoLap[Lap].TenthsSecond) //DeltaSecond combines Seconds and TenthsSecond
    MakeSwap = true;

  // Swap _Gr to _Sm and init _Gr again
  if (MakeSwap) {
    DeltaHourSm   = DeltaHourGr;
    DeltaMinuteSm = DeltaMinuteGr;
    DeltaSecondSm = DeltaSecondGr;
    DeltaHourGr   = ChronoLap[Lap].Hour;
    DeltaMinuteGr = ChronoLap[Lap].Minute;
    DeltaSecondGr = (ChronoLap[Lap].Second*10) + ChronoLap[Lap].TenthsSecond; //DeltaSecond combines Seconds and TenthsSecond
  } else { // Init _Sm
    DeltaHourSm   = ChronoLap[Lap].Hour;
    DeltaMinuteSm = ChronoLap[Lap].Minute;
    DeltaSecondSm = (ChronoLap[Lap].Second*10) + ChronoLap[Lap].TenthsSecond; //DeltaSecond combines Seconds and TenthsSecond
  } 

  // If not enough Seconds, borrow some from Minute
  if (DeltaSecondGr < DeltaSecondSm) {
    DeltaSecondGr+=600; // 60*10 as DeltaSecond combines Seconds and TenthsSecond
    DeltaMinuteGr--;
  }
  // If not enough Minutes, borrow some from Hour
  if (DeltaMinuteGr < DeltaMinuteSm) {
    DeltaMinuteGr+=60;
    DeltaHourGr--;
  }

  // Substract _Sm from _Gr
  DeltaHourGr-=   DeltaHourSm;
  DeltaMinuteGr-= DeltaMinuteSm;
  DeltaSecondGr-= DeltaSecondSm;

  // Convert single Int to "Char like" for Hour with 3 positions
  display_acc = DeltaHourGr;
  
  display_step = display_acc-display_acc%100;
  DisplayVal[0] = display_step/100;
  display_acc = display_acc-display_step;

  display_step = display_acc-display_acc%10;
  DisplayVal[1] = display_step/10;
  display_acc = display_acc-display_step;

  DisplayVal[2] = display_acc;

  // Convert single Int to "Char like" for Minute with 2 positions
  display_acc = DeltaMinuteGr;

  display_step = display_acc-display_acc%10;
  DisplayVal[3] = display_step/10;
  display_acc = display_acc-display_step;

  DisplayVal[4] = display_acc;

  // Convert single Int to "Char like" for Second with 2 positions
  display_acc = DeltaSecondGr/10; //DeltaSecond combines Seconds and TenthsSecond

  display_step = display_acc-display_acc%10;
  DisplayVal[5] = display_step/10;
  display_acc = display_acc-display_step;

  DisplayVal[6] = display_acc;

  // 1 position for TenthsSecond
  DisplayVal[7] = DeltaSecondGr%10; //DeltaSecond combines Seconds and TenthsSecond

  // Display the "Char like" values in their respective position depending on the display mode
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

// Display the current CountDown in the specified Mode(format)
bool DisplayCurrentCountDown(int SkipChar, bool ForceModeZero) {
  int DisplayVal[8];

  int display_acc;
  int display_step;

  // If the CountDown as reached Zero
  if (IsCountDownDone) {
    if(ToggleCurVal) {
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
    // Convert single Int to "Char like" for Hour with 3 positions
    display_acc = CountDownHour;

    display_step = display_acc-display_acc%100;
    DisplayVal[0] = display_step/100;
    display_acc = display_acc-display_step;

    display_step = display_acc-display_acc%10;
    DisplayVal[1] = display_step/10;
    display_acc = display_acc-display_step;

    DisplayVal[2] = display_acc;

    // Convert single Int to "Char like" for Minute with 2 positions
    display_acc = CountDownMinute;

    display_step = display_acc-display_acc%10;
    DisplayVal[3] = display_step/10;
    display_acc = display_acc-display_step;

    DisplayVal[4] = display_acc;

    // Convert single Int to "Char like" for Second with 3 positions
    display_acc = CountDownSecond;

    display_step = display_acc-display_acc%100;
    DisplayVal[5] = display_step/100;
    display_acc = display_acc-display_step;

    display_step = display_acc-display_acc%10;
    DisplayVal[6] = display_step/10;
    display_acc = display_acc-display_step;

    DisplayVal[7] = display_acc;

    // Display the "Char like" values in their respective position depending on the display mode
    // In UpdateMode, Mode Zero is forced and one of the position is skiped to display the cursor
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

      // Enven if Mode 1 is specified, if there's more than 99 second, the space is filled
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

// When some DisplayMode are not avaiable under the current LockMode, this is displayed
void ShowLockScreen(void *) {
  lc.setIntensity(0,LedIntensity);
  lc.setIntensity(1,LedIntensity);

  lc.clearDisplay(1);

  lc.setChar(0,7,'L',false);
  //lc.setChar(0,6,'0',false);
  lc.setRow(0,6,0b00011101); // o
  lc.setChar(0,5,'c',false);
  //lc.setRow(0,5,0b01001110); // C
  lc.setRow(0,4,0b01010111); // K
  lc.setChar(0,3,'E',false);
  lc.setChar(0,2,'d',false);
  lc.setChar(0,1,' ',false);
  lc.setChar(0,0,' ',false);
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Display the PassCode promt, the unlock prompt, current mode or PassCode fail message depending on the context
void ShowSetLock(void *) {
  int CurPos = 0;

  // If no PassCode set
  if (LockMode == 0) {
    lc.setChar(0,7,'5',false);
    lc.setChar(0,6,'E',false);
    lc.setRow(0,5,0b00001111); // t
    lc.setChar(0,4,' ',false);
    //lc.setChar(0,3,'c',false);
    lc.setRow(0,3,0b01001110); // C
    lc.setChar(0,2,'0',false);
    lc.setChar(0,1,'d',false);
    lc.setChar(0,0,'E',false);
  } else{
    if (Unlocked == 0) {  // When locked, ask for PassCode
      lc.setRow(0,7,0b00001111); // t
      lc.setRow(0,6,0b00111011); // y
      lc.setChar(0,5,'P',false);
      lc.setChar(0,4,'E',false);
      lc.setChar(0,3,' ',false);
      lc.setChar(0,2,'c',false);
      //lc.setRow(0,2,0b01001110); // C
      lc.setChar(0,1,'d',false);
      lc.setChar(0,0,' ',false);
    } else if (Unlocked == 1 ) {  // When unlock, show current Mode
      //lc.setRow(0,7,0b01010100); // M
      lc.setRow(0,7,0b01000110); // M
      lc.setRow(0,6,0b01110010); // M
      lc.setRow(0,5,0b00011101); // o
      lc.setChar(0,4,'d',false);
      lc.setChar(0,3,'E',false);
      lc.setChar(0,2,' ',false);
      lc.setChar(0,1,'0' + LockMode,false);
      lc.setChar(0,0,' ',false);
    } else if (Unlocked == 2 ) {  // If PassCode verif failed
      lc.setChar(0,7,'F',false);
      lc.setChar(0,6,'A',false);
      lc.setChar(0,5,'1',false);
      lc.setChar(0,4,'L',false);
      lc.setChar(0,3,' ',false);
      lc.setChar(0,2,' ',false);
      lc.setChar(0,1,' ',false);
      lc.setChar(0,0,' ',false);
    }
  }

  // Show the cursor and the number of char typed so far
  CurPos = 7 - (TestPassCodeLength / 2);

  for (int i=7; i>=0; i--){
    if (CurPos < i)
      lc.setRow(1,i,0b00001001); // _-
    if (CurPos > i)
      lc.setChar(1,i,' ',false);      
    if (CurPos == i ) {
      if (ToggleCurVal && TestPassCodeLength < 16) 
        lc.setChar(1,CurPos,'_',false);
      else {
        if (TestPassCodeLength % 2 == 1)
          lc.setChar(1,i,'-',false); 
        else
          lc.setChar(1,i,' ',false);     
      }
    }
  }
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Set every single segments to ON at full brightness, used only if the Knob is not correctly set
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
  //lc.setRow(1,7,SpecChar[TEST]);

  digitalWrite(LedTickPin, HIGH);
  digitalWrite(LedLockPin, HIGH);

  //Remove ASCII values left from UpdateModes
  if (ToggleCurChar <= 0x7F) {
    ToggleCurChar = 0b10000000; // ' .'
  }

  // React to a new Keypress
  if (NewKeyPress) {
    NewKeyPress = false; // Consume the Keypress

    switch (KeyPressVal) {
      case 'U':
        ToggleCurChar = 0b10111110; //'U.';
        break;
      case 'R':
        ToggleCurChar = 0b10000101; // 'r.'
        break;
      case 'D':
        ToggleCurChar = 0b10111101; //'d.';
        break;
      case 'L':
        ToggleCurChar = 0b10001110; //'L.';
        break;
      case 'C':
        ToggleCurChar = 0b11001110; //'C.';
        break;
      case 'E':
        ToggleCurChar = 0b11001111; //'E.';
        break;
      default: 
        ToggleCurChar = SpecChar[(KeyPressVal - '0') + 10]; // N.
    }

  }

  // Display the current state
  if(ToggleCurVal)
    lc.setRow(1,7,ToggleCurChar);
  else
    lc.setRow(1,7,SpecChar[TEST]);

}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Debug/Test...Now unused
void UserShowCounter(void *) {
/*
  int display_acc = cmp;
  int display_step;
*/

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
