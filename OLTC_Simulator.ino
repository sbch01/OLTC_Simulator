/*
  OLTC Emulator

*/
#include <Arduino.h>

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


uint8_t Step = 1;


//---Temporary variables !!##$%^^
//????????????????????????????????????????????????????
uint16_t Counter = 0;
uint16_t MotionTime = 20000;
uint8_t UpTrigger = 0; 
uint8_t DownTrigger = 0;
//???????????????????????????????????????????????????


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

  

  
}



// the loop function runs over and over again forever
void loop() {
  
    OLTCmonitor();
 
    
       
}

//Update BCD Ports function
//===============================================================================
void Update_BCD (uint8_t PBCDigit){

    digitalWrite(BCD_0, PBCDigit & 0b00000001);
    digitalWrite(BCD_1, PBCDigit & 0b00000010);
    digitalWrite(BCD_2, PBCDigit & 0b00000100);
    digitalWrite(BCD_3, PBCDigit & 0b00001000);
    digitalWrite(BCD_4, PBCDigit & 0b00010000);
    digitalWrite(BCD_5, PBCDigit & 0b00100000) ;

}


//"rotate add 3" algorithm for convert Byte to Packed BCD Digit.
//==================================================================================
 uint8_t ByteToBCD (uint8_t Byte){
 
     unsigned int BCD_Result = 0;
     unsigned char index = 0;
     
        for (index = 0;index < 7;index++){
                         
            //Test from MSB to LSB of Byte if is 1, this 1 is loaded to LSB of BCD_Result
            if(Byte & (0x80 >> index)){
                BCD_Result |=  0x0001; //set LSB of BCD_Result as 1
            }
            else{
                BCD_Result &= 0xfffe; //set LSB of BCD_Result as 0
            }

            //Start Check Nibbles and add 3 if nibble is greater then 5              
            //Check first nibble
            if ((BCD_Result & 0x000f) > 0x0004){
                BCD_Result += 0x0003;//add 3 to first nibble
            }

            //Check second nibble
            if ((BCD_Result & 0x00f0) > 0x0040){
                BCD_Result += 0x0030;//add 3 to second nibble
            }

            //Check third nibble
            if ((BCD_Result & 0x0f00) > 0x0400){
                BCD_Result += 0x0300;//add 3 to third nibble
            }
            //Rotate to left one bit BCD_Result
            BCD_Result <<= 1;        
        }
            
        //Check the last bit of Byte and put it to the LSB of BCD_Result
        if(Byte & (0x80 >> 7)){
            BCD_Result |=  0x0001; //set LSB of BCD_Result as 1
        }
     return BCD_Result;
 }


 void OLTCmonitor(){

    if (!digitalRead(Cmd_up)) UpTrigger =1;

    
    if (!digitalRead(Cmd_down)) DownTrigger =1;

    if (UpTrigger == 1){

        digitalWrite(MotionPin, HIGH);
        Counter++;
        
        if (Counter == MotionTime){
          
          Step++;
          digitalWrite(MotionPin, LOW);
          Counter = 0;
          UpTrigger = 0;
        }

    }

    if (DownTrigger == 1){

        digitalWrite(MotionPin, HIGH);
        Counter++;

        if (Counter == MotionTime){
          
          Step--;
          digitalWrite(MotionPin, LOW);
          Counter = 0;
          DownTrigger = 0;
        }


    }

    Update_BCD(ByteToBCD(Step));
 }





