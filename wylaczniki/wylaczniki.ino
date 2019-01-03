//#include <stdint.h>
#include <Wire.h>
#include "helpers.h"

#include "MCP23017.h"

#define    GPIOA    (0x12)  
#define    GPIOB    (0x13) 

#define    MemA     (0x00)  
#define    MemB     (0x01) 


int delay_v = 10;  
MCP *mcpc[2];
ArduMCP pr;



void setup(){
  Serial.begin(1000000);
mcpc[0] = new MCP(0x20, 0xff, 0xff, 0x00, 0x00);
mcpc[1] = new MCP(0x21, 0xff, 0xff, 0x00, 0x00);

}

void loop(){
  mcpc[0]->readAll(GPIOA);
  pr.print_binary8(mcpc[MemA]->McpMemory[MemA]);
  mcpc[0]->writeAll(mcpc[MemA]->McpMemory[MemA],GPIOB,0x00);

  mcpc[1]->readAll(GPIOA);
  mcpc[1]->writeAll(mcpc[MemA]->McpMemory[MemA],GPIOB,0x00);


    delay(delay_v);    

}    
