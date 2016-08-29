/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */


#include <SharpIR.h>

#define ir A0
#define model 20150

boolean done=false;


SharpIR sharp(ir, 25, 93, model);


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
    pinMode (ir, INPUT);
}

// the loop function runs over and over again forever
void loop() {
//  int del_timer = 200;
  int del_timer=sharp.distance();
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(del_timer);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(del_timer);              // wait for a second
}
