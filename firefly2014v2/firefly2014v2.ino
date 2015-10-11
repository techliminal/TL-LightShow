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
// #install <SoftwareSerial.h>       // use if you do not wish to use the lightweight library 
#include <SendOnlySoftwareSerial.h>  // See http://forum.arduino.cc/index.php?topic=112013.0
 
 //Input IR Vaules
// apple + 0xFDF85427
//apple - 0xFDF84C27
//FFW 0xFBF8C84F

 
// SoftwareSerial Serial(1,0);      // Receive, Transmit (Receive not used)
SendOnlySoftwareSerial Serial(0);   // Transmit serial on Trinket/Gemma pin GPIO #0/D0
 
// We need to use the 'raw' pin reading methods because timing is very important here 
// and the digitalRead() procedure is slower!
#define IRpin_PIN PINB // ATTiny85 had Port B pins
#define IRpin 3

#define LED1 1 

 
#define MAXPULSE    5000  // the maximum pulse we'll listen for - 5 milliseconds 
#define NUMPULSES    30  // max IR pulse pairs to sample
#define RESOLUTION     2  // // time between IR measurements
 
// we will store up to 100 pulse pairs (this is -a lot-)
uint16_t pulses[30][2]; // pair is high and low pulse
uint16_t currentpulse = 0; // index for pulses we're storing
uint32_t irCode = 0;
 
void setup(void) {
  Serial.begin(9600);
  Serial.println("Ready to decode IR!");
  pinMode(IRpin, INPUT);   // Listen to IR receiver on Trinket/Gemma pin D2
  pinMode(LED1, OUTPUT);
  
  for(int i=0; i< 3; i++){
      digitalWrite(LED1, HIGH);
      delay(500);
      digitalWrite(LED1, LOW);
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
  
  Serial.print("--");
  //Serial.print(half, HEX);  // Print upper 16 bits in hex
  //Serial.println(irCode & 0xFFFF, HEX); // print lower 16 bits in hex
  
  Serial.println(less, HEX); // print lower 16 bits in hex
  
  if (less > 0 && less < 0x1350){
    all_on();
  } else if (less > 0x1350 && less < 0x1520){
      Serial.println("11111");
  } else if (less > 0x1520 && less < 0x1620){
     all_off(); 
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
      digitalWrite(LED1, HIGH);
      delay(500);
      digitalWrite(LED1, LOW);
      delay(500);
  }
}

void all_on(){
    digitalWrite(LED1, HIGH);
}

void all_off(){
   digitalWrite(LED1, LOW); 
}
