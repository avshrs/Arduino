 #include <stdint.h>
#include <Wire.h>


const uint8_t MCPMa = 0x00; //addres in Union Memory table for a side 
const uint8_t MCPMb = 0x01; //addres in Union Memory table for b side 
const uint8_t MCPFa = 0x02; //addres in Union Force  table for a side 
const uint8_t MCPFb = 0x03; //addres in Union Force  table for b side 
const uint8_t MCPSa = 0x04; //addres in Union State  table for a side 
const uint8_t MCPSb = 0x05; //addres in Union State  table for b side 
const uint8_t MCPa  = 0x00; // Side A
const uint8_t MCPb  = 0x01; // Side B
const uint8_t HUMAN = 1; 
const uint8_t BINARY = 0;
const uint8_t MCP1_ADDR  = 0x20; //a2 gnd | a1 gnd | a0 gnd 
const uint8_t MCP2_ADDR  = 0x21; //a2 gnd | a1 gnd | a0 +5v
const uint8_t MCP3_ADDR  = 0x22; //a2 gnd | a1 +5  | a0 gnd 
const uint8_t MCP4_ADDR  = 0x23; //a2 gnd | a1 +5  | a0 +5v
const uint8_t MCP5_ADDR  = 0x24; //a2 +5  | a1 gnd | a0 gnd 
const uint8_t MCP6_ADDR  = 0x25; //a2 +5  | a1 gnd | a0 +5v
const uint8_t MCP7_ADDR  = 0x26; //a2 +5  | a1 +5  | a0 gnd
const uint8_t MCP8_ADDR  = 0x27; //a2 +5  | a1 +5  | a0 +5v
const uint8_t MCP_A_ADDR = 0x12; // Register Address of Port A
const uint8_t MCP_B_ADDR = 0x13; // Register Address of Port B
const uint8_t MCP_A_INIT   = 0x00; // MCP init
const uint8_t MCP_B_INIT   = 0x01; // MCP init
const uint8_t MCP_TO_OUT = 0x00; // set all of port A to outputs
#define MCP_TO_IN  = 0x01; // set all of port A to outputs
#define MCP23017_ADDRESS 0x20

// registers
#define MCP23017_IODIRA 0x00
#define MCP23017_IPOLA 0x02
#define MCP23017_GPINTENA 0x04
#define MCP23017_DEFVALA 0x06
#define MCP23017_INTCONA 0x08
#define MCP23017_IOCONA 0x0A
#define MCP23017_GPPUA 0x0C
#define MCP23017_INTFA 0x0E
#define MCP23017_INTCAPA 0x10
#define MCP23017_GPIOA 0x12
#define MCP23017_OLATA 0x14


#define MCP23017_IODIRB 0x01
#define MCP23017_IPOLB 0x03
#define MCP23017_GPINTENB 0x05
#define MCP23017_DEFVALB 0x07
#define MCP23017_INTCONB 0x09
#define MCP23017_IOCONB 0x0B
#define MCP23017_GPPUB 0x0D
#define MCP23017_INTFB 0x0F
#define MCP23017_INTCAPB 0x11
#define MCP23017_GPIOB 0x13
#define MCP23017_OLATB 0x15

void setup() {
     // put your setup code here, to run once:
    initMcp(MCP1_ADDR, MCP_A_INIT ,MCP_TO_IN)
    Serial.begin(1000000);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void initMcp(const uint8_t MCP_ADDR,const uint8_t MCP_A, const uint8_t  MCP_TO){
    Wire.begin();
    Wire.beginTransmission(MCP_ADDR);
    Wire.write(MCP_A);
    Wire.write(MCP_TO);
    Wire.endTransmission();
    

    Wire.beginTransmission(MCP_ADDR);   
    Wire.write(MCP_B);
    Wire.write(MCP_TO);
    Wire.endTransmission();
}

void writeMcp(){
  
}

void readMcp(){
  
}

