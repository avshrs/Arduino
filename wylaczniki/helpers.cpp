#include <Wire.h>
#include "helpers.h"
#import "Arduino.h"


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

void ArduMCP::readAllMcp(const uint8_t &I2C_ADDR, uint8_t &MEMORY, uint8_t &FORCED,uint8_t &STATE, const uint8_t SIDE){
  uint8_t value = readMcp(I2C_ADDR, SIDE);
   for ( uint8_t i = 0; i< sizeof(STATE)*8;++i){
      uint8_t mask = (1 << i);
      if ((value & mask) > 0) 
          if (((STATE & mask) == 0) && ((MEMORY & mask) > 0)){
             
              FORCED &= ~mask;
          }
      
      if ((value & mask) == 0) 
          if (((STATE & mask) > 0) && ((MEMORY & mask) == 0)){
              FORCED &= ~mask;
             
          }
      }
  STATE = value;
  MEMORY = MEMORY^((~FORCED)&(value^MEMORY));

}

void ArduMCP::print_binary8(uint8_t &v){  
    for (uint8_t i = 0 ; i < (sizeof(v)*8) ; ++i){
       if ((v & (1 << i )) > 0) 
           Serial.print("1");
       else
           Serial.print("0"); 
    }
    Serial.println(" "); 
}



void ArduMCP::setMcpToOn(const uint8_t &I2C_ADDR,uint8_t PIN, uint8_t &MEMORY, uint8_t &FORCED,const uint8_t SIDE, bool FORCE){
  uint8_t mask = (1 << PIN);
  if((MEMORY & mask) != mask){
    MEMORY |= mask;
  if(FORCE)
      if ((FORCED & mask) > 0)     
          FORCED &= ~mask;
      else 
          FORCED |= mask;
    writeMcp(I2C_ADDR,MEMORY,SIDE);
  }
}


void ArduMCP::setMcpToOff(const uint8_t &I2C_ADDR, uint8_t PIN, uint8_t &MEMORY, uint8_t &FORCED,const uint8_t SIDE, bool FORCE){
  uint8_t mask = (1 << PIN);
  if ((MEMORY &   mask) == mask){
    MEMORY &= ~mask;
   if(FORCE)
        if ((FORCED & mask) > 0)     
            FORCED &= ~mask;
        else      
            FORCED |= mask;
    writeMcp(I2C_ADDR,MEMORY,SIDE);   
  }
}

void ArduMCP::print_binary(uint32_t &v){  
    for (uint8_t i = 0 ; i < (sizeof(v)*8) ; ++i){
       if ( i == 8 )Serial.print(","); 
       if ( i == 16 )Serial.print(","); 
       if ( i == 24 )Serial.print(","); 
       if ((v & (1 << i )) > 0) 
           Serial.print("1");
       else
           Serial.print("0"); 
    }
    Serial.println(" "); 
}

void ArduMCP::print_binary16(uint16_t &v){  
    for (uint8_t i = 0 ; i < (sizeof(v)*8) ; ++i){
       if ((v & (1 << i )) > 0) 
           Serial.print("1");
       else
           Serial.print("0"); 
    }
    Serial.println(" "); 
}
