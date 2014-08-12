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
 
// SoftwareSerial Serial(1,0);      // Receive, Transmit (Receive not used)
SendOnlySoftwareSerial Serial(0);   // Transmit serial on Trinket/Gemma pin GPIO #0/D0
 
// We need to use the 'raw' pin reading methods because timing is very important here 
// and the digitalRead() procedure is slower!
#define IRpin_PIN PINB // ATTiny85 had Port B pins
#define IRpin 2
 
#define MAXPULSE    5000  // the maximum pulse we'll listen for - 5 milliseconds 
#define NUMPULSES    100  // max IR pulse pairs to sample
#define RESOLUTION     2  // // time between IR measurements
 
// we will store up to 100 pulse pairs (this is -a lot-)
uint16_t pulses[100][2]; // pair is high and low pulse
uint16_t currentpulse = 0; // index for pulses we're storing
uint32_t irCode = 0;
 
void setup(void) {
  Serial.begin(9600);
  Serial.println("Ready to decode IR!");
  pinMode(IRpin, INPUT);   // Listen to IR receiver on Trinket/Gemma pin D2
}
 
void loop(void) {
  
  uint16_t numpulse=listenForIR(); // Wait for an IR Code

  // Process the pulses to get a single number representing code
  for (int i = 0; i < 32; i++) {
    irCode=irCode<<1;
    if((pulses[i][0] * RESOLUTION)>0&&(pulses[i][0] * RESOLUTION)<500) {
      irCode|=0; 
    } else {
      irCode|=1;
    }
  }
  
  printcode();  // Print IR code to softwareserial
  
}
 
void printcode(void) {
  uint16_t half;
  half=irCode>>16;  // Get first 16 bits of code
  Serial.print("0x");
  Serial.print(half, HEX);  // Print upper 16 bits in hex
  Serial.println(irCode & 0xFFFF, HEX); // print lower 16 bits in hex
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
   }x
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
