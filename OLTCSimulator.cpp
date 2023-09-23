#include "Arduino.h"
#include "OLTCSimulator.h"


//Constructor passing variables to class
OLTC :: OLTC (uint8_t outbcd_0, uint8_t outbcd_1, uint8_t outbcd_2, uint8_t outbcd_3, uint8_t outbcd_4, uint8_t outbcd_5, uint8_t outFirstTap, uint8_t outLastTap, uint8_t outInMotion, uint8_t inCmd_Up, uint8_t inCmd_Down, uint8_t inCmd_EStop){

// Connect digital output for bcd  
_bcd0 = outbcd_0;
_bcd1 = outbcd_1;
_bcd2 = outbcd_2;
_bcd3 = outbcd_3;
_bcd4 = outbcd_4;
_bcd5 = outbcd_5;

//Connect digital output for tap changer status
_FirstTapEnd = outFirstTap;
_LastTapEnd = outLastTap;
_InMotion = outInMotion;

//Connect digital input fot tap chager control
_Cmd_Up = inCmd_Up;
_Cmd_Down = inCmd_Down;
_Cmd_EStop =  inCmd_EStop;


//Varaiables inside the object
_CurrentTab = 1; //Start program from first tab
_UpTrigger = 0;  //Control byte for presed up command
_DownTrigger = 0; //Control byte for pressed down command
_Counter = 0; //Used for Motion time delay

_BCDPinsUpdate(_uintToBCD(_CurrentTab));

}

//Method  pass parameters for OLTC Motor Driver
void OLTC:: getParameters (uint8_t Taps,uint8_t Reverser, uint16_t MotionTime){

_LastTab = Taps; //All taps of tab changer
_Reverser = Reverser; //Do you have reverser if no 0 elese on wich step 
_MotionTime = MotionTime; //How long takes the to switch tab aprox 1000 = 1S

}

//===================================================================================
//Main function That simulate MotorDriver of TapChanger
//===================================================================================
void OLTC::Monitoring(){

  //Condition to start Up Command
  if (!digitalRead(_Cmd_Up) && _DownTrigger == 0 && _CurrentTab < _LastTab) _UpTrigger =1; 

  //Condition to start Down Command
  if (!digitalRead(_Cmd_Down) && _UpTrigger == 0 && _CurrentTab > 1) _DownTrigger =1;

  if (_UpTrigger){ //Recived Up command

    digitalWrite(_InMotion, HIGH);
    _Counter++;   
  }

  if (_DownTrigger){ //Recived Down command

    digitalWrite(_InMotion, HIGH);
    _Counter++;
  }
 
  //Check For End Down condition
  if (_CurrentTab == 1){
    digitalWrite(_FirstTapEnd, HIGH);
  }
  else{
    digitalWrite(_FirstTapEnd, LOW);
  }

  //Check For End UP  condition
  if(_CurrentTab == _LastTab){
    digitalWrite(_LastTapEnd, HIGH);
  }
  else{
    digitalWrite(_LastTapEnd, LOW);
  }

  //If Motion time lapse
  if(_Counter == _MotionTime) {

    if (_DownTrigger) _CurrentTab--;
    if (_UpTrigger) _CurrentTab++;

    //Update every cycle BCD Ports 
    _BCDPinsUpdate(_uintToBCD(_CurrentTab));

    digitalWrite(_InMotion, LOW); //Turn Off InMotion LED RELAY
    _UpTrigger = 0; //Zero flags for Up trigger condition
    _DownTrigger = 0; //Zero flags for Down trigger condition
    _Counter = 0; //Zero counter for motion timer
    delay(500);

  }
}
//==================================================================================


//Update BCD Ports method
//==================================================================================
void OLTC:: _BCDPinsUpdate (uint8_t PackBCD){

    digitalWrite(_bcd0, PackBCD & 0b00000001);
    digitalWrite(_bcd1, PackBCD & 0b00000010);
    digitalWrite(_bcd2, PackBCD & 0b00000100);
    digitalWrite(_bcd3, PackBCD & 0b00001000);
    digitalWrite(_bcd4, PackBCD & 0b00010000);
    digitalWrite(_bcd5, PackBCD & 0b00100000);

}

//"rotate add 3" algorithm for convert Byte to Packed BCD Digit method
//==================================================================================
 uint8_t OLTC :: _uintToBCD (uint8_t Byte){
 
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