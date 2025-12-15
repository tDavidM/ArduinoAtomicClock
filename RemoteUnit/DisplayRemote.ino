
// Contains the rountines used to display information on the LED segment display
// Individual char are numbered from 7 to 0, fromt the left to the right
// The refresh rate is about 50ms and mostly controled by the routine UpdateDisplay()

// Bit->Segment relation for the bitmask
//  |--2--|
//  7     3
//  |     |
//  ---8---
//  |     |
//  6     4
//  |--5--|  .1.
// 0b12345678

// Display the current Date in the specified format
//void DisplayCurrentDate(int DispFormat)

// Display the current Time
//void DisplayCurrentTime(void *)

// Display the current AlternateTime (TimeZone)
//void DisplayTZAltTime(void *)

// Display the current Chrono
//void DisplayCurrentChrono(void *)

// Display the current CountDown
//void DisplayCurrentCountDown(void *)

// Turn off the display but keep the blinking dot PPS 
//void DisplayBlinkingDot(void *)

// Set every single segments to ON at full brightness, used only if the Knob is not correctly set
//void DisplayTestPatern(void *)

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Display the current Date in the specified format
void DisplayCurrentDate(byte DispFormat) {
  char DisplayVal[8];

  int display_acc;
  int display_step;

  lc.setIntensity(0, LedIntensity);

  // Convert each Int to individual Char with leading zeros
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
  // Display the values in their respective position
  if (DispFormat == DM_DATEISO) {
    //YYYYMMDD
    lc.setDigit(LEDROWID, LEDCOL0, DisplayVal[0], false);
    lc.setDigit(LEDROWID, LEDCOL1, DisplayVal[1], false);
    lc.setDigit(LEDROWID, LEDCOL2, DisplayVal[2], false);
    lc.setRow(LEDROWID,   LEDCOL3, SpecChar[(DisplayVal[3])+CHR_DOTOFFET]);
    lc.setDigit(LEDROWID, LEDCOL4, DisplayVal[4], false);
    lc.setRow(LEDROWID,   LEDCOL5, SpecChar[(DisplayVal[5])+CHR_DOTOFFET]);
    lc.setDigit(LEDROWID, LEDCOL6, DisplayVal[6], false);

    if (LockIndicatorDot)
      lc.setRow(LEDROWID,   LEDCOL7, SpecChar[(DisplayVal[7])+CHR_DOTOFFET]); // same value but with a dot
    else
      lc.setDigit(LEDROWID, LEDCOL7, DisplayVal[7], false);

  } else if (DispFormat == DM_DATESHORT ) {
    //YY-MM-DD
    lc.setDigit(LEDROWID, LEDCOL0, DisplayVal[2], false);
    lc.setDigit(LEDROWID, LEDCOL1, DisplayVal[3], false);
    lc.setRow(LEDROWID,   LEDCOL2, SpecChar[CHR_BAR]);
    lc.setDigit(LEDROWID, LEDCOL3, DisplayVal[4], false);
    lc.setDigit(LEDROWID, LEDCOL4, DisplayVal[5], false);
    lc.setRow(LEDROWID,   LEDCOL5, SpecChar[CHR_BAR]);
    lc.setDigit(LEDROWID, LEDCOL6, DisplayVal[6], false);

    if (LockIndicatorDot)
      lc.setRow(LEDROWID,   LEDCOL7, SpecChar[(DisplayVal[7])+CHR_DOTOFFET]); // same value but with a dot
    else
      lc.setDigit(LEDROWID, LEDCOL7, DisplayVal[7], false);
  }
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Display the current Time
void DisplayCurrentTime(void *) {
  char DisplayVal[8];

  int display_acc;
  int display_step;

  lc.setIntensity(LEDROWID, LedIntensity);

  // Convert single Int to "Char like" for Hour with 2 positions
  display_acc = Hour;

  display_step = display_acc-display_acc%10;
  DisplayVal[0] = display_step/10;
  display_acc = display_acc-display_step;

  DisplayVal[1] = display_acc;

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
  
  // Display the values in their respective position
    //HH_MI SS
    lc.setDigit(LEDROWID, LEDCOL0, DisplayVal[0], false);
    lc.setDigit(LEDROWID, LEDCOL1, DisplayVal[1], false);
    lc.setRow(LEDROWID,   LEDCOL2, SpecChar[CHR_H]);
    lc.setDigit(LEDROWID, LEDCOL3, DisplayVal[2], false);
    lc.setDigit(LEDROWID, LEDCOL4, DisplayVal[3], false);
    lc.setRow(LEDROWID,   LEDCOL5, SpecChar[CHR_BLANK]);
    lc.setDigit(LEDROWID, LEDCOL6, DisplayVal[4], false);

    if (LockIndicatorDot)
      lc.setRow(LEDROWID,   LEDCOL7, SpecChar[(DisplayVal[5])+CHR_DOTOFFET]); // same value but with a dot
    else
      lc.setDigit(LEDROWID, LEDCOL7, DisplayVal[5], false);
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Display the current AlternateTime (TimeZone)
void DisplayTZAltTime(void *) {
  char DisplayVal[8];

  int display_acc;
  int display_step;

  lc.setIntensity(LEDROWID, LedIntensity);

  // Convert single Int to "Char like" for Hour with 2 positions
  display_acc = HourTZAlt;

  display_step = display_acc-display_acc%10;
  DisplayVal[0] = display_step/10;
  display_acc = display_acc-display_step;

  DisplayVal[1] = display_acc;

  // Convert single Int to "Char like" for Minute with 2 positions
  display_acc = MinuteTZAlt;

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
  
  // Display the values in their respective position
  //HH_MI SS
  lc.setDigit(LEDROWID, LEDCOL0, DisplayVal[0], false);
  lc.setDigit(LEDROWID, LEDCOL1, DisplayVal[1], false);
  lc.setRow(LEDROWID,   LEDCOL2, SpecChar[CHR_H]);
  lc.setDigit(LEDROWID, LEDCOL3, DisplayVal[2], false);
  //lc.setDigit(LEDROWID, LEDCOL4, DisplayVal[3], false);
  lc.setRow(LEDROWID,   LEDCOL4, SpecChar[(DisplayVal[3])+CHR_DOTOFFET]);
  lc.setRow(LEDROWID,   LEDCOL5, SpecChar[CHR_BLANK]);
  lc.setDigit(LEDROWID, LEDCOL6, DisplayVal[4], false);

  if (LockIndicatorDot)
    lc.setRow(LEDROWID,   LEDCOL7, SpecChar[(DisplayVal[5])+CHR_DOTOFFET]); // same value but with a dot
  else
    lc.setDigit(LEDROWID, LEDCOL7, DisplayVal[5], false);
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Display the current Chrono
void DisplayCurrentChrono(void *) {
  char DisplayVal[8];

  int display_acc;
  int display_step;

  lc.setIntensity(LEDROWID, LedIntensity);

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
  
  // Display the values in their respective position
  //HHHMI SS
  lc.setDigit(LEDROWID, LEDCOL0, DisplayVal[0], false);
  lc.setDigit(LEDROWID, LEDCOL1, DisplayVal[1], false);
  lc.setRow(LEDROWID,   LEDCOL2, SpecChar[(DisplayVal[2])+CHR_DOTOFFET]);
  lc.setDigit(LEDROWID, LEDCOL3, DisplayVal[3], false);
  lc.setDigit(LEDROWID, LEDCOL4, DisplayVal[4], false);
  lc.setRow(LEDROWID,   LEDCOL5, SpecChar[CHR_BLANK]);
  lc.setDigit(LEDROWID, LEDCOL6, DisplayVal[5], false);

  if (LockIndicatorDot)
    lc.setRow(LEDROWID,   LEDCOL7, SpecChar[(DisplayVal[6])+CHR_DOTOFFET]); // same value but with a dot
  else
    lc.setDigit(LEDROWID, LEDCOL7, DisplayVal[6], false);

}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Display the current CountDown
void DisplayCurrentCountDown(void *) {
  char DisplayVal[8];

  int display_acc;
  int display_step;

  lc.setIntensity(LEDROWID, LedIntensity);

  // If the CountDown as reached Zero
  if (CountDownBlinking > -1) {
    if(CountDownBlinking % 10 == 0) {
      lc.setRow(LEDROWID, LEDCOL0, SpecChar[10]);
      lc.setRow(LEDROWID, LEDCOL1, SpecChar[10]);
      lc.setRow(LEDROWID, LEDCOL2, SpecChar[10]);
      lc.setRow(LEDROWID, LEDCOL3, SpecChar[10]);
      lc.setRow(LEDROWID, LEDCOL4, SpecChar[10]);
      lc.setRow(LEDROWID, LEDCOL5, SpecChar[10]);
      lc.setRow(LEDROWID, LEDCOL6, SpecChar[10]);
      lc.setRow(LEDROWID, LEDCOL7, SpecChar[10]);
    } else if(CountDownBlinking % 5 == 0) {
      lc.setChar(LEDROWID, LEDCOL0, '_', false);
      lc.setChar(LEDROWID, LEDCOL1, '_', false);
      lc.setChar(LEDROWID, LEDCOL2, '_', false);
      lc.setChar(LEDROWID, LEDCOL3, '_', false);
      lc.setChar(LEDROWID, LEDCOL4, '_', false);
      lc.setChar(LEDROWID, LEDCOL5, '_', false);
      lc.setChar(LEDROWID, LEDCOL6, '_', false);
      lc.setChar(LEDROWID, LEDCOL7, '_', false);
    }
    CountDownBlinking++;
    if(CountDownBlinking > 100)
      CountDownBlinking = -1;
  } else {
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

    // Display the values in their respective position
    //HHHMMSSS
    lc.setDigit(LEDROWID, LEDCOL0, DisplayVal[0], false);
    lc.setDigit(LEDROWID, LEDCOL1, DisplayVal[1], false);
    lc.setRow(LEDROWID,   LEDCOL2, SpecChar[(DisplayVal[2])+CHR_DOTOFFET]);
    lc.setDigit(LEDROWID, LEDCOL3, DisplayVal[3], false);
    lc.setRow(LEDROWID,   LEDCOL4, SpecChar[(DisplayVal[4])+CHR_DOTOFFET]);
    lc.setDigit(LEDROWID, LEDCOL5, DisplayVal[5], false);
    lc.setDigit(LEDROWID, LEDCOL6, DisplayVal[6], false);

    if (LockIndicatorDot)
      lc.setRow(LEDROWID,   LEDCOL7, SpecChar[(DisplayVal[7])+CHR_DOTOFFET]); // same value but with a dot
    else
      lc.setDigit(LEDROWID, LEDCOL7, DisplayVal[7], false);
  }
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

void DisplayBlinkingDot(void *) {
  lc.shutdown(LEDROWID, false);
  lc.setIntensity(LEDROWID, LedIntensity);
  
  lc.setRow(LEDROWID, LEDCOL0, SpecChar[CHR_BLANK]);
  lc.setRow(LEDROWID, LEDCOL1, SpecChar[CHR_BLANK]);
  lc.setRow(LEDROWID, LEDCOL2, SpecChar[CHR_BLANK]);
  lc.setRow(LEDROWID, LEDCOL3, SpecChar[CHR_BLANK]);
  lc.setRow(LEDROWID, LEDCOL4, SpecChar[CHR_BLANK]);
  lc.setRow(LEDROWID, LEDCOL5, SpecChar[CHR_BLANK]);
  lc.setRow(LEDROWID, LEDCOL6, SpecChar[CHR_BLANK]);
  if (LockIndicatorDot)
    lc.setRow(LEDROWID, LEDCOL7, SpecChar[CHR_DOT]);
  else
    lc.setRow(LEDROWID, LEDCOL7, SpecChar[CHR_BLANK]);
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Set every single segments to ON at full brightness
void DisplayTestPatern(bool LedMax) {
  lc.shutdown(LEDROWID, false);
  lc.setIntensity(LEDROWID, LedMax ? LEDMAXINTENSITY : LedIntensity );
  
  lc.setRow(LEDROWID, LEDCOL0, SpecChar[CHR_TEST]);
  lc.setRow(LEDROWID, LEDCOL1, SpecChar[CHR_TEST]);
  lc.setRow(LEDROWID, LEDCOL2, SpecChar[CHR_TEST]);
  lc.setRow(LEDROWID, LEDCOL3, SpecChar[CHR_TEST]);
  lc.setRow(LEDROWID, LEDCOL4, SpecChar[CHR_TEST]);
  lc.setRow(LEDROWID, LEDCOL5, SpecChar[CHR_TEST]);
  lc.setRow(LEDROWID, LEDCOL6, SpecChar[CHR_TEST]);

  if (LockIndicatorDot)
    lc.setRow(LEDROWID, LEDCOL7, SpecChar[8]);
  else
    lc.setRow(LEDROWID, LEDCOL7, SpecChar[CHR_TEST]);

}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
