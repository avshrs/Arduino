#include <stdint.h>
#include <Wire.h>
#include "vars.h"
#include "helpers.h"
ArduMCP mcpArdu;
#include "print_s.h"


void readAllMcp(const uint8_t &I2C_ADDR, uint8_t &MEMORY, uint8_t &FORCED,uint8_t &STATE, const uint8_t SIDE){
  uint8_t value = mcpArdu.readMcp(I2C_ADDR, SIDE);
   for ( uint8_t i = 0; i< sizeof(STATE)*8;++i){
      uint8_t mask = (1 << i);
      if ((value & mask) > 0) 
          if (((STATE & mask) == 0) && ((MEMORY & mask) > 0)){
              Serial.println(" value > 0 force dis");
              FORCED &= ~mask;
          }
      
      if ((value & mask) == 0) 
          if (((STATE & mask) > 0) && ((MEMORY & mask) == 0)){
              FORCED &= ~mask;
               Serial.println(" value == 0");
          }
      }
  STATE = value;
  MEMORY = MEMORY^((~FORCED)&(value^MEMORY));
    Serial.print((uint16_t)SIDE);
    Serial.print(" val=");
    print_binary8(value);
    Serial.print((uint16_t)SIDE);
    Serial.print(" mem=");
    print_binary8(MEMORY);
    Serial.print((uint16_t)SIDE);
    Serial.print(" for=");
    print_binary8(FORCED);
}


char   RS_CHAR;
String RS_BUFFER    = "";
String RS_VALUE     = "";
char   RS_DELIMITER = ',';
int    RS_POSITION  = 0;
char   RS_FLAG      = '\n';

int    delay_v          = 10;   //refresh timer 10 ms
int    delay_mili_micro = 1;



bool rsReceiver() {
  if(Serial.available()) {
    RS_CHAR = char(Serial.read());
    if(RS_CHAR == RS_FLAG) {
      RS_VALUE = RS_BUFFER;
      RS_BUFFER = "";
      return true;
    }
    else{
      RS_BUFFER += RS_CHAR;
      return false;
    }
  }
  else 
    return false;
}

String getCmd() {
  RS_POSITION = RS_VALUE.indexOf(RS_DELIMITER);
  if(RS_POSITION > -1) 
    return RS_VALUE.substring(0, RS_POSITION); 
  else 
    return  RS_VALUE;
}

bool isCmd(String CMD){
  if (getCmd() == CMD)
    return true;
  else 
    return false;
}

String getValue() {
  RS_POSITION = RS_VALUE.indexOf(RS_DELIMITER);
  if(RS_POSITION > -1) 
    return RS_VALUE.substring(RS_POSITION + 1, RS_VALUE.length());
  else 
    return "else";
}


void print_binary(uint32_t &v){  
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
void print_binary8(uint8_t &v){  
    for (uint8_t i = 0 ; i < (sizeof(v)*8) ; ++i){
       if ((v & (1 << i )) > 0) 
           Serial.print("1");
       else
           Serial.print("0"); 
    }
    Serial.println(" "); 
}
void print_binary16(uint16_t &v){  
    for (uint8_t i = 0 ; i < (sizeof(v)*8) ; ++i){
       if ((v & (1 << i )) > 0) 
           Serial.print("1");
       else
           Serial.print("0"); 
    }
    Serial.println(" "); 
}


void setMcpToOn(const uint8_t &I2C_ADDR,uint8_t PIN, uint8_t &MEMORY, uint8_t &FORCED,const uint8_t SIDE, bool FORCE){
  uint8_t mask = (1 << PIN);
  if((MEMORY & mask) != mask){
    MEMORY |= mask;
  if(FORCE)
      if (FORCED > 0)     
          FORCED &= ~mask;
      else if (FORCED == 0)     
          FORCED |= mask;
    mcpArdu.writeMcp(I2C_ADDR,MEMORY,SIDE);
  }
}


void setMcpToOff(const uint8_t &I2C_ADDR, uint8_t PIN, uint8_t &MEMORY, uint8_t &FORCED,const uint8_t SIDE, bool FORCE){
  uint8_t mask = (1 << PIN);
  if ((MEMORY &   mask) == mask){
    MEMORY &= ~mask;
 if(FORCE)
            if (FORCED > 0)     
                FORCED &= ~mask;
            else if (FORCED == 0)     
                FORCED |= mask;
    mcpArdu.writeMcp(I2C_ADDR,MEMORY,SIDE);   
  }
}

void printAll(const uint8_t &_HUMAN,uint32_t &MEMORY){
  if (_HUMAN == true ){
    for (uint8_t i = 0 ; i< (sizeof(MEMORY)*8)/2 ; ++i){
      Serial.print("Output nr: ");
      Serial.print(i);
      if ((MEMORY & (1 << i )) > 0) 
        Serial.print(" 1");
      else
        Serial.print(" 0"); 
      if ((MEMORY & (1 << i+15 )) > 0) 
        Serial.print(" 1");
      else
        Serial.print(" 0"); 
      Serial.println(" "); 
    }
  }
  else
    print_binary(MEMORY);
}

void serialCom(){
  if(rsReceiver()) {
    if(isCmd("test")) {
      Serial.println("True");
    }
    if(isCmd("delay")) {
      delay_v = getValue().toInt();
      Serial.println(getValue().toInt());
    }
    if(isCmd("delay_mu")) {
      delay_mili_micro = getValue().toInt();
      Serial.println(delay_mili_micro);
    }
    if(isCmd("setMCP1aOn")) {
      Serial.println("on");
     setMcpToOn(MCP1_ADDR, getValue().toInt() , MCP1s.MCPAM,  MCP1s.MCPAF, MCP_A_RW, true);
      
    }
    if(isCmd("setMCP1bOn")) {
      Serial.println("on");
      setMcpToOn(MCP1_ADDR, getValue().toInt() , MCP1s.MCPBM,  MCP1s.MCPBF, MCP_B_RW, true);
    }
    if(isCmd("setMCP1aOff")) {
      setMcpToOff(MCP1_ADDR, getValue().toInt(), MCP1s.MCPAM,  MCP1s.MCPAF, MCP_A_RW, true);
    }
    if(isCmd("setMCP1bOff")) {
      setMcpToOff(MCP1_ADDR, getValue().toInt() ,  MCP1s.MCPBM,  MCP1s.MCPBF, MCP_B_RW, true);
    }
    if(isCmd("force")) {  // force,111 
      String pars = getValue();
      String par = (String)pars[1];
      if (pars[0] == '1')
          if (pars[2] == '1')
              setMcpToOn(MCP1_ADDR, par.toInt() , MCP1s.MCPBM,  MCP1s.MCPBF, MCP_B_RW, true);
          else
              setMcpToOff(MCP1_ADDR, par.toInt() , MCP1s.MCPBM,  MCP1s.MCPBF, MCP_B_RW, true);
      
    }
  

    if(isCmd("help")) {
      Serial.println("delay \t\t- set loop dlelay");
      Serial.println("delay_mu \t\t- set from Miliseconds (1) to Microseconds (0)");
      Serial.println("readMCP1 \t\t- read all values from MCP1 in human");
      Serial.println("BinReadMCP1 \t\t- read all values from MCP1 in binary");
      Serial.println("readMCP2 \t\t- read all values from MCP2 in human");
      Serial.println("BinReadMCP2 \t\t- read all values from MCP2 in binary");
      Serial.println("setMCP\t\t- set output from MCP setMCP e.g. setMCP,1b51 (1) adres of mpc 1, (a) side ,(1) output number , (1) state");
    }
  }
}

void setup(){
  Serial.begin(1000000);
  mcpArdu.initMcp(MCP1_ADDR, MCPa_INIT, MCP_TO_IN);
  mcpArdu.initMcp(MCP1_ADDR, MCPb_INIT, MCP_TO_OUT);
  mcpArdu.initMcp(MCP2_ADDR, MCPa_INIT, MCP_TO_IN);
  mcpArdu.initMcp(MCP2_ADDR, MCPb_INIT, MCP_TO_OUT);
}

void loop(){
  serialCom();

  readAllMcp(MCP1_ADDR, MCP1s.MCPAM, MCP1s.MCPAF, MCP1s.MCPAS, MCP_A_RW);
  mcpArdu.writeMcp(MCP1_ADDR, MCP1s.MCPAM, MCP_B_RW);
  readAllMcp(MCP2_ADDR, MCP2s.MCPAM, MCP2s.MCPAF, MCP2s.MCPAS, MCP_A_RW);
  mcpArdu.writeMcp(MCP2_ADDR, MCP2s.MCPAM, MCP_B_RW);


  if (delay_mili_micro > 0)
    delay(delay_v);    
  else
    delayMicroseconds(delay_v);      
}    
