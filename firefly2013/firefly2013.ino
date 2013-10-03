// a git comment
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
   Forked once more by Anca Mosoiu 08 Aug 2013 (one year LATER!)
 
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


int wakePin = 0;                           //Interrupt 0 which is the button on DIGITAL PIN 2!
//int wakePin = 1;                           //Interrupt 1 which is the IR sensor on DIGITAL PIN 3!
int mode = 1;                              //Mode value for switch 
volatile int inputVal = 0;                 //Button Press which must be called as a volatile variable because it is used in an Interrupt Service Routine
int j;                                     //Counter value

int del;              //delay value (ms)
int lop;              //loop value

int LED1 = 4;
int LED2 = 1;
int LED3 = 0;

int led1, led2, led3;

void setup() {
  //Serial.begin(9600);
  //Serial.println("Ready to decode IR!");
  pinMode(LED1, OUTPUT);                      //Create (D4) LED1 as an output
  pinMode(LED2, OUTPUT);                      //Create (D1) LED2 as an output 
  pinMode(LED3, OUTPUT);                      //Create (D0) LED3 as an output 
  randomSeed(3);                           //Not so random seed
  //attachInterrupt(wakePin, wakeUp, HIGH);   //Attach the interrupt to the IR Sensor on D3 and execute the ISR on a LOW signal
}

void loop(){

  mode=1;       // Force a mode
   
  switch(mode){                            //Switch case statement which check for which mode the program needs to be in
  
    case 0:
      sleepNow();                          //Call function and wait for a push button interrupt
      //mode++;                              //Increase the mode value to switch to the next case
      break;                               //Exit switch case
       
    case 1:
      FireFly(20000);
      //mode++;
      break;   
       
    case 2:
      FireFly(10);
      //mode++;
      delay(2000);
      break;
    case 3: // reset to 0
      mode=0; 
}
 
  delay(500);                              //Debounce the button press
  inputVal = 0;                            //Reset the button state
}

/*************************WAKE UP SUBROUTINE***************************************/

void wakeUp(){                             //ISR -> Interrupt Service Routine, called when button is pressed

  inputVal = 1;                            //Change the state of the Button Press in order to exit whatever loop is running
  mode++;
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


/*************************FIRE FLY SUBROUTINE***************************************/
  
void FireFly(unsigned int z){              //Randomly selected LEDs Pulse and fade like Fire flies

  del = 150;
  lop = 100;
   
  while(1) {                               //Continue to inifitely run this pattern until the button interrupts the loop
     
    if (inputVal > 0)  {
      break;            //Check for Signal
    }
    
    digitalWrite(LED1, LOW); 
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);                  //All LEDs off
    
   led1 =  led2 = led3 = LOW;
    // pick a duration for the cycle, and then loop.  Duration should be a number of seconds, not less
    
    
    // decide whether a given light will be on or off.  Each light will have a modifier.
   
   //int max1 = random(z);
   int max1 = random(z-1000);
   int max2 = random(z-1000);
   int max3 = random(z-1000);
  
   for (int i = 0; i<z+lop; i++){
       if (i == max1){
           led1 = HIGH;
           pulse_up(lop, LED1);
       }
       if (i == max2){
           led2=HIGH;
           pulse_up(lop, LED2);
       }
       if(i == max3){
         led3=HIGH;
         pulse_up(lop, LED3);    
       }
      digitalWrite(LED1, led1); 
      digitalWrite(LED2, led2);
      digitalWrite(LED3, led3);                  //All LEDs off
      delay(1);
     }
     for (int i = 0; i<z+lop; i++){
       if (i == max1){
           led1 = LOW;
           pulse_down(lop, LED1);
       }
       if (i == max2){
           led2=LOW;
           pulse_down(lop, LED2);
       }
       if(i == max3){
         led3=LOW;
         pulse_down(lop, LED3);    
       }
      digitalWrite(LED1, led1); 
      digitalWrite(LED2, led2);
      digitalWrite(LED3, led3);                  //All LEDs off
      delay(1);
     }
  }
}

void pulse_up(int lop, byte led){
     
  j = 0;

  digitalWrite(led, LOW);
  
  while (j < lop) {                    //Pulse LED up
  
      if (inputVal > 0)  break;        //Check for Button Press
  
      for (int n = 1; n + j > 0; n--){
        
          if (inputVal > 0)  break;    //Check for Button Press
          
        digitalWrite(led, HIGH);    //LED on
        delayMicroseconds(10); 
      }

      for (int m = 0; m + j < lop; m++){
        
          if (inputVal > 0)  break;    //Check for Button Press
          
        digitalWrite(led, LOW);     //LED off
        delayMicroseconds(10); 
      }
  
    j++;
  }
}

void pulse_down(int lop, byte ledPin){
      j = 0;
    digitalWrite(ledPin, HIGH);             //Turn LED on
      
      while ( j < lop){                       //Keep LEDs on for a certain delay
        
          if (inputVal > 0)  break;         //Check for Button Press
          
        delay(11);
        j++;
      }
   
    j = 0;
 
      j = 0;
 
      while (j < lop) {                      //Fade LED down
     
          if (inputVal > 0)  break;          //Check for Button Press 
     
          for (int n = 1; n + j > 0; n -= 1){
            
              if (inputVal > 0)  break;      //Check for Button Press
              
            digitalWrite(ledPin, LOW);       //LED off
            delayMicroseconds(10); 
          }
    
          for (int m = 0; m + j < lop; m += 1){
             
              if (inputVal > 0)  break;      //Check for Button Press
              
            digitalWrite(ledPin, HIGH);      //LED on
            delayMicroseconds(10); 
          }
      
        j++;
      }
} 

