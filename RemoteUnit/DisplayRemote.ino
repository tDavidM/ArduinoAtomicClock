
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
  char ConvBuff[8];
  char DisplayVal[8];

  lc.setIntensity(0,LedIntensity);

  // Convert each Int to individual Char with leading zeros
  itoa(Year, DisplayVal, 10);

  itoa(Month, ConvBuff, 10);
  if(Month>9) {
    DisplayVal[4] = ConvBuff[0];
    DisplayVal[5] = ConvBuff[1];
  } else {
    DisplayVal[4] = '0';
    DisplayVal[5] = ConvBuff[0];
  } 

  itoa(Day, ConvBuff, 10);
  if(Day>9) {
    DisplayVal[6] = ConvBuff[0];
    DisplayVal[7] = ConvBuff[1];
  } else {
    DisplayVal[6] = '0';
    DisplayVal[7] = ConvBuff[0];
  }
  
  // Display the values in their respective position
  if (DispFormat == 0) {
    //YYYYMMDD
    lc.setDigit(0, 7, DisplayVal[0]-'0', false);
    lc.setDigit(0, 6, DisplayVal[1]-'0', false);
    lc.setDigit(0, 5,DisplayVal[2]-'0', false);
    lc.setRow(0, 4, SpecChar[(DisplayVal[3]-'0')+10]);
    lc.setDigit(0, 3, DisplayVal[4]-'0', false);
    lc.setRow(0, 2, SpecChar[(DisplayVal[5]-'0')+10]);
    lc.setDigit(0, 1, DisplayVal[6]-'0', false);

    if (LockIndicatorDot)
      lc.setRow(0, 0, SpecChar[(DisplayVal[7]-'0')+10]); // same value but with a dot
    else
      lc.setDigit(0, 0, DisplayVal[7]-'0', false);

  } else if (DispFormat == 1 ) {
    //YY-MM-DD
    lc.setDigit(0, 7, DisplayVal[2]-'0', false);
    lc.setDigit(0, 6, DisplayVal[3]-'0', false);
    lc.setRow(0, 5, SpecChar[BAR]);
    lc.setDigit(0, 4, DisplayVal[4]-'0', false);
    lc.setDigit(0, 3, DisplayVal[5]-'0', false);
    lc.setRow(0, 2, SpecChar[BAR]);
    lc.setDigit(0, 1, DisplayVal[6]-'0', false);

    if (LockIndicatorDot)
      lc.setRow(0, 0, SpecChar[(DisplayVal[7]-'0')+10]); // same value but with a dot
    else
      lc.setDigit(0, 0, DisplayVal[7]-'0', false);
  }
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Display the current Time
void DisplayCurrentTime(void *) {
  char ConvBuff[8];
  char DisplayVal[8];

  lc.setIntensity(0,LedIntensity);

  // Convert each Int to individual Char with leading zeros
  itoa(Hour, ConvBuff, 10);
  if(Hour>9) {
    DisplayVal[0] = ConvBuff[0];
    DisplayVal[1] = ConvBuff[1];
  } else {
    DisplayVal[0] = '0';
    DisplayVal[1] = ConvBuff[0];
  }

  itoa(Minute, ConvBuff, 10);
  if(Minute>9) {
    DisplayVal[2] = ConvBuff[0];
    DisplayVal[3] = ConvBuff[1];
  } else {
    DisplayVal[2] = '0';
    DisplayVal[3] = ConvBuff[0];
  }

  itoa(Second, ConvBuff, 10);
  if(Second>9) {
    DisplayVal[4] = ConvBuff[0];
    DisplayVal[5] = ConvBuff[1];
  } else {
    DisplayVal[4] = '0';
    DisplayVal[5] = ConvBuff[0];
  }
  
  // Display the values in their respective position
    //HH_MI SS
    lc.setDigit(0, 7, DisplayVal[0]-'0', false);
    lc.setDigit(0, 6, DisplayVal[1]-'0', false);
    lc.setRow(0, 5, SpecChar[CH_H]);
    lc.setDigit(0, 4, DisplayVal[2]-'0', false);
    lc.setDigit(0, 3, DisplayVal[3]-'0', false);
    lc.setRow(0, 2, SpecChar[BLANK]);
    lc.setDigit(0, 1, DisplayVal[4]-'0', false);

    if (LockIndicatorDot)
      lc.setRow(0, 0, SpecChar[(DisplayVal[5]-'0')+10]); // same value but with a dot
    else
      lc.setDigit(0, 0, DisplayVal[5]-'0', false);
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Display the current AlternateTime (TimeZone)
void DisplayTZAltTime(void *) {
  char ConvBuff[8];
  char DisplayVal[8];

  lc.setIntensity(0,LedIntensity);

  // Convert each Int to individual Char with leading zeros
  itoa(HourTZAlt, ConvBuff, 10);
  if(HourTZAlt>9) {
    DisplayVal[0] = ConvBuff[0];
    DisplayVal[1] = ConvBuff[1];
  } else {
    DisplayVal[0] = '0';
    DisplayVal[1] = ConvBuff[0];
  }

  itoa(MinuteTZAlt, ConvBuff, 10);
  if(MinuteTZAlt>9) {
    DisplayVal[2] = ConvBuff[0];
    DisplayVal[3] = ConvBuff[1];
  } else {
    DisplayVal[2] = '0';
    DisplayVal[3] = ConvBuff[0];
  } 

  itoa(Second, ConvBuff, 10);
  if(Second>9) {
    DisplayVal[4] = ConvBuff[0];
    DisplayVal[5] = ConvBuff[1];
  } else {
    DisplayVal[4] = '0';
    DisplayVal[5] = ConvBuff[0];
  }
  
  // Display the values in their respective position
  //HH_MI SS
  lc.setDigit(0, 7, DisplayVal[0]-'0', false);
  lc.setDigit(0, 6, DisplayVal[1]-'0', false);
  lc.setRow(0, 5, SpecChar[CH_H]);
  lc.setDigit(0, 4, DisplayVal[2]-'0', false);
  //lc.setDigit(0, 3, DisplayVal[3]-'0', false);
  lc.setRow(0, 3, SpecChar[(DisplayVal[3]-'0')+10]);
  lc.setRow(0, 2, SpecChar[BLANK]);
  lc.setDigit(0, 1, DisplayVal[4]-'0', false);

  if (LockIndicatorDot)
    lc.setRow(0, 0, SpecChar[(DisplayVal[5]-'0')+10]); // same value but with a dot
  else
    lc.setDigit(0, 0, DisplayVal[5]-'0', false);
}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Display the current Chrono
void DisplayCurrentChrono(void *) {
  char ConvBuff[8];
  char DisplayVal[8];

  lc.setIntensity(0,LedIntensity);

  // Convert each Int to individual Char with leading zeros
  itoa(ChronoHour, ConvBuff, 10);
  if(ChronoHour>99) {
    DisplayVal[0] = ConvBuff[0];
    DisplayVal[1] = ConvBuff[1];
    DisplayVal[2] = ConvBuff[2];
  } else if(ChronoHour>9) {
    DisplayVal[0] = '0';
    DisplayVal[1] = ConvBuff[0];
    DisplayVal[2] = ConvBuff[1];
  } else {
    DisplayVal[0] = '0';
    DisplayVal[1] = '0';
    DisplayVal[2] = ConvBuff[0];
  }

  itoa(ChronoMinute, ConvBuff, 10);
  if(ChronoMinute>9) {
    DisplayVal[3] = ConvBuff[0];
    DisplayVal[4] = ConvBuff[1];
  } else {
    DisplayVal[3] = '0';
    DisplayVal[4] = ConvBuff[0];
  }

  itoa(ChronoSecond, ConvBuff, 10);
  if(ChronoSecond>9) {
    DisplayVal[5] = ConvBuff[0];
    DisplayVal[6] = ConvBuff[1];
  } else {
    DisplayVal[5] = '0';
    DisplayVal[6] = ConvBuff[0];
  }
  
  // Display the values in their respective position
  //HHHMI SS
  lc.setDigit(0, 7, DisplayVal[0]-'0', false);
  lc.setDigit(0, 6, DisplayVal[1]-'0', false);
  lc.setRow(0, 5, SpecChar[(DisplayVal[2]-'0')+10]);
  lc.setDigit(0, 4,DisplayVal[3]-'0', false);
  lc.setDigit(0, 3, DisplayVal[4]-'0', false);
  lc.setRow(0, 2, SpecChar[BLANK]);
  lc.setDigit(0, 1, DisplayVal[5]-'0', false);

  if (LockIndicatorDot)
    lc.setRow(0, 0, SpecChar[(DisplayVal[6]-'0')+10]); // same value but with a dot
  else
    lc.setDigit(0, 0, DisplayVal[6]-'0', false);

}

//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####
//     #####     #####     #####     #####     #####

// Display the current CountDown
void DisplayCurrentCountDown(void *) {
  char ConvBuff[8];
  char DisplayVal[8];

  lc.setIntensity(0,LedIntensity);

  // If the CountDown as reached Zero
  if (CountDownBlinking > -1) {
    if(CountDownBlinking % 10 == 0) {
      lc.setRow(0,7,SpecChar[10]);
      lc.setRow(0,6,SpecChar[10]);
      lc.setRow(0,5,SpecChar[10]);
      lc.setRow(0,4,SpecChar[10]);
      lc.setRow(0,3,SpecChar[10]);
      lc.setRow(0,2,SpecChar[10]);
      lc.setRow(0,1,SpecChar[10]);
      lc.setRow(0,0,SpecChar[10]);
    } else if(CountDownBlinking % 5 == 0) {
      lc.setChar(0,7,'_',false);
      lc.setChar(0,6,'_',false);
      lc.setChar(0,5,'_',false);
      lc.setChar(0,4,'_',false);
      lc.setChar(0,3,'_',false);
      lc.setChar(0,2,'_',false);
      lc.setChar(0,1,'_',false);
      lc.setChar(0,0,'_',false);
    }
    CountDownBlinking++;
    if(CountDownBlinking > 100)
      CountDownBlinking = -1;
  } else {
    // Convert each Int to individual Char with leading zeros
    itoa(CountDownHour, ConvBuff, 10);
    if(CountDownHour>99) {
      DisplayVal[0] = ConvBuff[0];
      DisplayVal[1] = ConvBuff[1];
      DisplayVal[2] = ConvBuff[2];
    } else if(CountDownHour>9) {
      DisplayVal[0] = '0';
      DisplayVal[1] = ConvBuff[0];
      DisplayVal[2] = ConvBuff[1];
    } else {
      DisplayVal[0] = '0';
      DisplayVal[1] = '0';
      DisplayVal[2] = ConvBuff[0];
   }

    itoa(CountDownMinute, ConvBuff, 10);
    if(CountDownMinute>9) {
      DisplayVal[3] = ConvBuff[0];
      DisplayVal[4] = ConvBuff[1];
    } else {
      DisplayVal[3] = '0';
      DisplayVal[4] = ConvBuff[0];
    }

    itoa(CountDownSecond, ConvBuff, 10);
    if(CountDownSecond>99) {
      DisplayVal[5] = ConvBuff[0];
      DisplayVal[6] = ConvBuff[1];
      DisplayVal[7] = ConvBuff[2];
    } else if(CountDownSecond>9) {
      DisplayVal[5] = '0';
      DisplayVal[6] = ConvBuff[0];
      DisplayVal[7] = ConvBuff[1];
    } else {
      DisplayVal[5] = '0';
      DisplayVal[6] = '0';
      DisplayVal[7] = ConvBuff[0];
    }
  
    // Display the values in their respective position
    //HHHMMSSS
    lc.setDigit(0, 7, DisplayVal[0]-'0', false);
    lc.setDigit(0, 6, DisplayVal[1]-'0', false);
    lc.setRow(0, 5, SpecChar[(DisplayVal[2]-'0')+10]);
    lc.setDigit(0, 4,DisplayVal[3]-'0', false);
    lc.setRow(0, 3, SpecChar[(DisplayVal[4]-'0')+10]);
    lc.setDigit(0, 2, DisplayVal[5]-'0', false);
    lc.setDigit(0, 1, DisplayVal[6]-'0', false);

    if (LockIndicatorDot)
      lc.setRow(0, 0, SpecChar[(DisplayVal[7]-'0')+10]); // same value but with a dot
    else
      lc.setDigit(0, 0, DisplayVal[7]-'0', false);
  }
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
