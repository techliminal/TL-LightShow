/* Trinket/Gemma compatible Raw IR decoder sketch
This sketch/program uses an Adafruit Trinket or Gemma
ATTiny85 based mini microcontroller and a PNA4602 to
decode IR received. This can be used to make a IR receiver
(by looking for a particular code) or transmitter 
(by pulsing an IR LED at ~38KHz for the durations detected

Based on Adafruit tutorial http://learn.adafruit.com/ir-sensor/using-an-ir-sensor

and ATTiny program by TinyPCRemote Nathan Chantrell http://nathan.chantrell.net
under Creative Commons Attribution-ShareAlike 3.0 Unported (CC BY-SA 3.0) license

SendSoftwareSerial Lirary modification by Nick Gammon from NewSoftwareSerial code
GNU Lesser General Public License as published by the Free Software Foundation version 2.1 
at http://gammon.com.au/Arduino/SendOnlySoftwareSerial.zip
*/
#define IRpin_PIN PINB // ATTiny85 had Port B pins
#define IRpin 3

#define LED1 1 
#define LED2 4
#define LED3 0
 
#define MAXPULSE    5000  // the maximum pulse we'll listen for - 5 milliseconds 
#define NUMPULSES    30  // max IR pulse pairs to sample
#define RESOLUTION     2  // // time between IR measurements
 
// we will store up to 100 pulse pairs (this is -a lot-)
uint16_t pulses[30][2]; // pair is high and low pulse
uint16_t currentpulse = 0; // index for pulses we're storing
uint32_t irCode = 0;
 
void setup(void) {
  pinMode(IRpin, INPUT);   // Listen to IR receiver on Trinket/Gemma pin D2
  pinMode(LED1, OUTPUT);                      //Create (D4) LED1 as an output
  pinMode(LED2, OUTPUT);                      //Create (D1) LED2 as an output 
  pinMode(LED3, OUTPUT);                      //Create (D0) LED3 as an output 
  randomSeed(analogRead(0));                           //Not so random seed
  
  for(int i=0; i< 3; i++){
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, HIGH);
      delay(500);
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      delay(500);
  }
  
}
 
void loop(void) {
  
  uint16_t numpulse=listenForIR(); // Wait for an IR Code

  // Process the pulses to get a single number representing code
  for (int i = 0; i < NUMPULSES; i++) {
    irCode=irCode<<1;
    if((pulses[i][0] * RESOLUTION)>0&&(pulses[i][0] * RESOLUTION)<500) {
      irCode|=0; 
    } else {
      irCode|=1;
    }
  }
  
  dostuff();  // Print IR code to softwareserial
  

}
void dostuff(void) {
  uint16_t half;
  uint16_t less;
  
  half=irCode>>16;  // Get first 16 bits of code
  less = irCode & 0xFFFF;  // the second 16 bits
  
  if (less > 0xE000) return;
  
  
  if (less > 0 && less < 0x1350){
    all_on();
  } else if (less > 0x1350 && less < 0x1520){
      FireFly(6000);
  } else if (less > 0x1520 && less < 0x1620){
     all_off(); 
  } else if (less > 0x1620 && less <= 0x1A0D){
     FireFly(2000);
  }
}



uint16_t listenForIR() {  // IR receive code
  currentpulse = 0;
  while (1) {
   unsigned int highpulse, lowpulse;  // temporary storage timing
   highpulse = lowpulse = 0; // start out with no pulse length 
  
   while (IRpin_PIN & _BV(IRpin)) { // got a high pulse
      highpulse++; 
      delayMicroseconds(RESOLUTION);
      if (((highpulse >= MAXPULSE) && (currentpulse != 0))|| currentpulse == NUMPULSES) {
        return currentpulse; 
      }
   }
   pulses[currentpulse][0] = highpulse;

   while (! (IRpin_PIN & _BV(IRpin))) { // got a low pulse
      lowpulse++; 
      delayMicroseconds(RESOLUTION);
      if (((lowpulse >= MAXPULSE) && (currentpulse != 0))|| currentpulse == NUMPULSES) {
        return currentpulse; 
      }
   }
   pulses[currentpulse][1] = lowpulse;
   currentpulse++;
  }
}

void blink4(){
    for(int i=0; i< 3; i++){
      digitalWrite(LED1, HIGH);
      delay(500);
      digitalWrite(LED1, LOW);
      delay(500);
  }
}

/*************************FIRE FLY SUBROUTINE***************************************/
  
void FireFly(unsigned int z){              //Randomly selected LEDs Pulse and fade like Fire flies

  del = 150;
  lop = 100;
   
  while(1) {                               //Continue to inifitely run this pattern until the button interrupts the loop
         
    digitalWrite(LED1, LOW); 
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);                  //All LEDs off
    
   led1 =  led2 = led3 = LOW;
    // pick a duration for the cycle, and then loop.  Duration should be a number of seconds, not less
    
    
    // decide whether a given light will be on or off.  Each light will have a modifier.
   
   int half = z/2;
   randomSeed(analogRead(2));
   //int min1 = random(z);
   int min1 = random(half);
   int min2 = random(half);
   int min3 = random(half);
 
   int max1 = z-min1;
   int max2 = z-min2;
   int max3 = z-min3;
   
   for (int i = 0; i<z+lop; i++){
       if (i == min1){
           led1 = HIGH;
           pulse_up(lop, LED1);
       }
       if (i == min2){
           led2=HIGH;
           pulse_up(lop, LED2);
       }
       if(i == min3){
         led3=HIGH;
         pulse_up(lop, LED3);    
       }
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
      digitalWrite(LED3, led3);                  //All LEDs off or on
      delay(1);
     }
     delay(1000); 
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

void all_on(){
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
}

void all_off(){
   digitalWrite(LED1, LOW); 
   digitalWrite(LED2, LOW);
   digitalWrite(LED3, LOW);
}
