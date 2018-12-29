#include "MCP23017.h"
#include <Wire.h>
//#include "Arduino.h"


MCP::MCP(uint8_t cMcpAddress, uint8_t sideA, uint8_t pullA, uint8_t sideB, uint8_t pullB){                  // args:   i2c address , side a function,side a pullup bool, side b pullup bool,  side b function 
    mcpAddress = cMcpAddress;
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


uint8_t MCP::readRaw(uint8_t side){
    uint8_t r_value = 0; 
    Wire.begin();
    Wire.beginTransmission (mcpAddress);  // expander has I2C address 0x20
    Wire.write (side);   // register 0 is the I/O direction register for Port A
    Wire.endTransmission ();
    Wire.requestFrom((int)mcpAddress, 1);
    while(Wire.available())    
        r_value = Wire.read();
    return r_value;
}

void MCP::readAll(uint8_t side){
    uint8_t value = readRaw(side);
    if(side == 0x12){
        for(uint8_t i = 0; i< sizeof(aMcpState)*8;++i){
            uint8_t mask = (1 << i);
            if ((value & mask) > 0) 
                if (((aMcpState & mask) == 0) && ((aMcpMemory & mask) > 0))
                    aMcpForce &= ~mask;      
            if ((value & mask) == 0) 
                if (((aMcpState & mask) > 0) && ((aMcpMemory & mask) == 0))
                aMcpForce &= ~mask;
        }
        aMcpState = value;
        aMcpMemory = aMcpMemory^((~aMcpForce)&(value^aMcpMemory));
    }
    else if(side == 0x13){
        for(uint8_t i = 0; i< sizeof(bMcpState)*8;++i){
            uint8_t mask = (1 << i);
            if ((value & mask) > 0) 
                if (((bMcpState & mask) == 0) && ((bMcpMemory & mask) > 0))
                    bMcpForce &= ~mask;      
            if ((value & mask) == 0) 
                if (((bMcpState & mask) > 0) && ((bMcpMemory & mask) == 0))
                    bMcpForce &= ~mask;
        }
        bMcpState = value;
        bMcpMemory = bMcpMemory^((~bMcpForce)&(value^bMcpMemory));
    }
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
    if( side == 0x12 ){
        if((aMcpMemory & mask) > (value & mask))
            aMcpMemory &= ~mask;
        else 
            aMcpMemory |= mask;
        if(force == 0xff)
            if ((aMcpForce & mask) > 0)     
                aMcpForce &= ~mask;
            else 
                aMcpForce |= mask;
        writeRaw(side,aMcpMemory);
    }
    else if( side == 0x13 ){
        if((bMcpMemory & mask) > (value & mask))
            bMcpMemory &= ~mask;
        else 
            bMcpMemory |= mask;
        if(force == 0xff)
            if ((bMcpForce & mask) > 0)     
                bMcpForce &= ~mask;
            else 
                bMcpForce |= mask;
        writeRaw(side,bMcpMemory);
  }

}       

void MCP::writeAll(uint8_t values, uint8_t side, uint8_t force){
    for( uint8_t i = 0; i< sizeof(uint8_t)*8;++i)
        writeOne(i,values, side, force);
}
