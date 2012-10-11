
/*
 Solarbotics Firefly Code
    
    This example code shows how to Blink and fade LEDs using a software PWM method with "For" loops.
    as well as using interrupts and sleep functions with the ATTINY45/85V at 8MHz.
 
    The circuit:
    * LED1 is attached from digital pin 3 (D4) to the 47 ohm resistot then to GND
    * LED2 is attached from digital pin 6 (D1) to the 47 ohm resistor then to GND
    * LED3 is attached from digital pin 5 (D0) to the 47 ohm resistor then to GND
    * 
    * The button is attached from digital pin 7 (D2) to GND
    
    * IR recvr attached to D3
 
   Forked by Anca Mosoiu 08 Aug 2012
   
   Forked again by Michael Schrecker 19 Sep 2012
   
 
    Created 12 May 2012
    By Jeremie Boulianne for Solarbotics Ltd.
 
    Inspiration and sponsorship from Awesome Calgary winner:

      Calgary's Illuminated Landscapes: http://www.collaborativeprojectsyyc.blogspot.ca/

    Code elements & information borrowed from the following resources:

      High Low Tech's Programming an ATtiny with Arduino 1.0 Tutorial: http://hlt.media.mit.edu/?p=1695
      Engblaze's AVR/Arduino sleep function tutorial: http://www.engblaze.com/hush-little-microprocessor-avr-and-arduino-sleep-mode-basics/
      Engblaze's AVR/Arduino interrupt tutorial: http://www.engblaze.com/we-interrupt-this-program-to-bring-you-a-tutorial-on-arduino-interrupts/ 
      Alex Iannuzzi's reducing ATtiny power tutorial : http://www.insidegadgets.com/2011/02/05/reduce-attiny-power-consumption-by-sleeping-with-the-watchdog-timer/
      Dale Wheat's Tiny cyclon project: http://www.dalewheat.com/info/
*/

#include <avr/sleep.h>
 

//Code needed to modify the Analog to Digital Convertor (ADC) register bits to turn it on and off
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif                                       

//IR Pin definition, needed for raw read
#define IRpin_PIN      PIND
#define IRpin          3

/*

// the maximum pulse we'll listen for - 65 milliseconds is a long time
#define MAXPULSE 65000
#define NUMPULSES 50

// what our timing resolution should be, larger is better
// as its more 'precise' - but too large and you wont get
// accurate timing
#define RESOLUTION 20 

// What percent we will allow in variation to match the same code
#define FUZZINESS 20

// we will store up to 100 pulse pairs (this is -a lot-)
uint16_t pulses[NUMPULSES][2];  // pair is high and low pulse 
uint8_t currentpulse = 0; // index for pulses we're storing

#include "ircodes.h"

*/


//int wakePin = 0;       wa                    //Interrupt 0 which is the button on DIGITAL PIN 2!
int wakePin = 1;                           //Interrupt 1 which is the IR sensor on DIGITAL PIN 3!
int mode = 1;                              //Mode value for switch 
volatile int inputVal = 0;                 //Button Press which must be called as a volatile variable because it is used in an Interrupt Service Routine
int j;                                     //Counter value
int ledPin;                                //LED value
long randNumber;                           //Random number value
int del;              //delay value (ms)
int lop;              //loop value
int lop2;            //loop value 2
int IRRead;          //Raw IR Reading
int control = 0;        //Control state

void setup() {
  pinMode(4, OUTPUT);                      //Create (D4) LED1 as an output
  pinMode(5, OUTPUT);                      //Create (D5) LED2 as an output 
  pinMode(6, OUTPUT);                      //Create (D6) LED3 as an output 
  pinMode(2, INPUT);                       //Create our button pin as an input on D2
  pinMode(3, INPUT);                       //Create D3 as IR input
//  digitalWrite(2, HIGH);                   //Enable pullup resistor for IR (do we need to?) so it isn't floating
//  randomSeed(analogRead(3));               //Grab whatever flaoting value on (A3) to give us our random start
  randomSeed(3);                           //Not so random seed
//  attachInterrupt(wakePin, wakeUp, HIGH);   //Attach the interrupt to the IR Sensor on D3 and execute the ISR on a LOW signal

  Serial.begin(9600);
  Serial.println("Ready to decode IR!");
}

void loop(){
  
  switch(mode){                            //Switch case statement which check for which mode the program needs to be in
    case 1:
      FireFly(200);
      mode=0;
      break;   
  }

}
/*************************FIRE FLY SUBROUTINE***************************************/
  
void FireFly(unsigned int z){              //Randomly selected LEDs Pulse and fade like Fire flies

  del = 25;
  lop = 100;
  lop2 = 100;
  control = 0;
   
  while(1) {                               //Continue to inifitely run this pattern until the button interrupts the loop
  
   IRReader();
  
  }
}

// ************** IR Reader subroutine

void IRReader(void)
{              //Listen to IR Reader, D3, change control value
  
    
    IRRead = digitalRead(3); 
    Serial.print ("IR  State: "); Serial.println(IRRead);
    
    if (IRRead == HIGH){
      control = 1;
    } 
    if (IRRead == LOW){
      control = 2;
    }

}
