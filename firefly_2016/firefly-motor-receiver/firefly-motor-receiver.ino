/* Example program for from IRLib – an Arduino library for infrared encoding and decoding
 * Version 1.3   January 2014
 * Copyright 2014 by Chris Young http://cyborg5.com
 * Based on original example sketch for IRremote library 
 * Version 0.11 September, 2009
 * Copyright 2009 Ken Shirriff
 * http://www.righto.com/
 */
/*
 * IRhashdecode - decode an arbitrary IR code.
 * Instead of decoding using a standard encoding scheme
 * (e.g. Sony, NEC, RC5), the code is hashed to a 32-bit value.
 * This should produce a unique 32-bit number however that number cannot be used
 * to retransmit the same code. This is just a quick and dirty way to detect a unique code
 * for controlling a device when you don't really care what protocol or values
 * are being sent.
 */

#include <IRLib.h>

int RECV_PIN = 8;
byte mode = 0;

#define MOTOR1 2

IRrecv My_Receiver(RECV_PIN);
IRdecode My_Decoder;
IRdecodeHash My_Hash_Decoder;

void setup()
{
  My_Receiver.enableIRIn(); // Start the receiver
  Serial.begin(9600);
  delay(2000);while(!Serial);//delay for Leonardo
  
  // set up the motor pins
  pinMode(MOTOR1, OUTPUT);                      //Create (D4) MOTOR1 as an output
  /*pinMode(MOTOR2, OUTPUT);                      //Create (D4) MOTOR1 as an output
  pinMode(MOTOR3, OUTPUT);                      //Create (D4) MOTOR1 as an output
  pinMode(MOTOR4, OUTPUT);                      //Create (D4) MOTOR1 as an output
  pinMode(MOTOR5, OUTPUT);                      //Create (D1) MOTOR2 as an output 
  pinMode(MOTOR6, OUTPUT);     */                 //Create (D0) MOTOR3 as an output 
  randomSeed(analogRead(0));                           //Not so random seed

}

void loop() {
  if (My_Receiver.GetResults(&My_Decoder)) {//Puts results in My_Decoder
    My_Hash_Decoder.copyBuf(&My_Decoder);//copy the results to the hash decoder
    My_Decoder.decode();
    Serial.print("real decode type:");
    Serial.print(Pnames(My_Decoder.decode_type));
    Serial.print(" value: 0x");
    Serial.print(My_Decoder.value, HEX);
    My_Hash_Decoder.decode();
    Serial.print(", hash decode: 0x");
    Serial.println(My_Hash_Decoder.hash, HEX); // Do something interesting with this value
    
    mode = getMode(My_Decoder.value);
    
    switch(mode){
        case 1:
          buzz_one();
          break;
        case 2:
          buzz_two();
          break;
        default:
          all_off();
    }
    
    delay(1000);
    My_Receiver.resume(); 
  }
}

byte getMode(int value){
   if (value < 0x77E10C00){
     return 1;
   } else if (value >= 0x77E10C00 && value < 0x77E1A05E){
      return 2; 
   }
}

void buzz_one(){
   int pause = random(1000);
   for(byte i=0; i<random(10) ; i++){
     digitalWrite(MOTOR1, HIGH);
     delay(pause);
     digitalWrite(MOTOR1, LOW);
     delay(pause);
   }
}

void buzz_two(){
   int pause = random(500);
   for(byte i=0; i<random(10) ; i++){
     digitalWrite(MOTOR1, HIGH);
     delay(30);
     digitalWrite(MOTOR1, LOW);
     delay(30);  
      digitalWrite(MOTOR1, HIGH);
     delay(30);
     digitalWrite(MOTOR1, LOW);
     delay(pause);     
   }
}

void all_off(){
   digitalWrite(MOTOR1, LOW); 
}
