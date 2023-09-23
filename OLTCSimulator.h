/*
  OLTC.cpp - Library for Implement On Load Tap Chager
  Created by S. Banchev, September, 2023.
  Released into the public domain.
*/


#ifndef _OLTCSIMULATOR_H_
#define _OLTCSIMULATOR_H_



#include "Arduino.h"


class OLTC  {
 
  public:

  // Constructor
  OLTC (uint8_t outbcd_0, uint8_t outbcd_1, uint8_t outbcd_2, uint8_t outbcd_3, uint8_t outbcd_4, uint8_t outbcd_5, uint8_t outFirstTap, uint8_t outLastTap, uint8_t outInMotion,
        uint8_t inCmd_Up, uint8_t inCmd_Down, uint8_t inCmd_EStop);

  // Method thath cycling in loop and controling CB
  void Monitoring();
  void getParameters(uint8_t Taps,uint8_t Reverser, uint16_t MotionTime); 

  private:

    //Variable connected to BCD pins
    uint8_t _bcd0, _bcd1, _bcd2, _bcd3, _bcd4, _bcd5;
    //Variable connected to  tap changer status pins
    uint8_t _FirstTapEnd, _LastTapEnd, _InMotion;
    //Variable connected to tap changer commands pins
    uint8_t _Cmd_Up, _Cmd_Down, _Cmd_EStop;

    //Convert binary digit 
    uint8_t _uintToBCD (uint8_t Byte);
    //Method for update the BCD Ports
    void _BCDPinsUpdate(uint8_t PackBCD);

    uint8_t _CurrentTab; //Hold the current tap of tap chager
    uint8_t _LastTab; //Hold the max tab os tab chager
    uint8_t _Reverser; //Hold the option that tab changer have reverser if vale 0 
    uint16_t _MotionTime;

    uint8_t _UpTrigger; //Control byte for presed up command
    uint8_t _DownTrigger; //Control byte for presed down command

    uint16_t _Counter;
   
    
};

//????
#endif