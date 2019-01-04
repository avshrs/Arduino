
#include "helpers.h"
#include "print_s.h"
#include <Arduino.h>
PrintBin pbs;
/*

write one: 
uint8_t 0 - 0xYZ | MCP NUMBER 0-7
uint8_t 1 - 0xYZ | SIDE  0 A | 1 B 
uint8_t 2 - 0x00 | write one 
uint8_t 3 - 0xYZ | Y - PIN 0 - 7 , Z - value 0|1 0x00 | null

write all: 
uint8_t 0 - 0xYZ | MCP NUMBER 0-7
uint8_t 1 - 0xYZ | SIDE  0 A | 1 B 
uint8_t 2 - 0x01 | write all
uint8_t 3 - 0xYZ | 0x00 - 0xff | egz. 0b00010001


read all: 
uint8_t 0 - 0xYZ | MCP NUMBER 0-7
uint8_t 1 - 0xYZ | SIDE  0 A | 1 B 
uint8_t 2 - 0x11 | read all
uint8_t 3 - 0x00 | null

*/

void Communication::checkPayloadData(SERIALMCPFRAME* data, MCP *mcpc[8]){
    if(data->INSTRUCTIONS == 0x00){  
        mcpc[data->MCPNR]->writeOne((uint8_t)((data->VALUE)>>4),(uint8_t)(data->VALUE)&0x0F, (uint8_t)data->MCPSIDE, 0xFF);
    
    }
    else if(data->INSTRUCTIONS == 0x01){  
        mcpc[data->MCPNR]->writeAll(data->VALUE, data->MCPSIDE, 0xFF);
    
    }
    else if(data->INSTRUCTIONS == 0x11){  
        mcpc[data->MCPNR]->readAll(data->MCPSIDE);
        pbs.print_binary3x8(*mcpc[data->MCPNR]->McpMemory,*mcpc[data->MCPNR]->McpForce,*mcpc[data->MCPNR]->McpState);
                
        /*
        TO DO
        communication back to leasener on ip;
        */
            
    }
          
        data->MCPNR= 0;
        data->MCPSIDE = 0;
        data->INSTRUCTIONS= 0;
        data->VALUE= 0;
          
    }
        
    








