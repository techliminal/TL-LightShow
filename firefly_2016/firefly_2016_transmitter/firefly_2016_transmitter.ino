/*
 * firefly_2016_transmitter: Send IR Codes to the fireflies.
 * 
 * (C)2016 Anca Mosoiu and Michael Schrecker, Garaj Luminat
 * Source: https://learn.adafruit.com/using-an-infrared-library/sending-ir-codes
 *
 */

#include <IRremote.h>

IRsend irsend;

void setup()
{
  Serial.begin(9600);
}

void loop() {
  if (Serial.read() != -1) {
    for (int i = 0; i < 10; i++) {
      irsend.sendSony(0xa90, 12); // Sony TV power code
      delay(40);
    }
  }
}
