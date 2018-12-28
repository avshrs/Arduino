#include "MCP23017.h"
#include <Wire.h>
#import "Arduino.h"


MCP::MCP(uint8_t mcpAddress, uint8_t sideA, uint8_t pullA, uint8_t sideB, uint8_t pullB){                  // args:   i2c address , side a function,side a pullup bool, side b pullup bool,  side b function 
    Wire.begin();
    Wire.beginTransmission (mcpAddress);  // expander has I2C address 0x20
    Wire.write (IODIRA);   // register 0 is the I/O direction register for Port A
    Wire.write (sideA);   //  0x00 for all pins to output mode, 0xFF for all pins to input mode
    Wire.endTransmission ();
    if (pullA == 0xff){
        Wire.beginTransmission (mcpAddress);  // expander has I2C address 0x20
        Wire.write (GPPUA);   // register 0 is the I/O direction register for Port A
        Wire.write (0xff);   //  0x00 for all pins to output mode, 0xFF for all pins to input mode
        Wire.endTransmission ();
    }
    Wire.beginTransmission (mcpAddress);  // expander has I2C address 0x20
    Wire.write (IODIRB);   // register 0 is the I/O direction register for Port A
    Wire.write (sideB);   //  0x00 for all pins to output mode, 0xFF for all pins to input mode
    Wire.endTransmission ();
    if (pullB == 0xff){
        Wire.beginTransmission (mcpAddress);  // expander has I2C address 0x20
        Wire.write (GPPUB);   // register 0 is the I/O direction register for Port A
        Wire.write (0xff);   //  0x00 for all pins to output mode, 0xFF for all pins to input mode
        Wire.endTransmission ();
    }


}

void WriteAll(uint8_t, uint8_t, uint8_t);        // args: bitmask, side , foce(bool)
void ReadAll(uint8_t);                        // args: side
void WriteOne(uint8_t, uint8_t, uint8_t);        // args: pin, side , foce(bool)
void ReadOne(uint8_t, uint8_t);               // args: side, pin

void ArduMCP::initMcp(const uint8_t &MCP_ADDR, const  uint8_t &MCP_SIDE, const uint8_t &MCP_DIRECTION){   
  Wire.begin();
  Wire.beginTransmission (MCP_ADDR);  // expander has I2C address 0x20
  Wire.write (MCP_SIDE);   // register 0 is the I/O direction register for Port A
  Wire.write (MCP_DIRECTION);   //  0x00 for all pins to output mode, 0xFF for all pins to input mode
  Wire.endTransmission ();
}