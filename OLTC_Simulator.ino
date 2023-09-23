/*
  OLTC Emulator

*/
#include <Arduino.h>
#include "OLTCSimulator.h"

//==========Config binary outputs names ===================

//Config names for bcd output pins for BCD
const uint8_t BCD_0  = 2;
const uint8_t BCD_1  = 3;
const uint8_t BCD_2  = 4;
const uint8_t BCD_3  = 5;
const uint8_t BCD_4  = 6;
const uint8_t BCD_5  = 7; 

//Config names for end position 
const uint8_t Up_end = 8; //up end position
const uint8_t Down_end = 9; //down end position

//Config motion output
const uint8_t MotionPin = 10; //this output is activated when OLTC is in motion

//==========================================================

//==========Config binary inputs names =====================

const uint8_t Cmd_up = 14; //A0 input for up command 
const uint8_t Cmd_down = 15; //A1 input for down command
const uint8_t Cmd_e_stop = 16; //A2 input for emergency stop

//==========================================================


OLTC MotorDriver(BCD_0, BCD_1, BCD_2, BCD_3, BCD_4, BCD_5, Down_end, Up_end, MotionPin, Cmd_up, Cmd_down, Cmd_e_stop);





// the setup function runs once when you press reset or power the board
void setup() {

  pinMode(BCD_0, OUTPUT);
  pinMode(BCD_1, OUTPUT);
  pinMode(BCD_2, OUTPUT);
  pinMode(BCD_3, OUTPUT);
  pinMode(BCD_4, OUTPUT);
  pinMode(BCD_5, OUTPUT);

  pinMode(Up_end, OUTPUT);
  pinMode(Down_end, OUTPUT);
  pinMode(MotionPin, OUTPUT);

  pinMode(Cmd_down, INPUT);
  pinMode(Cmd_up, INPUT);
  pinMode(Cmd_e_stop, INPUT);

 //Pass parameter to object oltc motor driver
 // (Taps, Reverser, Time for chage the tap) 
 MotorDriver.getParameters(7,0,7000);
  
}

// the loop function runs over and over again forever
void loop() {
      
 MotorDriver.Monitoring();
  
}