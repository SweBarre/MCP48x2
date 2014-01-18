/*
     MCP48x2.cpp - Library for Microship DAC MCP4802/4812/4822.
     Created by Jonas Forsberg, 18/1, 2014.
     Licensed under GPL3, no warrenty what so ever and the code is "as-is".
     version 0.1

*/
#include <Arduino.h>
#include <SPI.h>
#include "MCP48x2.h"


MCP48x2::MCP48x2(byte mcpModel, unsigned int csPin){
  //if latch pin isn't used be sure to tie the latch pin on MCP chip to GND
  //the both channels will be latched on the rising edge of CS (chip select)
  this->_useLDAC = false;
  this->_init(mcpModel, csPin, 0);
}

MCP48x2::MCP48x2(byte mcpModel, unsigned int csPin, unsigned int ldacPin){
  this->_useLDAC = true;
  this->_init(mcpModel, csPin, ldacPin);
}

void MCP48x2::_init(byte mcpModel, unsigned int csPin, unsigned int ldacPin){
  this->_csPin = csPin;
  this->_ldacPin = 0;
  this->_mcpModel = mcpModel;
  
  pinMode(this->_csPin, OUTPUT);
  digitalWrite(this->_csPin, HIGH);
}

void MCP48x2::send(byte config, word data){
  //move the LSB on the data to corispond to the model of chip
  word payload = data;
  if(this->_mcpModel) payload=payload << this->_mcpModel;
  
  //apply the config to the config bits
  payload = (config << 8) | (payload & 0x0FFF);
  //configure the SPI
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  //enable the chip
  digitalWrite(this->_csPin,LOW);
  //send the high byte of the word
  SPI.transfer(highByte(payload));
  //send the low byte of the word
  SPI.transfer(lowByte(payload));
  //sending complete, raise the chip select.
  //if the latch pin of the chip is tied to GND then the registers (both channels) will be latched
  //on the rising edge of the CS. 
  digitalWrite(this->_csPin,HIGH);
}

void MCP48x2::latch(){
  if(this->_useLDAC){
    //Latch pin is defined and the latch pin will be set to LOW to latch both registers
    digitalWrite(this->_ldacPin, LOW);
    //raise the latch pin.
    digitalWrite(this->_ldacPin, HIGH);
  }
}
