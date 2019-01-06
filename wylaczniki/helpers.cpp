
#include "helpers.h"
#include "print_s.h"
#include "vars.h"
#include <Arduino.h>
PrintBin pbs;
/*

write one: 
uint8_t  - 0xYZ | MCP NUMBER 0-7
uint8_t  - 0xYZ | SIDE  0 A | 1 B 
uint8_t  - 0x00 | write one 
uint8_t  - 0xYZ | Y - PIN 0 - 7 , Z - value 0|1 0x00 | null

write all: 
uint8_t  - 0xYZ | MCP NUMBER 0-7
uint8_t  - 0xYZ | SIDE  0 A | 1 B 
uint8_t  - 0x01 | write all
uint8_t  - 0xYZ | 0x00 - 0xff | egz. 0b00010001


read all: 
uint8_t  - 0x11 | read all
uint8_t  - 0xYZ | MCP NUMBER 0-7
uint8_t  - 0xYZ | SIDE  0 A | 1 B 
uint8_t  - 0x00 | null

change delay:
uint8_t  - 0xF1 | change delay
uint8_t  - 0xYZ | MCP NUMBER 0-7
uint8_t  - 0xYZ | SIDE  0 A | 1 B 
uint8_t  - 0x00 | null


----------------------------------------------------------------------
uint8_t INSTRUCTIONS = 0;          
uint8_t MCPNR = 0; 
uint8_t MCPSIDE = 0; 
uint8_t VALUE = 0;
-----------------------------------------------
       
*/

void Communication::checkPayloadData(SERIALMCPFRAME* data, MCP *mcpc[8], int &delay){
    // Print walue on one sensor 
    if(data->INSTRUCTIONS == 0x00){  
        mcpc[data->MCPNR]->writeOne((uint8_t)((data->VALUE)>>4),(uint8_t)(data->VALUE)&0x0F, (uint8_t)data->MCPSIDE, FORCE);
    }
    // Print to all at once 
    else if(data->INSTRUCTIONS == 0x01){  
        mcpc[data->MCPNR]->writeAll(data->VALUE, data->MCPSIDE, FORCE);
    }
    // Print values from side 
    else if(data->INSTRUCTIONS == 0x11){  
        mcpc[data->MCPNR]->readAll(data->MCPSIDE);
        //print on serial 
        pbs.print_binary3x8(mcpc[data->MCPNR]->McpMemory[data->MCPSIDE],mcpc[data->MCPNR]->McpForce[data->MCPSIDE],mcpc[data->MCPNR]->McpState[data->MCPSIDE]);
                
        /*
        TO DO
        communication back to leasener on ip;
        */
            
    }    
    
    else if(data->INSTRUCTIONS == 0xF1){  
        delay = (int)data->VALUE; 
    }
          
        data->MCPNR= 0;
        data->MCPSIDE = 0;
        data->INSTRUCTIONS= 0;
        data->VALUE= 0;
          
    }
        
    








