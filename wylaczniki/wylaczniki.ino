#include <stdint.h>
#include <Wire.h>
#include "vars.h"
#include "print_s.h"
#include "helpers.h"

void setMcpToOn(const uint8_t &I2C_ADDR,uint8_t PIN, uint8_t &MEMORY, uint8_t &FORCED,const uint8_t SIDE, bool FORCE){
  uint8_t mask = (1 << PIN);
  if((MEMORY & mask) != mask){
    MEMORY |= mask;
    if(FORCE)
      FORCED ^= mask;
    writeMcp(I2C_ADDR,MEMORY,SIDE);
  }
}


void setMcpToOff(const uint8_t &I2C_ADDR, uint8_t PIN, uint8_t &MEMORY, uint8_t &FORCED,const uint8_t SIDE, bool FORCE){
  uint8_t mask = (1 << PIN);
  if ((MEMORY &   mask) == mask){
    MEMORY &= ~mask;
    if(FORCE)
      FORCED ^= mask;
    writeMcp(I2C_ADDR,MEMORY,SIDE);   
  }
}

void readAllMcp(const uint8_t &I2C_ADDR, uint8_t &MEMORY, uint8_t &FORCED,const uint8_t SIDE){
  uint8_t value = readMcp(I2C_ADDR,MEMORY, SIDE);
  MEMORY = MEMORY^((~FORCED)&(value^MEMORY));
}


void setup(){
  Serial.begin(250000);
  initMcp(MCP1_ADDR, MCPa, MCP_TO_IN);
  initMcp(MCP2_ADDR, MCPb, MCP_TO_OUT);
}

void loop(){
  serialCom();

  readAllMcp(MCP1_ADDR, MCP1s.MCPAM, MCP1s.MCPAF, MCPa);
  readAllMcp(MCP2_ADDR, MCP2s.MCPAM, MCP2s.MCPAF, MCPa);
  writeMcp(MCP2_ADDR, MCP2s.MCPAM, MCPb);
  writeMcp(MCP1_ADDR, MCP1s.MCPAM, MCPb);

  if (delay_mili_micro > 0)
    delay(delay_v);    
  else
    delayMicroseconds(delay_v);      
}    
