#include "MCP23017.h"
#include <Wire.h>
#include <Arduino.h>

MCP::MCP(uint8_t MCPADDRSS, uint8_t GIPOA_TYPE, uint8_t GIPOA_PULL, uint8_t GIPOB_TYPE, uint8_t GIPOB_PULL){
    mcpAddress = MCPADDRSS;

    Wire.begin();
    Wire.beginTransmission(MCPADDRSS);  // expander has I2C address 0x20
    Wire.write(IODIRA);   // register 0 is the I/O direction register for Port A
    Wire.write(GIPOA_TYPE);   //  0x00 for all pins to output mode, 0xFF for all pins to input mode
    Wire.endTransmission();

    if (GIPOA_PULL == 0xFF && GIPOA_TYPE == 0xFF){
        Wire.beginTransmission(MCPADDRSS);  // expander has I2C address 0x20
        Wire.write(GPPUA);   // register 0 is the I/O direction register for Port A
        Wire.write(0xFF);   //  0x00 for all pins to output mode, 0xFF for all pins to input mode
        Wire.endTransmission();
    }

    Wire.beginTransmission (MCPADDRSS);  // expander has I2C address 0x20
    Wire.write(IODIRB);   // register 0 is the I/O direction register for Port A
    Wire.write(GIPOB_TYPE);   //  0x00 for all pins to output mode, 0xFF for all pins to input mode
    Wire.endTransmission();

    if (GIPOB_PULL == 0xFF && GIPOB_TYPE == 0xFF){
        Wire.beginTransmission (MCPADDRSS);  // expander has I2C address 0x20
        Wire.write(GPPUB);   // register 0 is the I/O direction register for Port A
        Wire.write(0xFF);   //  0x00 for all pins to output mode, 0xFF for all pins to input mode
        Wire.endTransmission();
    }
}

uint8_t MCP::readRaw(uint8_t side){
    uint8_t r_value = 0; 
    Wire.beginTransmission(mcpAddress); 
    Wire.write(side);
    Wire.endTransmission();
    Wire.requestFrom((int)mcpAddress, 1);
    while(Wire.available())    
        r_value = Wire.read();
    return r_value;
}

void MCP::readAll(uint8_t Side){
    uint8_t value = readRaw(Side);
    uint8_t s = 0;
    if (Side == 0x13) s = 1;
    for(uint8_t i = 0; i< sizeof(McpState[s])*8; ++i){
        uint8_t mask = (1 << i);
        if ((value & mask) > 0) 
            if (((McpState[s] & mask) == 0) && ((McpMemory[s] & mask) > 0))
                McpForce[s] &= ~mask;      
        if ((value & mask) == 0) 
            if (((McpState[s] & mask) > 0) && ((McpMemory[s] & mask) == 0))
            McpForce[s] &= ~mask;
    }
    McpState[s] = value;
    McpMemory[s] = McpMemory[s]^((~McpForce[s])&(value^McpMemory[s]));
}

void MCP::writeRaw(uint8_t side, uint8_t memory){
  Wire.begin();
  Wire.beginTransmission (mcpAddress);  // expander has I2C address 0x20
  Wire.write (side);   // register 0 is the I/O direction register for Port A
  Wire.write (memory);   //  0x00 for all pins to output mode, 0xFF for all pins to input mode
  Wire.endTransmission ();
}

void MCP::writeOne(uint8_t pin, uint8_t value, uint8_t side, uint8_t force){
    uint8_t mask = (1 << pin);
    uint8_t s = 0;
    uint8_t value_= 0x00;
    //side selector
    if (side == 0x13) s = 1;

    if (value > 0) value_ = (1 << pin);
    
    if((McpMemory[s] & mask) > (value_ & mask))
        McpMemory[s] &= ~mask;
    else 
        McpMemory[s] |= mask;
    
    if(force == 0xff)
        if ((McpForce[s] & mask) > 0)     
            McpForce[s] &= ~mask;
        else 
            McpForce[s] |= mask;
    writeRaw(side,McpMemory[s]);
    }


void MCP::writeAll(uint8_t values, uint8_t side, uint8_t force){
    uint8_t s = 0;
    if (side == 0x13) s = 1;
    if (force == 0xff){
        McpForce[s] = (~McpMemory[s] &values) | (McpMemory[s]& ~values);
        McpMemory[s] = values; 
        McpState[s] = values; 
    }
    else {
        McpMemory[s] = values; 
        McpState[s] = values; 
    }
    writeRaw(side,McpMemory[s]);
    
}
