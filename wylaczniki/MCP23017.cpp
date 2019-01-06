#include "MCP23017.h"
#include "vars.h"
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
    return ~r_value;
}

void MCP::readAll(uint8_t side){
    uint8_t s = GPIOA;
    if (side > 0x00) s = GPIOB;
    uint8_t value = readRaw(s);

    McpForce[side] = (~value & ~McpState[side] & McpForce[side]) | (value & McpState[side] & McpForce[side]);
    McpState[side] = value;
 //   McpMemory[side] = ((~McpForce[side] & (value)) | (McpMemory[side] & McpForce[side]));
    //  test multivibrator mask 
    McpMemory[side] = ( value & ~McpForce[side] & ~McpBiStable[side]) |
                      ( ~McpState[side] & value &  McpBiStable[side])  | 
                      ( McpState[side] & ~value &  McpBiStable[side])  |
                      ( McpState[side] & McpForce[side] & ~McpBiStable[side]);
}    



void MCP::writeRaw(uint8_t side, uint8_t memory){
  Wire.begin();
  Wire.beginTransmission (mcpAddress);
  Wire.write (side);  
  Wire.write (memory);   
  Wire.endTransmission ();
}

void MCP::writeOne(uint8_t pin, uint8_t value, uint8_t side, uint8_t force){
    uint8_t mask = (1 << pin);
    uint8_t value_= 0x00;
    uint8_t _write = GPIOA;
    uint8_t _read = 0x01;
    if (side > 0x00) {
        _read = 0;
        _write = GPIOB;
    }
    if (value > 0) value_ = (1 << pin); 
    if (value_ > 0 &&  (McpMemory[_read] & mask) == 0){
        McpMemory[_read] |= mask;
        if(force == FORCE)
            if ((McpForce[_read] & mask) == 0)     
                McpForce[_read] |= mask;
            else if ((McpForce[_read] & mask) > 0)     
                McpForce[_read] &= ~mask;
    }
    else if ((value_ & mask) == 0 &&  (McpMemory[_read] & mask) > 0){
        McpMemory[_read] &= ~mask;
        if(force == FORCE)
            if ((McpForce[_read] & mask) == 0)     
                McpForce[_read] |= mask;
            else if ((McpForce[_read] & mask) > 0)     
                McpForce[_read] &= ~mask;
    }
    writeRaw(_write,McpMemory[_read]);
}

void MCP::writeAll(uint8_t values, uint8_t side, uint8_t force){
    uint8_t _write = GPIOA;
    uint8_t _read = 0x01;
    if (side > 0x00) {
        _read = 0;
        _write = GPIOB;
    }
    if (force > 0x00){
        McpForce[_read] =   (~McpMemory[_read] & ~values & McpForce[_read]) | 
                            (~McpMemory[_read] &  values & ~McpForce[_read]) | 
                            ( McpMemory[_read] & ~values & ~McpForce[_read]) | 
                            ( McpMemory[_read] &  values &  McpForce[_read]);
        McpMemory[_read] = values; 
    }
    else {
        McpMemory[_read] = values;      
    }
    writeRaw(_write,McpMemory[_read]); 
}

