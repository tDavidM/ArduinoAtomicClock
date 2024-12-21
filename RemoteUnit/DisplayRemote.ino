
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
void DisplayCurrentDate(int DispFormat) {
  String ConvBuff;
  String DisplayVal;

  lc.setIntensity(0,LedIntensity);

  // Convert each Int to individual Char with leading zeros
  DisplayVal = String(Year);

  Month>9 ? ConvBuff=String(Month) : ConvBuff="0"+String(Month) ;
  DisplayVal = DisplayVal + ConvBuff;

  Day>9 ? ConvBuff=String(Day) : ConvBuff="0"+String(Day) ;
  DisplayVal = DisplayVal + ConvBuff;
  
  // Display the values in their respective position
  if (DispFormat == 0) {
    //YYYYMMDD
    lc.setDigit(0, 7, DisplayVal.charAt(0)-'0', false);
    lc.setDigit(0, 6, DisplayVal.charAt(1)-'0', false);
    lc.setDigit(0, 5,DisplayVal.charAt(2)-'0', false);
    lc.setRow(0, 4, SpecChar[(DisplayVal.charAt(3)-'0')+10]);
    lc.setDigit(0, 3, DisplayVal.charAt(4)-'0', false);
    lc.setRow(0, 2, SpecChar[(DisplayVal.charAt(5)-'0')+10]);
    lc.setDigit(0, 1, DisplayVal.charAt(6)-'0', false);

    if (LockIndicatorDot)
      lc.setRow(0, 0, SpecChar[(DisplayVal.charAt(7)-'0')+10]); // same value but with a dot
    else
      lc.setDigit(0, 0, DisplayVal.charAt(7)-'0', false);

  } else if (DispFormat == 1 ) {
    //YY-MM-DD
    lc.setDigit(0, 7, DisplayVal.charAt(2)-'0', false);
    lc.setDigit(0, 6, DisplayVal.charAt(3)-'0', false);
    lc.setRow(0, 5, SpecChar[BAR]);
    lc.setDigit(0, 4, DisplayVal.charAt(4)-'0', false);
    lc.setDigit(0, 3, DisplayVal.charAt(5)-'0', false);
    lc.setRow(0, 2, SpecChar[BAR]);
    lc.setDigit(0, 1, DisplayVal.charAt(6)-'0', false);

    if (LockIndicatorDot)
      lc.setRow(0, 0, SpecChar[(DisplayVal.charAt(7)-'0')+10]); // same value but with a dot
    else
      lc.setDigit(0, 0, DisplayVal.charAt(7)-'0', false);
  }
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Display the current Time
void DisplayCurrentTime(void *) {
  String ConvBuff;
  String DisplayVal;

  lc.setIntensity(0,LedIntensity);

  // Convert each Int to individual Char with leading zeros
  Hour>9 ? ConvBuff=String(Hour) : ConvBuff="0"+String(Hour) ;
  DisplayVal = ConvBuff;

  Minute>9 ? ConvBuff=String(Minute) : ConvBuff="0"+String(Minute) ;
  DisplayVal = DisplayVal + ConvBuff;

  Second>9 ? ConvBuff=String(Second) : ConvBuff="0"+String(Second) ;
  DisplayVal = DisplayVal + ConvBuff;
  
  // Display the values in their respective position
    //HH_MI SS
    lc.setDigit(0, 7, DisplayVal.charAt(0)-'0', false);
    lc.setDigit(0, 6, DisplayVal.charAt(1)-'0', false);
    lc.setRow(0, 5, SpecChar[CH_H]);
    lc.setDigit(0, 4, DisplayVal.charAt(2)-'0', false);
    lc.setDigit(0, 3, DisplayVal.charAt(3)-'0', false);
    lc.setRow(0, 2, SpecChar[BLANK]);
    lc.setDigit(0, 1, DisplayVal.charAt(4)-'0', false);

    if (LockIndicatorDot)
      lc.setRow(0, 0, SpecChar[(DisplayVal.charAt(5)-'0')+10]); // same value but with a dot
    else
      lc.setDigit(0, 0, DisplayVal.charAt(5)-'0', false);

}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Display the current AlternateTime (TimeZone)
void DisplayTZAltTime(void *) {
  String ConvBuff;
  String DisplayVal;

  lc.setIntensity(0,LedIntensity);

  // Convert each Int to individual Char with leading zeros
  HourTZAlt>9 ? ConvBuff=String(HourTZAlt) : ConvBuff="0"+String(HourTZAlt) ;
  DisplayVal = ConvBuff;

  MinuteTZAlt>9 ? ConvBuff=String(MinuteTZAlt) : ConvBuff="0"+String(MinuteTZAlt) ;
  DisplayVal = DisplayVal + ConvBuff;

  Second>9 ? ConvBuff=String(Second) : ConvBuff="0"+String(Second) ;
  DisplayVal = DisplayVal + ConvBuff;
  
  // Display the values in their respective position
  //HH_MI SS
  lc.setDigit(0, 7, DisplayVal.charAt(0)-'0', false);
  lc.setDigit(0, 6, DisplayVal.charAt(1)-'0', false);
  lc.setRow(0, 5, SpecChar[CH_H]);
  lc.setDigit(0, 4, DisplayVal.charAt(2)-'0', false);
  //lc.setDigit(0, 3, DisplayVal.charAt(3)-'0', false);
  lc.setRow(0, 3, SpecChar[(DisplayVal.charAt(3)-'0')+10]);
  lc.setRow(0, 2, SpecChar[BLANK]);
  lc.setDigit(0, 1, DisplayVal.charAt(4)-'0', false);

  if (LockIndicatorDot)
    lc.setRow(0, 0, SpecChar[(DisplayVal.charAt(5)-'0')+10]); // same value but with a dot
  else
    lc.setDigit(0, 0, DisplayVal.charAt(5)-'0', false);

}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Display the current Chrono
void DisplayCurrentChrono(void *) {
  String ConvBuff;
  String DisplayVal;

  lc.setIntensity(0,LedIntensity);

  // Convert each Int to individual Char with leading zeros
  ChronoHour>9 ? ChronoHour>99 ? ConvBuff=String(ChronoHour) : ConvBuff="0"+String(ChronoHour) : ConvBuff="00"+String(ChronoHour) ;
  DisplayVal = ConvBuff;

  ChronoMinute>9 ? ConvBuff=String(ChronoMinute) : ConvBuff="0"+String(ChronoMinute) ;
  DisplayVal = DisplayVal + ConvBuff;

  ChronoSecond>9 ? ConvBuff=String(ChronoSecond) : ConvBuff="0"+String(ChronoSecond) ;
  DisplayVal = DisplayVal + ConvBuff;
  
  // Display the values in their respective position
  //HHHMI SS
  lc.setDigit(0, 7, DisplayVal.charAt(0)-'0', false);
  lc.setDigit(0, 6, DisplayVal.charAt(1)-'0', false);
  lc.setRow(0, 5, SpecChar[(DisplayVal.charAt(2)-'0')+10]);
  lc.setDigit(0, 4,DisplayVal.charAt(3)-'0', false);
  lc.setDigit(0, 3, DisplayVal.charAt(4)-'0', false);
  lc.setRow(0, 2, SpecChar[BLANK]);
  lc.setDigit(0, 1, DisplayVal.charAt(5)-'0', false);

  if (LockIndicatorDot)
    lc.setRow(0, 0, SpecChar[(DisplayVal.charAt(6)-'0')+10]); // same value but with a dot
  else
    lc.setDigit(0, 0, DisplayVal.charAt(6)-'0', false);

}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Display the current CountDown
void DisplayCurrentCountDown(void *) {
  String ConvBuff;
  String DisplayVal;

  lc.setIntensity(0,LedIntensity);

  // Convert each Int to individual Char with leading zeros
  CountDownHour>9 ? CountDownHour>99 ? ConvBuff=String(CountDownHour) : ConvBuff="0"+String(CountDownHour) : ConvBuff="00"+String(CountDownHour) ;
  DisplayVal = ConvBuff;

  CountDownMinute>9 ? ConvBuff=String(CountDownMinute) : ConvBuff="0"+String(CountDownMinute) ;
  DisplayVal = DisplayVal + ConvBuff;

  CountDownSecond>9 ? CountDownSecond>99 ? ConvBuff=String(CountDownSecond) : ConvBuff="0"+String(CountDownSecond) : ConvBuff="00"+String(CountDownSecond) ;
  DisplayVal = DisplayVal + ConvBuff;
  
  // Display the values in their respective position
  //HHHMMSSS
  lc.setDigit(0, 7, DisplayVal.charAt(0)-'0', false);
  lc.setDigit(0, 6, DisplayVal.charAt(1)-'0', false);
  lc.setRow(0, 5, SpecChar[(DisplayVal.charAt(2)-'0')+10]);
  lc.setDigit(0, 4,DisplayVal.charAt(3)-'0', false);
  lc.setRow(0, 3, SpecChar[(DisplayVal.charAt(4)-'0')+10]);
  lc.setDigit(0, 2, DisplayVal.charAt(5)-'0', false);
  lc.setDigit(0, 1, DisplayVal.charAt(6)-'0', false);

  if (LockIndicatorDot)
    lc.setRow(0, 0, SpecChar[(DisplayVal.charAt(7)-'0')+10]); // same value but with a dot
  else
    lc.setDigit(0, 0, DisplayVal.charAt(7)-'0', false);

}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

void DisplayBlinkingDot(void *) {
  lc.shutdown(0,false);
  lc.setIntensity(0,LedIntensity);
  
  lc.setRow(0, 7, SpecChar[BLANK]);
  lc.setRow(0, 6, SpecChar[BLANK]);
  lc.setRow(0, 5, SpecChar[BLANK]);
  lc.setRow(0, 4, SpecChar[BLANK]);
  lc.setRow(0, 3, SpecChar[BLANK]);
  lc.setRow(0, 2, SpecChar[BLANK]);
  lc.setRow(0, 1, SpecChar[BLANK]);
  if (LockIndicatorDot)
    lc.setRow(0, 0, SpecChar[DOT]);
  else
    lc.setRow(0, 0, SpecChar[BLANK]);
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Set every single segments to ON at full brightness
void DisplayTestPatern(int LedMode) {
  lc.shutdown(0,false);
  lc.setIntensity(0, LedMode==1 ? 0xf : LedIntensity );
  
  lc.setRow(0,7,SpecChar[TEST]);
  lc.setRow(0,6,SpecChar[TEST]);
  lc.setRow(0,5,SpecChar[TEST]);
  lc.setRow(0,4,SpecChar[TEST]);
  lc.setRow(0,3,SpecChar[TEST]);
  lc.setRow(0,2,SpecChar[TEST]);
  lc.setRow(0,1,SpecChar[TEST]);

  if (LockIndicatorDot)
    lc.setRow(0,0,SpecChar[8]);
  else
    lc.setRow(0,0,SpecChar[TEST]);

}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
