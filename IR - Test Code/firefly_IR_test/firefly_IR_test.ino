
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

void setup() {
  pinMode(LED1, OUTPUT);                      //Create (D4) LED1 as an output
  pinMode(LED2, OUTPUT);                      //Create (D1) LED2 as an output 
  pinMode(LED3, OUTPUT);                      //Create (D0) LED3 as an output 
  pinMode(IRpin, INPUT);                       //Create D3 as IR input
  randomSeed(3);                           //Not so random seed
}

void loop(){
  SimpleIRRead();
 
}
  
// ************** Simple IR Reader

void SimpleIRRead(void){
  
  if (!digitalRead(IRpin)) {  // This is low-sensor, and is normally off
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
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
  
}


