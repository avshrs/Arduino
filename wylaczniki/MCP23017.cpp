#include "MCP23017.h"
#include <Wire.h>
//#include "Arduino.h"


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

void MCP::writeAll(uint8_t a, uint8_t b, uint8_t c){
    a=0;
}        // args: bitmask, side , foce(bool)

void MCP::readAll(uint8_t a){
    a=0;
}                        // args: side
void MCP::writeOne(uint8_t a, uint8_t b, uint8_t c){
    a=0;
}        // args: pin, side , foce(bool)
void MCP::readOne(uint8_t a, uint8_t b){
    a=0;
}               // args: side, pin
