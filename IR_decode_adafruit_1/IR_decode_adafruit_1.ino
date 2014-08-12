/* Trinket/Gemma compatible IR read sketch
This sketch/program uses an Adafruit Trinket or Gemma
ATTiny85 based mini microcontroller and a PNA4602 or TSOP38238 to
read an IR code and perform a function.  In this
test program, tones are generated to a piezo speaker
but you can use codes to trigger any function you wish.
Based on Adafruit tutorial http://learn.adafruit.com/ir-sensor/using-an-ir-sensor
and http://learn.adafruit.com/trinket-gemma-mini-theramin-music-maker
*/
 
// We need to use the 'raw' pin reading methods because timing is very important here 
// and the digitalRead() procedure is slower!
#define IRpin_PIN  PINB // ATTiny85 had Port B pins
#define IRpin      2    // IR sensor - TSOP38238 on Pin GPIO #2 / D2
#define SPEAKERPIN 1    // Piezo speaker on Trinket/Gemma Pin GPIO #1/D1
 
#define MAXPULSE    5000  // the maximum pulse we'll listen for - 5 milliseconds 
#define NUMPULSES    100  // max IR pulse pairs to sample
#define RESOLUTION     2  // // time between IR measurements
 
// we will store up to 100 pulse pairs (this is -a lot-, reduce if needed)
uint16_t pulses[100][2];   // pair is high and low pulse
uint16_t currentpulse = 0; // index for pulses we're storing
uint32_t irCode = 0;
 
void setup() {
  pinMode(IRpin, INPUT);       // Listen to IR receiver on Trinket/Gemma pin D2
  pinMode(SPEAKERPIN, OUTPUT); // Output tones on Trinket/Gemma pin D1
  
  digitalWrite(SPEAKERPIN, HIGH);
  delay(500);
  digitalWrite(SPEAKERPIN, LOW);
  delay(500);
  digitalWrite(SPEAKERPIN, HIGH);
  delay(500);
  digitalWrite(SPEAKERPIN, LOW);
  delay(500);
  
}
 
void loop() {
  
  irCode=listenForIR(); // Wait for an IR Code

  // Process the pulses to get our code
  for (int i = 0; i < 32; i++) {
    irCode=irCode<<1;
    if((pulses[i][0] * RESOLUTION)>0&&(pulses[i][0] * RESOLUTION)<500) {
      irCode|=0; 
    } else {
      irCode|=1;
    }
  }
  if(irCode==0xdff86827)  {      // "1" on my remote, USE YOUR CODE HERE
      digitalWrite(SPEAKERPIN, HIGH);  }  // Make a 400 Hz beep
  else  { // "4", USE YOUR OWN HEX CODE HERE
     digitalWrite(SPEAKERPIN, LOW);  // Make a 700 Hz beep
  }

} // end loop
 

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

// Generate a tone on speakerPin - Trinket/Gemma/ATTiny85 compatible
void beep (unsigned char speakerPin, int frequencyInHertz, long timeInMilliseconds)
{	 // http://web.media.mit.edu/~leah/LilyPad/07_sound_code.html
          int x;	 
          long delayAmount = (long)(1000000/frequencyInHertz);
          long loopTime = (long)((timeInMilliseconds*1000)/(delayAmount*2));
          for (x=0;x<loopTime;x++)	 
          {	 
              digitalWrite(speakerPin,HIGH);
              delayMicroseconds(delayAmount);
              digitalWrite(speakerPin,LOW);
              delayMicroseconds(delayAmount);
          }	 
}
