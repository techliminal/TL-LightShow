/*
 * firefly_2016_transmitter: Send IR Codes to the fireflies.
 * 
 * (C)2016 Anca Mosoiu and Michael Schrecker, Garaj Luminat
 * Source: https://learn.adafruit.com/using-an-infrared-library/sending-ir-codes
 * IRLib Documentation: http://tech.cyborg5.com/irlib/docs/
 * IR codes: https://hifiduino.wordpress.com/apple-aluminum-remote/
 *
 * Rest state:  Measure a certain amount of time when it's not doing anything.
 */

#include <IRremote.h>

//#include <SharpIR.h>

//#define ir A0
//#define model 1080

#include <NewPing.h>
 
#define TRIGGER_PIN  8
#define ECHO_PIN     9
#define MAX_DISTANCE 200
//#define TIMER_ENABLED false

//SharpIR sharp(ir, 25, 93, model);
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// we need firefly, mode 3

enum {
    CMD_LEN = 32,
    GREEN   = 0x77E15061,  // works, pulse green, mode 2
    AOFF    = 0x77E10000, // original value 0x77E10C68,  works, off, mode 1
    BLUE    = 0x77E1A05E,  //works, mode 5
    PREV    = 0x77E11F00, // original value 0x77E11A00,  mode 6
    NEXT    = 0x77E116AA,  // mode 4
    FIREFLY    = 0x77E15C5E //works, firefly, mode 3
  };



IRsend irsend;

void send_command(long command){
    int i;
    for (i=0; i<40; i++){
      irsend.sendNEC(command, 32);
//      Serial.write('s');
      delay(17);
    } 
    Serial.write('.');
}

void setup()
{
  Serial.begin(9600);
//  pinMode (ir, INPUT);
}

void loop() {

  int uS = sonar.ping();
  Serial.print("Ping: ");
  Serial.print(uS / US_ROUNDTRIP_CM);
  Serial.println("cm");

  
  const char command = Serial.read();
//   const char command = input_measure();
       Serial.print(command);  
     Serial.println("");

  switch(command) {
    case '1':   // off.  Works
     Serial.print("mode 1 / off");
      send_command(AOFF);
     break;
    case '2':   // pulse green
     Serial.print("mode 2 / pulse green");
     send_command(GREEN); 
     break;
    case '3':  //firefly
     Serial.print("mode 3 / firefly");
      send_command(FIREFLY);
     break;
    case '4':  //
    Serial.print("Mode 4 / blink blue");
    send_command(NEXT);
       break;
    case '5':  // 
     Serial.print("mode 5 / blink green");
      send_command(BLUE);
      break;    
   case '6': 
    Serial.print("mode 6 / pulse blue");
    send_command(PREV);
      break;
  }
}

/*
char input_measure(){
  char input = 'u';
  
       Serial.print("measuring...");
       
  int dis=sharp.distance();
       Serial.print(dis);  
       
  if (dis > 50){
     input = 'i';
     Serial.print("i");  
     Serial.println("");
 }
  else if (dis > 30  && dis < 50){
     input = 'o';
      Serial.print("o");  
     Serial.println("");
 }
  else if (dis > 15 && dis <30){
     input = 'p';
     Serial.print("p");  
     Serial.println("");
  }
  else {
     input = 'u';
      Serial.print("u");  
     Serial.println("");
  }
  Serial.print(input);
  return input;
}

*/
