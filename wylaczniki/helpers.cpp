#include <Wire.h>
#include "helpers.h"

void ArduMCP::initMcp(const uint8_t &MCP_ADDR, const  uint8_t &MCP_SIDE, const uint8_t &MCP_DIRECTION){   
  Wire.begin();
  Wire.beginTransmission (MCP_ADDR);  // expander has I2C address 0x20
  Wire.write (MCP_SIDE);   // register 0 is the I/O direction register for Port A
  Wire.write (MCP_DIRECTION);   //  0x00 for all pins to output mode, 0xFF for all pins to input mode
  Wire.endTransmission ();
}


void ArduMCP::writeMcp(const uint8_t &MCP_ADDR,uint8_t &MCP_MEMORY, const  uint8_t MCP_RW_SIDE){   
  Wire.begin();
  Wire.beginTransmission (MCP_ADDR);  // expander has I2C address 0x20
  Wire.write (MCP_RW_SIDE);   // register 0 is the I/O direction register for Port A
  Wire.write (MCP_MEMORY);   //  0x00 for all pins to output mode, 0xFF for all pins to input mode
  Wire.endTransmission ();
}

uint8_t ArduMCP::readMcp(const uint8_t &MCP_ADDR, const  uint8_t MCP_RW_SIDE){  
    uint8_t value = 0; 
    Wire.begin();
    Wire.beginTransmission (MCP_ADDR);  // expander has I2C address 0x20
    Wire.write (MCP_RW_SIDE);   // register 0 is the I/O direction register for Port A
    Wire.endTransmission ();
    Wire.requestFrom((int)MCP_ADDR, 1);
    while(Wire.available())    
        value = Wire.read();
    return value;
}
