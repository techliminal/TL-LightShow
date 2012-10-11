
/*
  Firely IR Reader test code 
    The circuit:
    * LED1 is attached from digital pin 3 (D4) to the 47 ohm resistot then to GND
    * LED2 is attached from digital pin 6 (D1) to the 47 ohm resistor then to GND
    * LED3 is attached from digital pin 5 (D0) to the 47 ohm resistor then to GND
    * The button is attached from digital pin 7 (D2) to GND
    * IR recvr attached to D3
 
   Forked by Anca Mosoiu 10/5/2012
   
   
    Original Firefly code by  Jeremie Boulianne for Solarbotics Ltd.
 
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


// We need to use the 'raw' pin reading methods
// because timing is very important here and the digitalRead()
// procedure is slower!
#define IRpin_PIN      3
#define IRpin          3

int IRRead;          //Raw IR Reading

int LED1 = 4;
int LED2 = 1;
int LED3 = 0;
int del;              //delay value (ms)
int lop;              //loop value
int lop2;            //loop value 2
int control = 0;        //Control state
int ledPin;
int randNumber;
int j;                //firefly internal counter
void setup() {
  pinMode(LED1, OUTPUT);                      //Create (D4) LED1 as an output
  pinMode(LED2, OUTPUT);                      //Create (D1) LED2 as an output 
  pinMode(LED3, OUTPUT);                      //Create (D0) LED3 as an output 
  pinMode(IRpin, INPUT);                       //Create D3 as IR input
  randomSeed(3);                           //Not so random seed
}

void loop(){
  SimpleIRRead();
  FireFly(50);
  
}
  
// ************** Simple IR Reader

void SimpleIRRead(void){
  
  if (!digitalRead(IRpin)) {  // This is low-sensor, and is normally off
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    control = 1;
  }
  else {  // This is high-sensor, and is normally on
      // pin is still HIGH
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
  }
  
 /*  if (! (IRpin_PIN & _BV(IRpin))) {  // This is low-sensor, and is normally off
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
  }
   if (IRpin_PIN & (1 << IRpin)) {  // This is high-sensor, and is normally on
      // pin is still HIGH
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
  } 
  
  */
  delay(5000);
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
            
 //             if (inputVal > 0)  break;    //Check for Button Press
              
            digitalWrite(ledPin, HIGH);    //LED on
            delayMicroseconds(del); 
          }
    
          for (int m = 0; m + j < lop2; m++){
            
//              if (inputVal > 0)  break;    //Check for Button Press
              
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
     
//          if (inputVal > 0)  break;          //Check for Button Press 
     
          for (int n = 1; n + j > 0; n -= 1){
            
//              if (inputVal > 0)  break;      //Check for Button Press
              
            digitalWrite(ledPin, LOW);       //LED off
            delayMicroseconds(del); 
          }
    
          for (int m = 0; m + j < lop2; m += 1){
             
 //             if (inputVal > 0)  break;      //Check for Button Press
              
            digitalWrite(ledPin, HIGH);      //LED on
            delayMicroseconds(del); 
          }
      
        j++;
      }
      
      
// IRReader();
  
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
  
  return;

}

