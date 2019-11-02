#include "Wire.h"
#include "RTClib.h"

RTC_DS3231 rtc;
DateTime now;

int tube = 0;

void setup() {
  if (! rtc.begin()) {
    while (1);
  }
  // DS3231 seconds, minutes, hours, day, date, month, year
  if (rtc.lostPower()) {
    //Sets the code compilation time to RTC DS3231
   // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  setDS3231time();

  //delay(2000);
}
long index_test = 0;
//tube0 = PB2   tube1 = PC0  tube2 = PB4 
//tube3 = PB5???   tube4 = PC1  tube5 = PB3
//driver A:       driver B:
//PD2 = A         PD6 = A
//PD4 = B         PD7 = B
//PD5 = C         PB1 = C
//PD3 = D         PB0 = D
void loop() {
  // retrieve data from DS3231
  now = rtc.now();
  //blink the IN3 tubes: PC0 and PC1
  /*if ( (now.second() % 2) == 0) 
  { PORTC |= (1 << PC0); PORTC |= (1 << PC1); }
  else
  { PORTC &= ~(1 << PC0); PORTC &= ~(1 << PC1); }*/

  //if ( /*(now.second() % 2)*/index_test == 1000) 
  { PORTC |= (1 << PC2); }
  //else
  //{ PORTC &= ~(1 << PC2); }

  //move to the next tube
  tube++;
  if(tube>=6){tube=0;}
  //set correct digit
  Set_digit(tube);
}

void close_digits()
{
  delay(10);
  //driver B
  PORTB &= ~(1 << PB0); //D
  PORTB &= ~(1 << PB1); //C
  PORTD &= ~(1 << PD7); //B
  PORTD &= ~(1 << PD6); //A

  //driver A
  PORTD &= ~(1 << PD3); //D
  PORTD &= ~(1 << PD5); //C
  PORTD &= ~(1 << PD4); //B
  PORTD &= ~(1 << PD2); //A
  delay(10);
}

void Set_digit(int int_tube)
{
close_digits();
switch(int_tube)
{  
  ////////// seconds
  case 0: //PB2 - SECONDS_UNIT
    //light the tube
    delay(1);
    PORTC &= ~(1 << PC0);
    delay(1);
    PORTB &= ~(1 << PB4);
    delay(1);
    PORTB |= (1 << PB2);
    delay(10);
    //light the number
    driverA_number(0/*(now.second())%10*/);
    driverB_number(0/*(now.second())%10*/);
    delay(10);
    PORTB &= ~(1 << PB2);
    break;
  case 1: //PC0 - SECONDS_DECIMAL
    //light the tube
    delay(1);
    PORTB &= ~(1 << PB2);
    delay(1);
    PORTB &= ~(1 << PB4);
    delay(1);
    PORTC |= (1 << PC0);
    delay(10);
    //light the number
    driverA_number(1/*(now.second())/10*/);
    driverB_number(1/*(now.second())/10*/);
    delay(10);
    PORTC &= ~(1 << PC0);
    break;

  //////////// minutes
  case 2: //PB4 - MINUTES_UNIT
    //light the tube
    delay(1);
    PORTB &= ~(1 << PB2);
    delay(1);
    PORTC &= ~(1 << PC0);
    delay(1);
    PORTB |= (1 << PB4);
    delay(10);
    //light the number
    driverA_number(2/*(now.minute())%10*/);
    driverB_number(2/*(now.minute())%10*/);
    delay(10);
    PORTB &= ~(1 << PB4);
    break;
#if 0
  case 3: //PB5 - minutes units
    //light the tube
    delay(1);
    PORTC &= ~(1 << PC1);
    delay(1);
    PORTB &= ~(1 << PB3);
    delay(1);
    PORTB |= (1 << PB5);
    delay(10);
    //light the number
    driverB_number(3/*(now.minute())/10*/);
    delay(10);
    PORTB &= ~(1 << PB5);
    break;

  ///////////// hours
  case 5: //PC1 - HOURS_DECIMAL
    //light the tube
    delay(1);
    PORTB &= ~(1 << PB5);
    delay(1);
    PORTB &= ~(1 << PB3);
    delay(1);
    PORTC |= (1 << PC1);
    delay(10);
    //light the number
    driverB_number(4/*(now.hour())/10*/);
    delay(10);
    PORTC &= ~(1 << PC1);
    break;
  case 4: //PB3 - HOURS_UNIT
    //light the tube
    delay(1);
    PORTB &= ~(1 << PB5);
    delay(1);
    PORTC &= ~(1 << PC1);
    delay(1);
    PORTB |= (1 << PB3);
    delay(10);
    //light the number
    driverB_number(5/*(now.hour())%10*/);
    delay(10);
    PORTB &= ~(1 << PB3);
    break;
#endif
}
}

//hours and mins
void driverA_number(int Number)
{
  if ( Number == 0) { //0b0000 
    PORTD &= ~(1 << PD2);//A
    PORTD &= ~(1 << PD4);//B
    PORTD &= ~(1 << PD5);//C
    PORTD &= ~(1 << PD3);//D
  }
  else if ( Number == 1) {  //0b0001 
    PORTD &= ~(1 << PD2);//A
    PORTD &= ~(1 << PD4);//B
    PORTD &= ~(1 << PD5);//C
    PORTD |= (1 << PD3);//D
  }
  else if ( Number == 2) {  //0b0010 
    PORTD &= ~(1 << PD2);//A
    PORTD &= ~(1 << PD4);//B
    PORTD |= (1 << PD5);//C
    PORTD &= ~(1 << PD3);//D
  }
  else if ( Number == 3) {  //0b0011
    PORTD &= ~(1 << PD2);//A
    PORTD &= ~(1 << PD4);//B
    PORTD |= (1 << PD5);//C
    PORTD |= (1 << PD3);//D
  }
  else if ( Number == 4) {  //0b0100
    PORTD &= ~(1 << PD2);//A
    PORTD |= (1 << PD4);//B
    PORTD &= ~(1 << PD5);//C
    PORTD &= ~(1 << PD3);//D
  }
  else if ( Number == 5) {  //0b0101
    PORTD &= ~(1 << PD2);//A
    PORTD |= (1 << PD4);//B
    PORTD &= ~(1 << PD5);//C
    PORTD |= (1 << PD3);//D
  }
  else if ( Number == 6) {  //0b0110
    PORTD &= ~(1 << PD2);//A
    PORTD |= (1 << PD4);//B
    PORTD |= (1 << PD5);//C
    PORTD &= ~(1 << PD3);//D
  }
  else if ( Number == 7) {  //0b0111
    PORTD &= ~(1 << PD2);//A
    PORTD |= (1 << PD4);//B
    PORTD |= (1 << PD5);//C
    PORTD |= (1 << PD3);//D
  }
  else if ( Number == 8) {  //0b1000
    PORTD |= (1 << PD2);//A
    PORTD &= ~(1 << PD4);//B
    PORTD &= ~(1 << PD5);//C
    PORTD &= ~(1 << PD3);//D
  }
  else if ( Number == 9) {  //0b1001
    PORTD |= (1 << PD2);//A
    PORTD &= ~(1 << PD4);//B
    PORTD &= ~(1 << PD5);//C
    PORTD |= (1 << PD3);//D
  }
  else {//0b0000 
    PORTD &= ~(1 << PD2);//A
    PORTD &= ~(1 << PD4);//B
    PORTD &= ~(1 << PD5);//C
    PORTD &= ~(1 << PD3);//D
  }
}

//seconds and mins
void driverB_number(int Number)
{
  if ( Number == 0) { //0b0000 
    PORTD &= ~(1 << PD6);//A
    PORTD &= ~(1 << PD7);//B
    PORTB &= ~(1 << PB1);//C
    PORTB &= ~(1 << PB0);//D
  }
  else if ( Number == 1) {  //0b0001 
    PORTD &= ~(1 << PD6);//A
    PORTD &= ~(1 << PD7);//B
    PORTB &= ~(1 << PB1);//C
    PORTB |= (1 << PB0);//D
  }
  else if ( Number == 2) {  //0b0010 
    PORTD &= ~(1 << PD6);//A
    PORTD &= ~(1 << PD7);//B
    PORTB |= (1 << PB1);//C
    PORTB &= ~(1 << PB0);//D
  }
  else if ( Number == 3) {  //0b0011
    PORTD &= ~(1 << PD6);//A
    PORTD &= ~(1 << PD7);//B
    PORTB |= (1 << PB1);//C
    PORTB |= (1 << PB0);//D
  }
  else if ( Number == 4) {  //0b0100
    PORTD &= ~(1 << PD6);//A
    PORTD |= (1 << PD7);//B
    PORTB &= ~(1 << PB1);//C
    PORTB &= ~(1 << PB0);//D
  }
  else if ( Number == 5) {  //0b0101
    PORTD &= ~(1 << PD6);//A
    PORTD |= (1 << PD7);//B
    PORTB &= ~(1 << PB1);//C
    PORTB |= (1 << PB0);//D
  }
  else if ( Number == 6) {  //0b0110
    PORTD &= ~(1 << PD6);//A
    PORTD |= (1 << PD7);//B
    PORTB |= (1 << PB1);//C
    PORTB &= ~(1 << PB0);//D
  }
  else if ( Number == 7) {  //0b0111
    PORTD &= ~(1 << PD6);//A
    PORTD |= (1 << PD7);//B
    PORTB |= (1 << PB1);//C
    PORTB |= (1 << PB0);//D
  }
  else if ( Number == 8) {  //0b1000
    PORTD |= (1 << PD6);//A
    PORTD &= ~(1 << PD7);//B
    PORTB &= ~(1 << PB1);//C
    PORTB &= ~(1 << PB0);//D
  }
  else if ( Number == 9) {  //0b1001
    PORTD |= (1 << PD6);//A
    PORTD &= ~(1 << PD7);//B
    PORTB &= ~(1 << PB1);//C
    PORTB |= (1 << PB0);//D
  }
  else {//0b0000 
    PORTD &= ~(1 << PD6);//A
    PORTD &= ~(1 << PD7);//B
    PORTB &= ~(1 << PB1);//C
    PORTB &= ~(1 << PB0);//D
  }
}

void setDS3231time()
{
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  rtc.adjust(DateTime(2018, 8, 9, 22, 38, 22));
}
