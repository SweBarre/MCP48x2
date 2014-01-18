/*
    MCP48x2.h - Library for Microship DAC MCP4803/4812/4822.
    Created by Jonas Forsberg, 18/1, 2014.
    Licensed under GPL3, no warrenty what so ever and the code is "as-is".
    version 0.1

    this library is depentant on SPI

*/

#ifndef MCP48x2_h
#define MCP48x2_h

#include <Arduino.h>
#include <SPI.h>

/* 
 * configuration bits for the MCP48x2
 * the configuration bits are defined as the four most significant bits in a byte
 * eg: byte channel = CHANNEL_A | GAIN_2 | MODE_ACTIVE
 * this will define 'channel' to be channel A with a gain of 2 and active
 */
#define CHANNEL_A 0x00
#define CHANNEL_B 0x80
#define GAIN_1 0x20
#define GAIN_2 0x00
#define MODE_SHUTDOWN 0x00
#define MODE_ACTIVE 0x10

/* Configuration bit numbering
 * if you define a channel : channel = CHANNEL_A | GAIN_2 | MODE_ACTIVE
 * and you just want to change the gain bit for the channel do
 * setBit(channel, GAIN_BIT)
 */
#define CHANNEL_BIT 7
#define GAIN_BIT 5
#define SHUTDOWN_BIT 4

//the model byte is used to bit shift the LSB so it fits the registers
//on the specific chip used
#define MCP4802 4
#define MCP4812 2
#define MCP4822 0



class MCP48x2 {
  public:
    /*
     * mcpModel defines what kind of DAC that is used, eg: MCP4802, MCP4812 or MCP4822
     * csPin is chip select pin for SPI communication.
     * ldacPin is latch pin for the DAC.
     * If tie the latch pin on the DAC to GND the DAC will automaticly latch the register on the rising
     * edge of the clock and you do not have to create the MCP48x2 object with a specified ldacPin.
     */
    MCP48x2(byte mcpModel, unsigned int csPin);
    MCP48x2(byte mcpModel, unsigned int csPin, unsigned int ldacPin);
    // config = will use the four high bits of the config byte as config bits for the word sent to MCP 
    void send(byte config, word data);
    // will bring the ldacPin low and the high to latch the registers. If no ldacPin is specified
    // when you created the object then latch() does nothing.
    void latch();
  private:
    // initializing the object and sets the default values for the object
    // it sets the csPin specified as OUTPUT and sets it HIGH
    void _init(byte mcpModel, unsigned int csPin, unsigned int ldacPin);
    //holds the chip select pin for this instance
    unsigned int _csPin;
    //holds the latch pin for this instance. If not used it will be 0
    unsigned int _ldacPin;
    //holds the MCP48x2 model
    byte _mcpModel;
    //is latch used
    bool _useLDAC;
};
#endif
