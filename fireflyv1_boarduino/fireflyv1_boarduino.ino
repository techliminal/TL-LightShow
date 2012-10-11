
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
  pinMode(1, OUTPUT);                      //Create (D1) LED2 as an output 
  pinMode(0, OUTPUT);                      //Create (D0) LED3 as an output 
  pinMode(2, INPUT);                       //Create our button pin as an input on D2
  pinMode(3, INPUT);                       //Create D3 as IR input
//  digitalWrite(2, HIGH);                   //Enable pullup resistor for IR (do we need to?) so it isn't floating
//  randomSeed(analogRead(3));               //Grab whatever flaoting value on (A3) to give us our random start
  randomSeed(3);                           //Not so random seed
//  attachInterrupt(wakePin, wakeUp, HIGH);   //Attach the interrupt to the IR Sensor on D3 and execute the ISR on a LOW signal
}

void loop(){
  
  switch(mode){                            //Switch case statement which check for which mode the program needs to be in
  
    case 0:
      sleepNow();                          //Call function and wait for a push button interrupt
      mode++;                              //Increase the mode value to switch to the next case
      break;                               //Exit switch case
       
    case 1:
//      BlinkAll(750); 
      FireFly(200);
      mode=0;
      break;   
       
 /*   case 2:
      BlinkAll(150);
      mode++;
      break;
      
    case 3:
      BlinkAll(75);
      mode++;
      break;
      
    case 4:
      sleepNow();                          
      mode++;                             
      break;        
  
    case 5:
      Metronome(200);
      mode++;
      break;
      
    case 6:
      Metronome(150);
      mode++;
      break;
      
    case 7:
      Metronome(100);
      mode++;
      break;
      
    case 8:
      sleepNow();                          //Call function and wait for a push button interrupt
      mode++;                              //Increase the mode value to switch to the next case
      break;        
  
    case 9:
      Disco(100);
      mode++;
      break;
      
    case 10:
      Disco(75);
      mode++;
      break;
      
    case 11:
      Disco(56);
      mode++;
      break;
  
    case 12:
      sleepNow();                          //Call function and wait for a push button interrupt
      mode++;                              //Increase the mode value to switch to the next case
      break;        
      
    case 13:
      FadingEyes(150);
      mode++;
      break;
      
    case 14:
      FadingEyes(100);
      mode++;
      break;
      
    case 15:
      FadingEyes(50);
      mode++;
      break;
      
    case 16:
      sleepNow();                          //Call function and wait for a push button interrupt
      mode++;                              //Increase the mode value to switch to the next case
      break;        
      
    case 17:
      FireFly(100);
      mode++;
      break;  
  
    case 18:
      FireFly(75);
      mode++;
      break;
    
    case 19:
      FireFly(50);
      mode = 0;                            //Reset mode back to zero to re-start the switch from the top
      break;   
*/  
}
 
  delay(500);                              //Debounce the button press
  inputVal = 0;                            //Reset the button state
}

/*************************WAKE UP SUBROUTINE***************************************/

void wakeUp(){                             //ISR -> Interrupt Service Routine, called when button is pressed

  inputVal = 1;                            //Change the state of the Button Press in order to exit whatever loop is running
}

/*************************SLEEP NOW SUBROUTINE***************************************/

void sleepNow(){                            //A subroutine that puts the Arduino to sleep and turns off the ADC
 
  digitalWrite(0, LOW); 
  digitalWrite(1, LOW);
  digitalWrite(4, LOW);                    //Turn off all the LEDs
  
  cbi(ADCSRA,ADEN);                        //Switch ADC OFF
     
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);     //Sleep mode is set here
    
  sleep_enable();                          //Enables the sleep bit in the mcucr register so sleep is possible. just a safety pin
                            
  sleep_mode();                            //Here the device is actually put to sleep!! THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP FROM INTERRUPT
                           
  sleep_disable();                         //First thing after waking from sleep: disable sleep
                            
  sbi(ADCSRA,ADEN);                        //Switch ADC ON
}

/*************************BLINK ALL SUBROUTINE***************************************/
 
void BlinkAll(unsigned int v){             //Blinks all the LEDs at a rate specified in the main loop
    
  while(1) {                               //Continue to inifitely run this pattern until the button interrupts the loop

    if (inputVal > 0)  break;              //Check for Button Press  
  
    digitalWrite(0, HIGH);
    digitalWrite(1, HIGH);
    digitalWrite(4, HIGH);                 //Turn on all the LEDs
    
    for (int m = v; m > 0; m--) {
      
        if (inputVal > 0)  break;          //Check for IR input 
          
      delay(2); 
    }
    
    digitalWrite(0, LOW); 
    digitalWrite(1, LOW);
    digitalWrite(4, LOW);                  //Turn off all the LEDs
    
    for (int m = v; m > 0; m--) {
      
        if (inputVal > 0)  break;          //Check for Button Press
      
      delay(2); 
    }
  }
}

/*************************METRONOME SUBROUTINE***************************************/
  
void Metronome(unsigned int w){           //LEDs bounce back and forth emulating a Metronome
     
  while(1) {                               //Continue to inifitely run this pattern until the button interrupts the loop
     
    if (inputVal > 0)  break;              //Check for Button Press
     
    digitalWrite(1, LOW);
    digitalWrite(4, LOW);
    digitalWrite(0, HIGH);                 //LED3 on, rest off
    
    for (int m = w; m > 0; m--) {
      
        if (inputVal > 0)  break;          //Check for Button Press
        
      delay(2); 
    } 
     
    digitalWrite(0, LOW);
    digitalWrite(1, HIGH);                 //LED2 on, rest off
    
    for (int m = w; m > 0; m--) {
      
        if (inputVal > 0)  break;          //Check for Button Press
        
      delay(1); 
    }
    
    digitalWrite(1, LOW);
    digitalWrite(4, HIGH);                 //LED1 on, rest off
     
    for (int m = w; m > 0; m--) {
      
        if (inputVal > 0)  break;          //Check for Button Press
        
      delay(2); 
    } 
     
    digitalWrite(4, LOW);
    digitalWrite(1, HIGH);                 //LED2 on, rest off
    
    for (int m = w; m > 0; m--) {
      
        if (inputVal > 0)  break;          //Check for Button Press
        
      delay(1); 
    }
    
    digitalWrite(1, LOW);
  }
}
 
 /*************************DISCO SUBROUTINE***************************************/
  
void Disco(unsigned int x){                //LEDs chase each other in a triangular pattern
    
   while(1) {                              //Continue to inifitely run this pattern until the button interrupts the loop
     
    if (inputVal > 0)  break;              //Check for Button Press
     
    digitalWrite(1, LOW);
    digitalWrite(4, LOW);
    digitalWrite(0, HIGH);                 //LED3 on, rest off
    
    for (int m = x; m > 0; m--) {
          
        if (inputVal > 0)  break;          //Check for Button Press
        
      delay(1); 
    } 
    
    digitalWrite(0, LOW);                  //LED3 off
    
    for (int m = x; m > 0; m--) {
      
        if (inputVal > 0)  break;          //Check for Button Press
        
      delay(1); 
    } 
   
    digitalWrite(1, HIGH);                 //LED2 on
   
    for (int m = x; m > 0; m--) {
      
        if (inputVal > 0)  break;          //Check for Button Press
        
      delay(1); 
    }
    
    digitalWrite(1, LOW);                  //LED2 off
    
    for (int m = x; m > 0; m--) {
      
        if (inputVal > 0)  break;          //Check for Button Press
       
      delay(1); 
    }
    
    digitalWrite(4, HIGH);                 //LED1 on
    
    for (int m = x; m > 0; m--) {
      
        if (inputVal > 0)  break;          //Check for Button Press
        
      delay(1); 
    }
      
    digitalWrite(4, LOW);                  //LED1 off
      
    for (int m = x; m > 0; m--) {
      
        if (inputVal > 0)  break;          //Check for Button Press
        
    delay(1); 
    } 
  }
}
  
/*************************FADING EYES SUBROUTINE***************************************/
  
void FadingEyes(unsigned int y){           //LEDs fade like the creepy eyes of a robot
   
  while(1){                                //Continue to inifitely run this pattern until the button interrupts the loop
  
      if (inputVal > 0)  break;            //Check for Button Press
   
    digitalWrite(0, LOW); 
    digitalWrite(1, LOW);
    digitalWrite(4, LOW);                  //All LEDs off
  
    j = 0;
    
    while ( j < y){                        //Keep LEDs off for a certain delay
      
        if (inputVal > 0)  break;          //Check for Button Press
        
      delay(1);
      j++;
    }
 
    j = 0;

    while (j < y) {                        //Pulse up LED1 & LED3
  
        if (inputVal > 0)  break;          //Check for Button Press
   
        for (int n = 1; n + j > 0; n--){ 
         
            if (inputVal > 0)  break;      //Check for Button Press
           
          digitalWrite(0, HIGH);
          digitalWrite(4, HIGH);           //LED1 & LED3 on
          delayMicroseconds(100); 
         }
     
        for (int m = 0; m + j < y; m++){
       
            if (inputVal > 0)  break;      //Check for Button Press
            
          digitalWrite(0, LOW); 
          digitalWrite(4, LOW);            //LED1 & LED3 off
          delayMicroseconds(100); 
        }
     
      j++;
    }
 
    j = 0;
 
    digitalWrite(0, HIGH);
    digitalWrite(4, HIGH);                 //LED1 & LED3 on
 
    while ( j < y){                        //Keep LEDs on for a certain delay
        
        if (inputVal > 0)  break;          //Check for Button Press
        
      delay(11);
      j++;
    }
 
    j = 0;

    while (j < y) {                        //Fade down LED1 & LED3
   
        if (inputVal > 0)  break;          //Check for Button Press

        for (int n = 1; n + j > 0; n--){
          
            if (inputVal > 0)  break;      //Check for Button Press
            
          digitalWrite(0, LOW); 
          digitalWrite(4, LOW);            //LED1 & LED3 off
          delayMicroseconds(1000); 
        }
    
        for (int m = 0; m + j < y; m++){
          
            if (inputVal > 0)  break;      //Check for Button Press
          
          digitalWrite(0, HIGH);
          digitalWrite(4, HIGH);           //LED1 & LED3 on
          delayMicroseconds(1000); 
        }
     
      j++;
    }
  }
}

/*************************FIRE FLY SUBROUTINE***************************************/
  
void FireFly(unsigned int z){              //Randomly selected LEDs Pulse and fade like Fire flies

 del = 25;
    lop = 100;
    lop2 = 100;
control = 0;
   
  while(1) {                               //Continue to inifitely run this pattern until the button interrupts the loop
     
//      if (inputVal > 0)  break;            //Check for Button Press
    
    digitalWrite(0, LOW); 
    digitalWrite(1, LOW);
    digitalWrite(4, LOW);                  //All LEDs off
    
   
    
    randNumber = random(20);               //Chooses a random number from 0-20

      if (randNumber >=0 && randNumber < 7 ){       //If the random number is between 0-6 choose LED3
    
        ledPin = 0;
      }

      else if (randNumber >=7 && randNumber < 14 ){ //Or else if the random number is between 7-13 choose LED2
   
        ledPin = 1;
      }
      
      else {                                        //Or if the random number is between 14-20 choose LED1
   
        ledPin = 4;
      } 
/*  
    j = 0;
    digitalWrite(ledPin, LOW);             //Turn LED off
   
      while ( j < z){                      //Keep LEDs off for a certain delay
    
          if (inputVal > 0)  break;        //Check for Button Press
        
        delay(5);
        j++;
      }
*/   
    j = 0;

      while (j < lop) {                    //Pulse LED up
      
 //         if (inputVal > 0)  break;        //Check for Button Press
      
          for (int n = 1; n + j > 0; n--){
            
              if (inputVal > 0)  break;    //Check for Button Press
              
            digitalWrite(ledPin, HIGH);    //LED on
            delayMicroseconds(del); 
          }
    
          for (int m = 0; m + j < lop2; m++){
            
              if (inputVal > 0)  break;    //Check for Button Press
              
            digitalWrite(ledPin, LOW);     //LED off
            delayMicroseconds(del); 
          }
      
        j++;
      }
/* 
    j = 0;
    digitalWrite(ledPin, HIGH);             //Turn LED on
      
      while ( j < z){                       //Keep LEDs on for a certain delay
        
          if (inputVal > 0)  break;         //Check for Button Press
          
        delay(11);
        j++;
      }
*/    
    j = 0;
 
      while (j < lop) {                      //Fade LED down
     
          if (inputVal > 0)  break;          //Check for Button Press 
     
          for (int n = 1; n + j > 0; n -= 1){
            
              if (inputVal > 0)  break;      //Check for Button Press
              
            digitalWrite(ledPin, LOW);       //LED off
            delayMicroseconds(del); 
          }
    
          for (int m = 0; m + j < lop2; m += 1){
             
              if (inputVal > 0)  break;      //Check for Button Press
              
            digitalWrite(ledPin, HIGH);      //LED on
            delayMicroseconds(del); 
          }
      
        j++;
      }
      
      
 IRReader();
  
//    control++;
    
  if (control == 1){
    del = 200;
  }  
      
      
  if (control == 2){
    del = 400;
  } 
      
/*      
      lop2++;
      lop++;
      del++;
*/
  }
}

// ************** IR Reader subroutine

void IRReader(void)
{              //Listen to IR Reader, D3, change control value
   
  while(1) {                               //Continue to inifitely run this pattern until the button interrupts the loop

//    control = 0;
    
    IRRead = digitalRead(3); 
    
    if (IRRead == HIGH){
      control = 1;
    } 
    if (IRRead == LOW){
      control = 2;
    }
    break;
  }
}
