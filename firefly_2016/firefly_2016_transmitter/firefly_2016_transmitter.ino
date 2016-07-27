/*
 * firefly_2016_transmitter: Send IR Codes to the fireflies.
 * 
 * (C)2016 Anca Mosoiu and Michael Schrecker, Garaj Luminat
 * Source: https://learn.adafruit.com/using-an-infrared-library/sending-ir-codes
 * IRLib Documentation: http://tech.cyborg5.com/irlib/docs/
 * IR codes: https://hifiduino.wordpress.com/apple-aluminum-remote/
 */

#include <IRremote.h>

enum {
    CMD_LEN = 32,
    UP      = 0x77E15061,
    DOWN    = 0x77E13061,
    PLAY    = 0x77E1A05E,
    PREV    = 0x77E1905E,
    NEXT    = 0x77E1605E,
    MENU    = 0x77E1C05E
  };



IRsend irsend;

void send_command(long command){
    int i;
    for (i=0; i<10; i++){
      irsend.sendNEC(PLAY, 32);
    } 
    delay(20);
}

void setup()
{
  Serial.begin(9600);
}

void loop() {
  const char command = Serial.read();
  switch(command) {
    case 'p':
     send_command(UP); // all blue
     break;
    case 'o':
      send_command(PLAY);
      break;    
    case 'i':
      send_command(PREV);
      break;
    case 'u':
      send_command(NEXT);
      break;
  }
}
