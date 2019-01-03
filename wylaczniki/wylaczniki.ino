//#include <stdint.h>
#include <Wire.h>
#INCLUDE "MCP23017.h"

#define    GPIOA    (0x12)  
#define    GPIOB    (0x13) 

#define    MemA     (0x00)  
#define    MemB     (0x01) 


int delay_v = 10;  

uint16_t MCPC[4];
MCPC[0] = new MCP(0x20, 0xff, 0xff, 0x00, 0x00);
MCPC[1] = new MCP(0x21, 0xff, 0xff, 0x00, 0x00)



void setup(){
  Serial.begin(1000000);



}

void loop(){
  MCPC[0]->readAll(GPIOA);
  MCPC[0]->writeAll(MCPC[MemA]->McpMemory[MemA],GPIOB,0x00);

  MCPC[1]->readAll(GPIOA);
  MCPC[1]->writeAll(MCPC[MemA]->McpMemory[MemA],GPIOB,0x00);


    delay(delay_v);    

}    
