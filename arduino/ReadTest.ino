#include "DS1374.h"
#include <Time.h>
#include <Wire.h>
#include "LedControl.h"
/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn
 pin 13 is connected to the CLK
 pin 14 is connected to LOAD
 We have only 2 MAX72XX.
 */
LedControl lc=LedControl(12,13,10,2);

/* we always wait a bit between updates of the display */
unsigned long delaytime=250;

tmElements_t tm;  
uint64_t startM = 31556736000LL;

void setup() {

  Serial.begin(9600);
  //while (!Serial) ; // wait for serial
  //delay(200);
 /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  lc.shutdown(1,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,15);
  lc.setIntensity(1,15);
  lc.setScanLimit(0, 6);
  lc.setScanLimit(1, 5);
  /* and clear the display */
  lc.clearDisplay(0);
  lc.clearDisplay(1);

    if (!RTC.readTime(tm)) 
    {
       if (RTC.chipPresent()) 
       {
           {
             // and configure the RTC with this info
             unsigned long t0 = 0;
             if (RTC.setTimeCounter(t0)) 
             {
                //Serial.println("DS1374 configured Time");
              }
           }  
       } else Serial.println("DS1374 read error!  Please check the circuitry.");
    }
  
  //Serial.println("DS1374RTC Read Test");
  //Serial.println("-------------------");
}

void loop() {
  tmElements_t tm;
  uint64_t tM;
  unsigned long tc;

  if (RTC.readTimeCounter(tc)) {
    //Serial.print("Ok, Timer = ");
    printNumber(startM - tc);
    //Serial.println();
  } else {
    if (RTC.chipPresent()) {
      if (RTC.ackRTCOSF()) {
      //Serial.println("The DS1374 is stopped.  Please run the SetTime");
      //Serial.println("to initialize the time and begin running.");
      //Serial.println();
      } 
    } else {
      //Serial.println("DS1374 read error!  Please check the circuitry.");
      //Serial.println();
    }
    delay(400);
  }
  delay(1000);
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}

void printNumber(uint64_t n)
{
 int digit;  
 int i = 0;
 int p=0; 

  while(n) {
    digit = n % 10;
    n /= 10;
    lc.setDigit(p,i,digit,false);
    i++;
    if (i>5) 
    {
      p++;
      i =0;
    }
    // Do something with digit
    Serial.print(digit);
  } 
}



