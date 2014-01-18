/* Example sketch for the MCP48x2 library
uses a MCP4812 but is easy to change for other MCP DAC

DAC connected to SPI pins (11 and 13 on arduino) and uses chip select on pin 10 on the arduino
two switches is connected to a pull up resister to +5V rail, to pin 4 and 5 on the arduino.
when pin 5 is low the output on the DAC increases
when pin 4 is low the output on the DAC decreases
*/
#include <SPI.h>
#include <MCP48x2.h>


//this example uses the 10bit DAC so we define 1023 as max value for the DAC
#define MAX_VALUE 1023

//define the increase pin
#define PIN_INC 5

//define the decreas pin
#define PIN_DEC 4

//Define the chip select pin to be pin 10
#define CSPIN 10

//define config byte for channel A with gain2 and active
byte channelA = CHANNEL_A | GAIN_2 | MODE_ACTIVE;

// holds the value that is sent to the DAC
int outputValue = 0;

//create the DAC object, this example uses the MCP4812 10 bit DAC
MCP48x2 dac(MCP4812, CSPIN);

void setup(){
  //start the SPI
  SPI.begin();
  //we only use channel A in this exampe so lets shutdown channel B
  dac.send(CHANNEL_B | MODE_SHUTDOWN, 0);
  //set the output on channel A to zero volts
  dac.send(channelA, 0);
  
}


void loop(){
  if(digitalRead(PIN_DEC) == LOW){
    outputValue -=1;
    //check so that the minimum value we send to the DAC is zero
    if(outputValue < 0) outputValue = 0;
    dac.send(channelA, outputValue);
      
  }

  if(digitalRead(PIN_INC) == LOW){
    outputValue +=1;
    //check so we do not rollover the max value for the DAC
    if(outputValue > MAX_VALUE) outputValue = MAX_VALUE;
    dac.send(channelA, outputValue);
  }
  // a small delay to "catch" bouncing on the switches.
  delay(10);
}

