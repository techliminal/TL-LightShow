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

#include <NewPing.h>
 
#define TRIGGER_PIN  8
#define ECHO_PIN     9
#define MAX_DISTANCE 400
//#define TIMER_ENABLED false

#define IR_PIN 3

char command = '1';
int dist = 0;
int prevDist = 0;

//SharpIR sharp(ir, 25, 93, model);
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// we need firefly, mode 3

enum {
    CMD_LEN = 32,
    GREEN   = 0x77E15061,  // works, all green, mode 2
    PREV    = 0x77E10C68,  
    BLUE    = 0x77E1A05E,  //works, blue, mode 4
    AOFF    = 0x77E11A00,  // works, off, mode 1
    NEXT    = 0x77E116AA,
    FIREFLY    = 0x77E15C5E //works, firefly, mode 3
  };



IRsend irsend;

void send_command(long command){
    int i;
    for (i=0; i<40; i++){
      irsend.sendNEC(command, 32);
      Serial.write(command);
      delay(17);
    } 
    Serial.write('...');
}

void setup()
{
  Serial.begin(9600);

}

void loop() {

  dist = check_distance();
  
  if (dist > prevDist){
    command = '2';
  } else if (dist == prevDist){
    command = '3';
  } else {
    command = '4';
  }

  prevDist = dist;

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
    Serial.print("Mode 4 / pulse blue");
    send_command(BLUE);
       break;
    case '5':  // 
     Serial.print("mode 5 / ??");
      send_command(NEXT);
      break;    
   case '6': 
    Serial.print("mode 6 /??");
    send_command(PREV);
      break;
    default:
      Serial.println('no change');
  }
}

int check_distance(){

  int d = 0;
  int uS;
  
  for(int i=0; i<10; i++){
   uS = sonar.ping();
    d += uS;
    delay(2);
  }
  d = floor(d/10);
  Serial.print("Ping: ");
  Serial.print(d / US_ROUNDTRIP_CM);
  Serial.print("cm  |   ");
  return d;
}


