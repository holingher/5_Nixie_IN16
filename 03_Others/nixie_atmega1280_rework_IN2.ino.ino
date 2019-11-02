// Nixie-Bot
// Created 2013
// Scott-Bot.com

#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 RTC;
DateTime now;

//Pin connected to ST_CP of 74HC595
int latchPin = 48;
//Pin connected to SH_CP of 74HC595
int clockPin = 50;
////Pin connected to DS of 74HC595
int dataPin = 52;

int testpattern = 0;

unsigned long SecondCount = 0; // Second counter
unsigned long MinuteCount = 0; // Second counter
int randint;

byte DataOut1 = B11111111;
byte DataOut2 = B11111111;
byte DataOut3 = B11111111;

void setup () 
{   
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  Serial.begin(57600);
  //Wire.begin();
  RTC.begin();
    RTC.adjust(DateTime(2014, 1, 21, 3, 50, 0));
  //Wait for power supply to power up
  delay(2000);
}

void loop () {
  GetTime();
  DispTime();
}

// Displays the time
void DispTime() 
{
  /*Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();*/
    delay(500);
  int hourdigit1 =(now.hour())/10;
  int hourdigit2 =(now.hour())%10;
  int mindigit1 =(now.minute())/10;
  int mindigit2 =(now.minute())%10;
  int secdigit1 =(now.second())/10;
  int secdigit2 =(now.second())%10;
  SetDigit(1, hourdigit2);
  SetDigit(2, hourdigit1);
  SetDigit(3, mindigit2);
  SetDigit(4, mindigit1);
  SetDigit(5, secdigit2);
  SetDigit(6, secdigit1);

  shiftOut();
  
}

// Sets a digit of the display
void SetDigit(int Digit, int Number) 
{
  byte BinNum;

  if ( Number == 0) {  
    BinNum= B0000;
  }
  else if ( Number == 1) {  
    BinNum= B0001;
  }
  else if ( Number == 2) {  
    BinNum= B0010;
  }
  else if ( Number == 3) {  
    BinNum= B0011;
  }
  else if ( Number == 4) {  
    BinNum= B0100;
  }
  else if ( Number == 5) {  
    BinNum= B0101;
  }
  else if ( Number == 6) {  
    BinNum= B0110;
  }
  else if ( Number == 7) {  
    BinNum= B0111;
  }
  else if ( Number == 8) {  
    BinNum= B1000;
  }
  else if ( Number == 9) {  
    BinNum= B1001;
  }
  else {
    BinNum= B0000;
  }

  if ( Digit == 1) {  
    DataOut1 &= B00001111;
    DataOut1 |= (BinNum << 4);
  }
  else if ( Digit == 2) { 
    DataOut1 &= B11110000;
    DataOut1 |= BinNum;
  }
  else if ( Digit == 3) { 
    DataOut2 &= B00001111;
    DataOut2 |= (BinNum << 4);
  }
  else if ( Digit == 4) { 
    DataOut2 &= B11110000;
    DataOut2 |= BinNum;
  }
  else if ( Digit == 5) { 
    DataOut3 &= B00001111;
    DataOut3 |= (BinNum << 4);
  }
  else if ( Digit == 6) { 
    DataOut3 &= B11110000;
    DataOut3 |= BinNum;
  }
  
}  

// Shifts out the data to the shift registers
void shiftOut() {
  // This shifts 8 bits out MSB first, 
  //on the rising edge of the clock,
  //clock idles low

  //internal function setup
  int i=0;
  int pinState;

  //clear everything out just in case to
  //prepare shift register for bit shifting
  //ground latchPin and hold low for as long as you are transmitting
  digitalWrite(latchPin, 0);
  digitalWrite(dataPin, 0);
  digitalWrite(clockPin, 0);

  //for each bit in the byte myDataOutďż˝
  //NOTICE THAT WE ARE COUNTING DOWN in our for loop
  //This means that %00000001 or "1" will go through such
  //that it will be pin Q0 that lights. 
  for (i=7; i>=0; i--)  {
    digitalWrite(clockPin, 0);

    //if the value passed to myDataOut and a bitmask result 
    // true then... so if we are at i=6 and our value is
    // %11010100 it would the code compares it to %01000000 
    // and proceeds to set pinState to 1.
    if ( DataOut1 & (1<<i) ) {
      pinState= 1;
    }
    else {  
      pinState= 0;
    }

    //Sets the pin to HIGH or LOW depending on pinState
    digitalWrite(dataPin, pinState);
    //register shifts bits on upstroke of clock pin  
    digitalWrite(clockPin, 1);
    //zero the data pin after shift to prevent bleed through
    digitalWrite(dataPin, 0);
  }
  
  //stop shifting
  digitalWrite(clockPin, 0);
  
  // Shift the second word
  for (i=7; i>=0; i--)  {
    digitalWrite(clockPin, 0);

    if ( DataOut2 & (1<<i) ) {
      pinState= 1;
    }
    else {  
      pinState= 0;
    }
    
    digitalWrite(dataPin, pinState);
    digitalWrite(clockPin, 1);
    digitalWrite(dataPin, 0);
  }

  digitalWrite(clockPin, 0);
  
  // Shift the third word
  for (i=7; i>=0; i--)  {
    digitalWrite(clockPin, 0);

    if ( DataOut3 & (1<<i) ) {
      pinState= 1;
    }
    else {  
      pinState= 0;
    }

    digitalWrite(dataPin, pinState);
    digitalWrite(clockPin, 1);
    digitalWrite(dataPin, 0);
  }

  digitalWrite(clockPin, 0);
  digitalWrite(latchPin, 1);
}

// Read from the real time clock
void GetTime () {
  now = RTC.now();
}


