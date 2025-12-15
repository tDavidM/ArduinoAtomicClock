
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

// Display the current Date in the specified Mode(format) (Zero=ISO 8601)
//bool DisplayCurrentDate(int SkipChar, int ForceModeZero)

// Display the current Time in the specified Mode(format) (Zero=ISO 8601)
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

//enum LCDUpDown, keys, displayModes

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Display the current Date in the specified Mode(format) (Zero=ISO 8601)
bool DisplayCurrentDate(int SkipChar, bool ForceModeZero) {
  int DisplayVal[8]; // stores YYYYMMDD

  int display_acc;
  int display_step;

  lc.setIntensity(UPPERLEDROW, LedIntensity);

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
  if (DateDisplayMode == DM_YYYYMMDD || ForceModeZero) {
    //YYYYMMDD
    if (SkipChar != LEDCOL0)
      lc.setDigit(UPPERLEDROW, LEDCOL0, DisplayVal[0], false);
    else
      ToggleCurChar = DisplayVal[0] + '0';

    if (SkipChar != LEDCOL1)
      lc.setDigit(UPPERLEDROW, LEDCOL1, DisplayVal[1], false);
    else
      ToggleCurChar = DisplayVal[1] + '0';

    if (SkipChar != LEDCOL2)
      lc.setDigit(UPPERLEDROW, LEDCOL2, DisplayVal[2], false);
    else
      ToggleCurChar = DisplayVal[2] + '0';

    if (SkipChar != LEDCOL3)
      lc.setRow(UPPERLEDROW, LEDCOL3, SpecChar[DisplayVal[3]+CHR_DOTOFFET]);
    else
      ToggleCurChar = DisplayVal[3] + '0';


    if (SkipChar != LEDCOL4)
      lc.setDigit(UPPERLEDROW, LEDCOL4, DisplayVal[4], false);
    else
      ToggleCurChar = DisplayVal[4] + '0';

    if (SkipChar != LEDCOL5)
      lc.setRow(UPPERLEDROW, LEDCOL5, SpecChar[DisplayVal[5]+CHR_DOTOFFET]);
    else
      ToggleCurChar = DisplayVal[5] + '0';

    if (SkipChar != LEDCOL6)
      lc.setDigit(UPPERLEDROW, LEDCOL6, DisplayVal[6], false);
    else
      ToggleCurChar = DisplayVal[6] + '0';

    if (SkipChar != LEDCOL7)
      lc.setDigit(UPPERLEDROW, LEDCOL7, DisplayVal[7], false);
    else
      ToggleCurChar = DisplayVal[7] + '0';
  } else if (DateDisplayMode == DM_YYMMDD ) {
    //YY-MM-DD
    lc.setDigit(UPPERLEDROW, LEDCOL0, DisplayVal[2], false);
    lc.setDigit(UPPERLEDROW, LEDCOL1, DisplayVal[3], false);
    lc.setRow(UPPERLEDROW,   LEDCOL2, SpecChar[CHR_BAR]);
    lc.setDigit(UPPERLEDROW, LEDCOL3, DisplayVal[4], false);
    lc.setDigit(UPPERLEDROW, LEDCOL4, DisplayVal[5], false);
    lc.setRow(UPPERLEDROW,   LEDCOL5, SpecChar[CHR_BAR]);
    lc.setDigit(UPPERLEDROW, LEDCOL6, DisplayVal[6], false);
    lc.setDigit(UPPERLEDROW, LEDCOL7, DisplayVal[7], false);
  } else if (DateDisplayMode == DM_YY_MM_DD ) {
    //YY MM DD
    lc.setDigit(UPPERLEDROW, LEDCOL0, DisplayVal[2], false);
    lc.setDigit(UPPERLEDROW, LEDCOL1, DisplayVal[3], false);
    lc.setRow(UPPERLEDROW,   LEDCOL2, SpecChar[CHR_BLANK]);
    lc.setDigit(UPPERLEDROW, LEDCOL3, DisplayVal[4], false);
    lc.setDigit(UPPERLEDROW, LEDCOL4, DisplayVal[5], false);
    lc.setRow(UPPERLEDROW,   LEDCOL5, SpecChar[CHR_BLANK]);
    lc.setDigit(UPPERLEDROW, LEDCOL6, DisplayVal[6], false);
    lc.setDigit(UPPERLEDROW, LEDCOL7, DisplayVal[7], false);
  } else if (DateDisplayMode == DM_MMDDYYYY ) {
    lc.setDigit(UPPERLEDROW, LEDCOL0, DisplayVal[4], false);
    lc.setRow(UPPERLEDROW,   LEDCOL1, SpecChar[DisplayVal[5]+CHR_DOTOFFET]);
    lc.setDigit(UPPERLEDROW, LEDCOL2, DisplayVal[6], false);
    lc.setRow(UPPERLEDROW,   LEDCOL3, SpecChar[DisplayVal[7]+CHR_DOTOFFET]);
    lc.setDigit(UPPERLEDROW, LEDCOL4, DisplayVal[0], false);
    lc.setDigit(UPPERLEDROW, LEDCOL5, DisplayVal[1], false);
    lc.setDigit(UPPERLEDROW, LEDCOL6, DisplayVal[2], false);
    lc.setDigit(UPPERLEDROW, LEDCOL7, DisplayVal[3], false);
  } else if (DateDisplayMode == DM_DDMMYYYY ) {
    lc.setDigit(UPPERLEDROW, LEDCOL0, DisplayVal[6], false);
    lc.setRow(UPPERLEDROW,   LEDCOL1, SpecChar[DisplayVal[7]+CHR_DOTOFFET]);
    lc.setDigit(UPPERLEDROW, LEDCOL2, DisplayVal[4], false);
    lc.setRow(UPPERLEDROW,   LEDCOL3, SpecChar[DisplayVal[5]+CHR_DOTOFFET]);
    lc.setDigit(UPPERLEDROW, LEDCOL4, DisplayVal[0], false);
    lc.setDigit(UPPERLEDROW, LEDCOL5, DisplayVal[1], false);
    lc.setDigit(UPPERLEDROW, LEDCOL6, DisplayVal[2], false);
    lc.setDigit(UPPERLEDROW, LEDCOL7, DisplayVal[3], false);
  } else if (DateDisplayMode == DM_YYYYDDMM ) {
    lc.setDigit(UPPERLEDROW, LEDCOL0, DisplayVal[0], false);
    lc.setDigit(UPPERLEDROW, LEDCOL1, DisplayVal[1], false);
    lc.setDigit(UPPERLEDROW, LEDCOL2, DisplayVal[2], false);
    lc.setRow(UPPERLEDROW,   LEDCOL3, SpecChar[DisplayVal[3]+CHR_DOTOFFET]);
    lc.setDigit(UPPERLEDROW, LEDCOL4, DisplayVal[6], false);
    lc.setRow(UPPERLEDROW,   LEDCOL5, SpecChar[DisplayVal[7]+CHR_DOTOFFET]);
    lc.setDigit(UPPERLEDROW, LEDCOL6, DisplayVal[4], false);
    lc.setDigit(UPPERLEDROW, LEDCOL7, DisplayVal[5], false);
  }

  return true;
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Display the current Time in the specified Mode(format) (Zero=ISO 8601)
bool DisplayCurrentTime(int SkipChar, bool ForceModeZero) {
  int DisplayVal[7];    // stores HHMMSSX
  int DisplayValAlt[2]; // stores HH for AM or HH-12 for PM
  bool IndPM = false;

  int display_acc;
  int display_step;

  lc.setIntensity(LOWERLEDROW, LedIntensity);

  // Convert single Int to "Char like" for Hour with 2 positions
  display_acc = Hour;

  display_step = display_acc-display_acc%10;
  DisplayVal[0] = display_step/10;
  display_acc = display_acc-display_step;

  DisplayVal[1] = display_acc;

  // If in one of the 12H modes, compute the AM/PM values
  if (TimeDisplayMode == TM_HHPMISSX || TimeDisplayMode == TM_HHPMI_SS) {
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
  if (TimeDisplayMode == TM_HHMISS_X || ForceModeZero) {
    //HHMISS X
    if (SkipChar != LEDCOL0)
      lc.setDigit(LOWERLEDROW, LEDCOL0, DisplayVal[0], false);
    else
      ToggleCurChar = DisplayVal[0] + '0';

    if (SkipChar != LEDCOL1)
      lc.setRow(LOWERLEDROW, LEDCOL1, SpecChar[DisplayVal[1]+CHR_DOTOFFET]);
    else
      ToggleCurChar = DisplayVal[1] + '0';


    if (SkipChar != LEDCOL2)
      lc.setDigit(LOWERLEDROW, LEDCOL2, DisplayVal[2], false);
    else
      ToggleCurChar = DisplayVal[2] + '0';

    if (SkipChar != LEDCOL3)
      lc.setRow(LOWERLEDROW, LEDCOL3, SpecChar[DisplayVal[3]+CHR_DOTOFFET]);
    else
      ToggleCurChar = DisplayVal[3] + '0';

    lc.setDigit(LOWERLEDROW, LEDCOL4, DisplayVal[4], false);
    //lc.setRow(LOWERLEDROW, LEDCOL5, SpecChar[DisplayVal[5]+CHR_DOTOFFET]);
    lc.setDigit(LOWERLEDROW, LEDCOL5, DisplayVal[5], false);

    lc.setRow(LOWERLEDROW,   LEDCOL6, SpecChar[CHR_BLANK]);
    lc.setDigit(LOWERLEDROW, LEDCOL7, DisplayVal[6], false);
  } else if (TimeDisplayMode == TM_HH_MISSX ) {
    //HH_MISSX
    lc.setDigit(LOWERLEDROW, LEDCOL0, DisplayVal[0], false);
    lc.setDigit(LOWERLEDROW, LEDCOL1, DisplayVal[1], false);
    lc.setRow(LOWERLEDROW,   LEDCOL2, SpecChar[CHR_H]);
    lc.setDigit(LOWERLEDROW, LEDCOL3, DisplayVal[2], false);
    lc.setRow(LOWERLEDROW,   LEDCOL4, SpecChar[DisplayVal[3]+CHR_DOTOFFET]);
    lc.setDigit(LOWERLEDROW, LEDCOL5, DisplayVal[4], false);
    lc.setRow(LOWERLEDROW,   LEDCOL6, SpecChar[DisplayVal[5]+CHR_DOTOFFET]);
    lc.setDigit(LOWERLEDROW, LEDCOL7, DisplayVal[6], false);
  } else if (TimeDisplayMode == TM_HHMI_SS ) {
    //HH_MI SS
    lc.setDigit(LOWERLEDROW, LEDCOL0, DisplayVal[0], false);
    lc.setDigit(LOWERLEDROW, LEDCOL1, DisplayVal[1], false);
    lc.setRow(LOWERLEDROW,   LEDCOL2, SpecChar[CHR_H]);
    lc.setDigit(LOWERLEDROW, LEDCOL3, DisplayVal[2], false);
    lc.setDigit(LOWERLEDROW, LEDCOL4, DisplayVal[3], false);
    lc.setRow(LOWERLEDROW,   LEDCOL5, SpecChar[CHR_BLANK]);
    lc.setDigit(LOWERLEDROW, LEDCOL6, DisplayVal[4], false);
    lc.setDigit(LOWERLEDROW, LEDCOL7, DisplayVal[5], false);
  } else if (TimeDisplayMode == TM_HH_MI_SS ) {
    //HH MI SS
    lc.setDigit(LOWERLEDROW, LEDCOL0, DisplayVal[0], false);
    lc.setDigit(LOWERLEDROW, LEDCOL1, DisplayVal[1], false);
    lc.setRow(LOWERLEDROW,   LEDCOL2, SpecChar[CHR_BLANK]);
    lc.setDigit(LOWERLEDROW, LEDCOL3, DisplayVal[2], false);
    lc.setDigit(LOWERLEDROW, LEDCOL4, DisplayVal[3], false);
    lc.setRow(LOWERLEDROW,   LEDCOL5, SpecChar[CHR_BLANK]);
    lc.setDigit(LOWERLEDROW, LEDCOL6, DisplayVal[4], false);
    lc.setDigit(LOWERLEDROW, LEDCOL7, DisplayVal[5], false);
  } else if (TimeDisplayMode == TM_HHPMISSX ) {
    //HHPMISSX
    lc.setDigit(LOWERLEDROW, LEDCOL0, DisplayValAlt[0], false);
    lc.setDigit(LOWERLEDROW, LEDCOL1, DisplayValAlt[1], false);
    if (IndPM)
      lc.setRow(LOWERLEDROW, LEDCOL2, SpecChar[CHR_P]);
    else
      lc.setRow(LOWERLEDROW, LEDCOL2, SpecChar[CHR_A]);
    lc.setDigit(LOWERLEDROW, LEDCOL3, DisplayVal[2], false);
    lc.setDigit(LOWERLEDROW, LEDCOL4, DisplayVal[3], false);
    lc.setRow(LOWERLEDROW,   LEDCOL5, SpecChar[CHR_BLANK]);
    lc.setDigit(LOWERLEDROW, LEDCOL6, DisplayVal[4], false);
    lc.setDigit(LOWERLEDROW, LEDCOL7, DisplayVal[5], false);
  } else if (TimeDisplayMode == TM_HHPMI_SS ) {
    //HHPMI SS
    lc.setDigit(LOWERLEDROW, LEDCOL0, DisplayValAlt[0], false);
    lc.setDigit(LOWERLEDROW, LEDCOL1, DisplayValAlt[1], false);
    if (IndPM)
      lc.setRow(LOWERLEDROW, LEDCOL2, SpecChar[CHR_P]);
    else
      lc.setRow(LOWERLEDROW, LEDCOL2, SpecChar[CHR_A]);
    lc.setDigit(LOWERLEDROW, LEDCOL3, DisplayVal[2], false);
    lc.setRow(LOWERLEDROW,   LEDCOL4, SpecChar[DisplayVal[3]+CHR_DOTOFFET]);
    lc.setDigit(LOWERLEDROW, LEDCOL5, DisplayVal[4], false);
    lc.setRow(LOWERLEDROW,   LEDCOL6, SpecChar[DisplayVal[5]+CHR_DOTOFFET]);
    lc.setDigit(LOWERLEDROW, LEDCOL7, DisplayVal[6], false);
  }

  return true;
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Display the current AlternateTime (TimeZone) in the specified Mode(format)
void DisplayTZAltTime(void *) {
  int DisplayVal[4];    // stores HHMM
  int DisplayValAlt[2]; // stores HH for AM or HH-12 for PM
  bool IndPM = false;

  int display_acc;
  int display_step;

  lc.setIntensity(UPPERLEDROW, LedIntensity);

  // Convert single Int to "Char like" for Hour with 2 positions
  display_acc = HourTZAlt;

  display_step = display_acc-display_acc%10;
  DisplayVal[0] = display_step/10;
  display_acc = display_acc-display_step;

  DisplayVal[1] = display_acc;

  // If in one of the 12H modes, compute the AM/PM values
  if (TimeDisplayMode == TM_HHPMISSX || TimeDisplayMode == TM_HHPMI_SS) {
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
  if (TimeDisplayMode == TM_HHMISS_X ) {
    //HHMI
    lc.setDigit(UPPERLEDROW, LEDCOL0, DisplayVal[0], false);
    lc.setRow(UPPERLEDROW,   LEDCOL1, SpecChar[DisplayVal[1]+CHR_DOTOFFET]);
    lc.setDigit(UPPERLEDROW, LEDCOL2, DisplayVal[2], false);
    lc.setRow(UPPERLEDROW,   LEDCOL3, SpecChar[DisplayVal[3]+CHR_DOTOFFET]);
    lc.setRow(UPPERLEDROW,   LEDCOL4, SpecChar[CHR_BLANK]);
    lc.setRow(UPPERLEDROW,   LEDCOL5, SpecChar[CHR_BLANK]);
    lc.setRow(UPPERLEDROW,   LEDCOL6, SpecChar[CHR_BLANK]);
    lc.setRow(UPPERLEDROW,   LEDCOL7, SpecChar[CHR_BLANK]);
  } else if (TimeDisplayMode == TM_HH_MISSX || TimeDisplayMode == TM_HHMI_SS) {
    //HH_MI
    lc.setDigit(UPPERLEDROW, LEDCOL0, DisplayVal[0], false);
    lc.setDigit(UPPERLEDROW, LEDCOL1, DisplayVal[1], false);
    lc.setRow(UPPERLEDROW,   LEDCOL2, SpecChar[CHR_H]);
    lc.setDigit(UPPERLEDROW, LEDCOL3, DisplayVal[2], false);
    lc.setDigit(UPPERLEDROW, LEDCOL4, DisplayVal[3], false);
    lc.setRow(UPPERLEDROW,   LEDCOL5, SpecChar[CHR_BLANK]);
    lc.setRow(UPPERLEDROW,   LEDCOL6, SpecChar[CHR_BLANK]);
    lc.setRow(UPPERLEDROW,   LEDCOL7, SpecChar[CHR_BLANK]);
  } else if (TimeDisplayMode == TM_HH_MI_SS ) {
    //HH MI
    lc.setDigit(UPPERLEDROW, LEDCOL0, DisplayVal[0], false);
    lc.setDigit(UPPERLEDROW, LEDCOL1, DisplayVal[1], false);
    lc.setRow(UPPERLEDROW,   LEDCOL2, SpecChar[CHR_BLANK]);
    lc.setDigit(UPPERLEDROW, LEDCOL3, DisplayVal[2], false);
    lc.setDigit(UPPERLEDROW, LEDCOL4, DisplayVal[3], false);
    lc.setRow(UPPERLEDROW,   LEDCOL5, SpecChar[CHR_BLANK]);
    lc.setRow(UPPERLEDROW,   LEDCOL6, SpecChar[CHR_BLANK]);
    lc.setRow(UPPERLEDROW,   LEDCOL7, SpecChar[CHR_BLANK]);
  } else if (TimeDisplayMode == TM_HHPMISSX || TimeDisplayMode == TM_HHPMI_SS) {
    //HHPMI
    lc.setDigit(UPPERLEDROW, LEDCOL0, DisplayValAlt[0], false);
    lc.setDigit(UPPERLEDROW, LEDCOL1, DisplayValAlt[1], false);
    if (IndPM)
      lc.setRow(UPPERLEDROW, LEDCOL2, SpecChar[CHR_P]);
    else
      lc.setRow(UPPERLEDROW, LEDCOL2, SpecChar[CHR_A]);
    lc.setDigit(UPPERLEDROW, LEDCOL3, DisplayVal[2], false);
    lc.setDigit(UPPERLEDROW, LEDCOL4, DisplayVal[3], false);
    lc.setRow(UPPERLEDROW,   LEDCOL5, SpecChar[CHR_BLANK]);
    lc.setRow(UPPERLEDROW,   LEDCOL6, SpecChar[CHR_BLANK]);
    lc.setRow(UPPERLEDROW,   LEDCOL7, SpecChar[CHR_BLANK]);
  }
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Display the current Chrono in the specified Mode(format)
void DisplayCurrentChrono(void *) {
  int DisplayVal[8]; // Stores HHHMISSX

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
  if (ChronoDisplayMode == CM_HHHMISSX) {
    //HHHMISSX
    lc.setDigit(UPPERLEDROW, LEDCOL0, DisplayVal[0], false);
    lc.setDigit(UPPERLEDROW, LEDCOL1, DisplayVal[1], false);
    lc.setRow(UPPERLEDROW,   LEDCOL2, SpecChar[DisplayVal[2]+CHR_DOTOFFET]);
    lc.setDigit(UPPERLEDROW, LEDCOL3, DisplayVal[3], false);
    lc.setRow(UPPERLEDROW,   LEDCOL4, SpecChar[DisplayVal[4]+CHR_DOTOFFET]);
    lc.setDigit(UPPERLEDROW, LEDCOL5, DisplayVal[5], false);
    lc.setRow(UPPERLEDROW,   LEDCOL6, SpecChar[DisplayVal[6]+CHR_DOTOFFET]);
    lc.setDigit(UPPERLEDROW, LEDCOL7, DisplayVal[7], false);
  } else if (ChronoDisplayMode == CM_HHHMI_SS ) {
    //HHHMI_SS
    lc.setDigit(UPPERLEDROW, LEDCOL0, DisplayVal[0], false);
    lc.setDigit(UPPERLEDROW, LEDCOL1, DisplayVal[1], false);
    lc.setRow(UPPERLEDROW,   LEDCOL2, SpecChar[DisplayVal[2]+CHR_DOTOFFET]);
    lc.setDigit(UPPERLEDROW, LEDCOL3, DisplayVal[3], false);
    lc.setDigit(UPPERLEDROW, LEDCOL4, DisplayVal[4], false);
    lc.setRow(UPPERLEDROW,   LEDCOL5, SpecChar[CHR_BLANK]);
    lc.setDigit(UPPERLEDROW, LEDCOL6, DisplayVal[5], false);
    lc.setDigit(UPPERLEDROW, LEDCOL7, DisplayVal[6], false); 
  }

}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Display the current ChronoLap in the specified Mode(format)
void DisplayCurrentLap(void *) {
  int DisplayVal[8]; // Stores HHHMISSX

  int display_acc;
  int display_step;

  int Lap = (ChronoLapMode%LAP_DELTA)-1;

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
  if (ChronoDisplayMode == CM_HHHMISSX) {
    //HHHMISSX
    lc.setDigit(LOWERLEDROW, LEDCOL0, DisplayVal[0], false);
    lc.setDigit(LOWERLEDROW, LEDCOL1, DisplayVal[1], false);
    lc.setRow(LOWERLEDROW,   LEDCOL2, SpecChar[DisplayVal[2]+CHR_DOTOFFET]);
    lc.setDigit(LOWERLEDROW, LEDCOL3, DisplayVal[3], false);
    lc.setRow(LOWERLEDROW,   LEDCOL4, SpecChar[DisplayVal[4]+CHR_DOTOFFET]);
    lc.setDigit(LOWERLEDROW, LEDCOL5, DisplayVal[5], false);
    lc.setRow(LOWERLEDROW,   LEDCOL6, SpecChar[DisplayVal[6]+CHR_DOTOFFET]);
    lc.setDigit(LOWERLEDROW, LEDCOL7, DisplayVal[7], false);
  } else if (ChronoDisplayMode == CM_HHHMI_SS ) {
    //HHHMI_SS
    lc.setDigit(LOWERLEDROW, LEDCOL0, DisplayVal[0], false);
    lc.setDigit(LOWERLEDROW, LEDCOL1, DisplayVal[1], false);
    lc.setRow(LOWERLEDROW,   LEDCOL2, SpecChar[DisplayVal[2]+CHR_DOTOFFET]);
    lc.setDigit(LOWERLEDROW, LEDCOL3, DisplayVal[3], false);
    lc.setDigit(LOWERLEDROW, LEDCOL4, DisplayVal[4], false);
    lc.setRow(LOWERLEDROW,   LEDCOL5, SpecChar[CHR_BLANK]);
    lc.setDigit(LOWERLEDROW, LEDCOL6, DisplayVal[5], false);
    lc.setDigit(LOWERLEDROW, LEDCOL7, DisplayVal[6], false); 
  }

}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Compute and display absolute delta value between current lap and the next (main clock if at the last lap)
void DisplayDeltaNextLap(void *) {
  int DisplayVal[8]; // Stores HHHMISSX

  int display_acc;
  int display_step;

  int Lap = (ChronoLapMode%LAP_DELTA)-1;
  bool MakeSwap = false;

  // _Gr for Greater and _Sm for Smaller
  int DeltaHourGr, DeltaMinuteGr, DeltaSecondGr,
      DeltaHourSm, DeltaMinuteSm, DeltaSecondSm;

  // Init _Gr with next Lap or if on the last, with main clock
  DeltaHourGr   = Lap<8 ? ChronoLap[Lap+1].Hour   : ChronoHour;
  DeltaMinuteGr = Lap<8 ? ChronoLap[Lap+1].Minute : ChronoMinute;
  DeltaSecondGr = Lap<8 ? (ChronoLap[Lap+1].Second*10) + ChronoLap[Lap+1].TenthsSecond :
                          (ChronoSecond*10) + ChronoTenthsSecond; //DeltaSecond combines Seconds and TenthsSecond

  // That's better, if _Gr smaller than _Sm, a swap must be made
  if (DeltaSecondGr < (ChronoLap[Lap].Second*10) + ChronoLap[Lap].TenthsSecond) //DeltaSecond combines Seconds and TenthsSecond
    MakeSwap = true;

  if (DeltaMinuteGr < ChronoLap[Lap].Minute)
    MakeSwap = true;
  else if(DeltaMinuteGr > ChronoLap[Lap].Minute)
    MakeSwap = false;

  if (DeltaHourGr < ChronoLap[Lap].Hour)
    MakeSwap = true;
  else if (DeltaHourGr > ChronoLap[Lap].Hour)
    MakeSwap = false;

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
  if (ChronoDisplayMode == CM_HHHMISSX) {
    //HHHMISSX
    lc.setDigit(UPPERLEDROW, LEDCOL0, DisplayVal[0], false);
    lc.setDigit(UPPERLEDROW, LEDCOL1, DisplayVal[1], false);
    lc.setRow(UPPERLEDROW,   LEDCOL2, SpecChar[DisplayVal[2]+CHR_DOTOFFET]);
    lc.setDigit(UPPERLEDROW, LEDCOL3, DisplayVal[3], false);
    lc.setRow(UPPERLEDROW,   LEDCOL4, SpecChar[DisplayVal[4]+CHR_DOTOFFET]);
    lc.setDigit(UPPERLEDROW, LEDCOL5, DisplayVal[5], false);
    lc.setRow(UPPERLEDROW,   LEDCOL6, SpecChar[DisplayVal[6]+CHR_DOTOFFET]);
    lc.setDigit(UPPERLEDROW, LEDCOL7, DisplayVal[7], false);
  } else if (ChronoDisplayMode == CM_HHHMI_SS) {
    //HHHMI_SS
    lc.setDigit(UPPERLEDROW, LEDCOL0, DisplayVal[0], false);
    lc.setDigit(UPPERLEDROW, LEDCOL1, DisplayVal[1], false);
    lc.setRow(UPPERLEDROW,   LEDCOL2, SpecChar[DisplayVal[2]+CHR_DOTOFFET]);
    lc.setDigit(UPPERLEDROW, LEDCOL3, DisplayVal[3], false);
    lc.setDigit(UPPERLEDROW, LEDCOL4, DisplayVal[4], false);
    lc.setRow(UPPERLEDROW,   LEDCOL5, SpecChar[CHR_BLANK]);
    lc.setDigit(UPPERLEDROW, LEDCOL6, DisplayVal[5], false);
    lc.setDigit(UPPERLEDROW, LEDCOL7, DisplayVal[6], false); 
  }

}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Display the current CountDown in the specified Mode(format)
bool DisplayCurrentCountDown(int SkipChar, bool ForceModeZero) {
  int DisplayVal[8]; // Stores HHHMISSS

  int display_acc;
  int display_step;

  // If the CountDown as reached Zero
  if (IsCountDownDone) {
    if(ToggleCurVal) {
      lc.setChar(UPPERLEDROW, LEDCOL0, '_', false);
      lc.setChar(UPPERLEDROW, LEDCOL1, '_', false);
      lc.setChar(UPPERLEDROW, LEDCOL2, '_', false);
      lc.setChar(UPPERLEDROW, LEDCOL3, '_', false);
      lc.setChar(UPPERLEDROW, LEDCOL4, '_', false);
      lc.setChar(UPPERLEDROW, LEDCOL5, '_', false);
      lc.setChar(UPPERLEDROW, LEDCOL6, '_', false);
      lc.setChar(UPPERLEDROW, LEDCOL7, '_', false);
    } else {
      lc.setRow(UPPERLEDROW, LEDCOL0, SpecChar[10]); // zero with a dot
      lc.setRow(UPPERLEDROW, LEDCOL1, SpecChar[10]);
      lc.setRow(UPPERLEDROW, LEDCOL2, SpecChar[10]);
      lc.setRow(UPPERLEDROW, LEDCOL3, SpecChar[10]);
      lc.setRow(UPPERLEDROW, LEDCOL4, SpecChar[10]);
      lc.setRow(UPPERLEDROW, LEDCOL5, SpecChar[10]);
      lc.setRow(UPPERLEDROW, LEDCOL6, SpecChar[10]);
      lc.setRow(UPPERLEDROW, LEDCOL7, SpecChar[10]);
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
    if (CountDownDisplayMode == CDM_HHHMISSS || ForceModeZero) {
      //HHHMISSS
      if (SkipChar != LEDCOL0)
        lc.setDigit(UPPERLEDROW, LEDCOL0, DisplayVal[0], false);
      else
        ToggleCurChar = DisplayVal[0] + '0';

      if (SkipChar != LEDCOL1)
        lc.setDigit(UPPERLEDROW, LEDCOL1, DisplayVal[1], false);
      else
        ToggleCurChar = DisplayVal[1] + '0';

      if (SkipChar != LEDCOL2)
        lc.setRow(UPPERLEDROW, LEDCOL2, SpecChar[DisplayVal[2]+CHR_DOTOFFET]);
      else
        ToggleCurChar = DisplayVal[2] + '0';

      if (SkipChar != LEDCOL3)
        lc.setDigit(UPPERLEDROW, LEDCOL3, DisplayVal[3], false);
      else
        ToggleCurChar = DisplayVal[3] + '0';

      if (SkipChar != LEDCOL4)
        lc.setRow(UPPERLEDROW, LEDCOL4, SpecChar[DisplayVal[4]+CHR_DOTOFFET]);
      else
        ToggleCurChar = DisplayVal[4] + '0';

      if (SkipChar != LEDCOL5)
        lc.setDigit(UPPERLEDROW, LEDCOL5, DisplayVal[5], false);
      else
        ToggleCurChar = DisplayVal[5] + '0';

      if (SkipChar != LEDCOL6)
        lc.setDigit(UPPERLEDROW, LEDCOL6, DisplayVal[6], false);
      else
        ToggleCurChar = DisplayVal[6] + '0';
      
      if (SkipChar != LEDCOL7)
        lc.setDigit(UPPERLEDROW, LEDCOL7, DisplayVal[7], false);
      else
        ToggleCurChar = DisplayVal[7] + '0';

    } else if (CountDownDisplayMode == CDM_HHHMI_SS ) {
      //HHHMI_SS
      lc.setDigit(UPPERLEDROW, LEDCOL0, DisplayVal[0], false);
      lc.setDigit(UPPERLEDROW, LEDCOL1, DisplayVal[1], false);
      lc.setRow(UPPERLEDROW,   LEDCOL2, SpecChar[DisplayVal[2]+CHR_DOTOFFET]);
      lc.setDigit(UPPERLEDROW, LEDCOL3, DisplayVal[3], false);

      // Enven if Mode 1 is specified, if there's more than 99 second, the space is filled
      if (DisplayVal[5] == 0) {
        lc.setDigit(UPPERLEDROW, LEDCOL4, DisplayVal[4],false);
        lc.setRow(UPPERLEDROW,   LEDCOL5, SpecChar[CHR_BLANK]);
      } else {
        lc.setRow(UPPERLEDROW,   LEDCOL4, SpecChar[DisplayVal[4]+CHR_DOTOFFET]);
        lc.setDigit(UPPERLEDROW, LEDCOL5, DisplayVal[5], false);
      }

      lc.setDigit(UPPERLEDROW, LEDCOL6, DisplayVal[6], false); 
      lc.setDigit(UPPERLEDROW, LEDCOL7, DisplayVal[7], false);
    }
  }

  return true;
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// When some DisplayMode are not avaiable under the current LockMode, this is displayed
void ShowLockScreen(void *) {
  lc.setIntensity(UPPERLEDROW, LedIntensity);
  lc.setIntensity(LOWERLEDROW, LedIntensity);

  lc.clearDisplay(LOWERLEDROW);

  lc.setChar(UPPERLEDROW, LEDCOL0, 'L', false);
  lc.setRow(UPPERLEDROW,  LEDCOL1, SpecChar[CHR_o]); // o
  lc.setChar(UPPERLEDROW, LEDCOL2, 'c', false);
  lc.setRow(UPPERLEDROW,  LEDCOL3, SpecChar[CHR_k]); // K
  lc.setChar(UPPERLEDROW, LEDCOL4, 'E', false);
  lc.setChar(UPPERLEDROW, LEDCOL5, 'd', false);
  lc.setChar(UPPERLEDROW, LEDCOL6, ' ', false);
  lc.setChar(UPPERLEDROW, LEDCOL7, ' ', false);
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Display the PassCode promt, the unlock prompt, current mode or PassCode fail message depending on the context
void ShowSetLock(void *) {
  int CurPos = 0;

  // If no PassCode set
  if (LockMode == LM_NONE) {
    lc.setChar(UPPERLEDROW, LEDCOL0, '5', false);
    lc.setChar(UPPERLEDROW, LEDCOL1, 'E', false);
    lc.setRow(UPPERLEDROW,  LEDCOL2, SpecChar[CHR_t]); // t
    lc.setChar(UPPERLEDROW, LEDCOL3, ' ', false);
    lc.setRow(UPPERLEDROW,  LEDCOL4, SpecChar[CHR_C]); // C
    lc.setChar(UPPERLEDROW, LEDCOL5, '0', false);
    lc.setChar(UPPERLEDROW, LEDCOL6, 'd', false);
    lc.setChar(UPPERLEDROW, LEDCOL7, 'E', false);
  } else{
    if (Unlocked == UM_LOCKED) {  // When locked, ask for PassCode
      lc.setRow(UPPERLEDROW,  LEDCOL0, SpecChar[CHR_t]); // t
      lc.setRow(UPPERLEDROW,  LEDCOL1, SpecChar[CHR_y]); // y
      lc.setChar(UPPERLEDROW, LEDCOL2, 'P', false);
      lc.setChar(UPPERLEDROW, LEDCOL3, 'E', false);
      lc.setChar(UPPERLEDROW, LEDCOL4, ' ', false);
      //lc.setChar(UPPERLEDROW, LEDCOL5, 'c', false);
      lc.setRow(UPPERLEDROW, LEDCOL5, SpecChar[CHR_C]); // C
      lc.setChar(UPPERLEDROW, LEDCOL6, 'd', false);
      lc.setChar(UPPERLEDROW, LEDCOL7, ' ', false);
    } else if (Unlocked == UM_UNLOCKED ) {  // When unlocked, show current Mode
      lc.setRow(UPPERLEDROW,  LEDCOL0, SpecChar[CHR_M]);   // M
      lc.setRow(UPPERLEDROW,  LEDCOL1, SpecChar[CHR_M+1]); // M
      lc.setRow(UPPERLEDROW,  LEDCOL2, SpecChar[CHR_o]);   // o
      lc.setChar(UPPERLEDROW, LEDCOL3, 'd', false);
      lc.setChar(UPPERLEDROW, LEDCOL4, 'E', false);
      lc.setChar(UPPERLEDROW, LEDCOL5, ' ', false);
      lc.setChar(UPPERLEDROW, LEDCOL6, '0' + LockMode, false);
      lc.setChar(UPPERLEDROW, LEDCOL7, ' ', false);
    } else if (Unlocked == UM_FAILED ) {  // If PassCode verif failed
      lc.setChar(UPPERLEDROW, LEDCOL0, 'F', false);
      lc.setChar(UPPERLEDROW, LEDCOL1, 'A', false);
      lc.setChar(UPPERLEDROW, LEDCOL2, '1', false);
      lc.setChar(UPPERLEDROW, LEDCOL3, 'L', false);
      lc.setChar(UPPERLEDROW, LEDCOL4, ' ', false);
      lc.setChar(UPPERLEDROW, LEDCOL5, ' ', false);
      lc.setChar(UPPERLEDROW, LEDCOL6, ' ', false);
      lc.setChar(UPPERLEDROW, LEDCOL7, ' ', false);
    }
  }

  // Show the cursor and the number of char typed so far
  CurPos = LEDCOL0 - (TestPassCodeLength / 2);

  for (int i=LEDCOL0; i>=0; i--){
    if (CurPos < i)
      lc.setRow(LOWERLEDROW, i, SpecChar[CHR_EQL]);
    if (CurPos > i)
      lc.setChar(LOWERLEDROW, i, ' ', false);      
    if (CurPos == i ) {
      if (ToggleCurVal && TestPassCodeLength < MAXPWDLENGTH) 
        lc.setChar(LOWERLEDROW, CurPos, '_', false);
      else {
        if (TestPassCodeLength % 2 == 1)
          lc.setChar(LOWERLEDROW, i, '-', false); 
        else
          lc.setChar(LOWERLEDROW, i, ' ', false);     
      }
    }
  }
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Set every single segments to ON at full brightness, used only if the Knob is not correctly set
void DisplayTestPatern(void *) {
  lc.setIntensity(UPPERLEDROW, LEDMAXINTENSITY);
  lc.setIntensity(LOWERLEDROW, LEDMAXINTENSITY);

  lc.shutdown(UPPERLEDROW, false);
  lc.shutdown(LOWERLEDROW, false);

  lc.setRow(UPPERLEDROW, 0, SpecChar[CHR_TEST]);
  lc.setRow(UPPERLEDROW, 1, SpecChar[CHR_TEST]);
  lc.setRow(UPPERLEDROW, 2, SpecChar[CHR_TEST]);
  lc.setRow(UPPERLEDROW, 3, SpecChar[CHR_TEST]);
  lc.setRow(UPPERLEDROW, 4, SpecChar[CHR_TEST]);
  lc.setRow(UPPERLEDROW, 5, SpecChar[CHR_TEST]);
  lc.setRow(UPPERLEDROW, 6, SpecChar[CHR_TEST]);
  lc.setRow(UPPERLEDROW, 7, SpecChar[CHR_TEST]);

  lc.setRow(LOWERLEDROW, 0, SpecChar[CHR_TEST]);
  lc.setRow(LOWERLEDROW, 1, SpecChar[CHR_TEST]);
  lc.setRow(LOWERLEDROW, 2, SpecChar[CHR_TEST]);
  lc.setRow(LOWERLEDROW, 3, SpecChar[CHR_TEST]);
  lc.setRow(LOWERLEDROW, 4, SpecChar[CHR_TEST]);
  lc.setRow(LOWERLEDROW, 5, SpecChar[CHR_TEST]);
  lc.setRow(LOWERLEDROW, 6, SpecChar[CHR_TEST]);
  //lc.setRow(LOWERLEDROW, 7, SpecChar[CHR_TEST]);

  digitalWrite(LedTickPin, HIGH);
  digitalWrite(LedLockPin, HIGH);

  //Remove ASCII values left from UpdateModes (anything without a dot)
  if (ToggleCurChar < SpecChar[CHR_DOT]) {
    ToggleCurChar = SpecChar[CHR_DOT]; // ' .'
  }

  // React to a new Keypress
  if (NewKeyPress) {
    NewKeyPress = false; // Consume the Keypress

    switch (KeyPressVal) {
      case KEY_UP:
        ToggleCurChar = SpecChar[CHR_Ud]; //'U.';
        break;
      case KEY_DOWN:
        ToggleCurChar = SpecChar[CHR_dd]; //'d.';
        break;
      case KEY_LEFT:
        ToggleCurChar = SpecChar[CHR_Ld]; //'L.';
        break;
      case KEY_RIGHT:
        ToggleCurChar = SpecChar[CHR_rd]; // 'r.'
        break;
      case KEY_CLEAR:
        ToggleCurChar = SpecChar[CHR_Cd]; //'C.';
        break;
      case KEY_ENTER:
        ToggleCurChar = SpecChar[CHR_Ed]; //'E.';
        break;
      default: 
        ToggleCurChar = SpecChar[(KeyPressVal - '0') + CHR_DOTOFFET]; // N.
    }

  }

  // Display the current state
  if(ToggleCurVal)
    lc.setRow(LOWERLEDROW, LEDCOL0, ToggleCurChar);
  else
    lc.setRow(LOWERLEDROW, LEDCOL0, SpecChar[CHR_TEST]);

}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

