#include <stdint.h>
#include <Wire.h>

const uint8_t MCPMa = 0;
const uint8_t MCPMb = 1;
const uint8_t MCPFa = 2;
const uint8_t MCPFb = 3;
const uint8_t MCPa  = 0;
const uint8_t MCPb  = 1;
const uint8_t HUMAN = 1;
const uint8_t BINARY = 0;
const uint8_t MCP1_ADDR  = 0x20;
const uint8_t MCP2_ADDR  = 0x21;
const uint8_t MCP_A_ADDR = 0x12; // Register Address of Port A
const uint8_t MCP_B_ADDR = 0x13; // Register Address of Port B
const uint8_t MCP_INIT   = 0x00; // MCP init
const uint8_t MCP_TO_OUT = 0x00; // Register Address of Port A
const uint8_t MCP_TO_IN  = 0x01; // Register Address of Port A
char   RS_CHAR;
String RS_BUFFER    = "";
String RS_VALUE     = "";
char   RS_DELIMITER = ',';
int    RS_POSITION  = 0;
char   RS_FLAG      = '\n';

int    delay_v          = 10;   //refresh timer 10 ms
int    delay_mili_micro = 1;

union uMCP1{
    uint32_t MCP1p;     
    uint8_t  MCP1[4]; 
}; 
uMCP1 MCP1u = {0};

union uMCP2{
    uint32_t MCP2p;     
    uint8_t  MCP2[4]; 
};           
uMCP2 MCP2u = {0};

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
    for (uint8_t i = 0 ; i< sizeof(v)*8 ; ++i){
       if ((v & (1 << i )) > 0) 
           Serial.print("1");
       else
           Serial.print("0"); 
       
    }
    Serial.println(" "); 
}

void printAll(const uint8_t &HUMAN,uint32_t &MEMORY){
    if (HUMAN == true ){
        for (uint8_t i = 0 ; i< sizeof(MEMORY)*8 ; ++i){
            Serial.print("Output nr: ");
            if ((MEMORY & (1 << i )) > 0) 
                Serial.print("1");
            else
                Serial.print("0"); 
        }
    }
    else
        print_binary(MEMORY);
}




void setMcpToOn(const uint8_t &I2C_ADDR,uint8_t PIN, uint8_t &MEMORY, uint8_t &FORCED,const uint8_t SIDE, bool FORCE){
    uint8_t mask = (1 << PIN);
    if((MEMORY & mask) != mask){
        MEMORY |= mask;
        if(FORCE)
            FORCED ^= mask;
        Wire.beginTransmission(I2C_ADDR);
        if (SIDE == MCPa){
            Wire.write(MCP_A_ADDR);
            Wire.write(MEMORY);
        }
        if (SIDE == MCPb){
            Wire.write(MCP_B_ADDR);
            Wire.write(MEMORY);  
        }
        Wire.endTransmission();    
    }
 }

void setMcpToOff(const uint8_t &I2C_ADDR, uint8_t PIN, uint8_t &MEMORY, uint8_t &FORCED,const uint8_t SIDE, bool FORCE){
    uint8_t mask = (1 << PIN);
    if ((MEMORY &   mask) == mask){
            MEMORY &= ~mask;
        if(FORCE)
            FORCED ^= mask;
        Wire.beginTransmission(I2C_ADDR);
        if (SIDE == MCPa){
            Wire.write(MCP_A_ADDR);
            Wire.write(MEMORY);     
        }
        if (SIDE == MCPb){
            Wire.write(MCP_B_ADDR);
            Wire.write(MEMORY);   
        }
        Wire.endTransmission();    
    }
}


void writeAllMcp(const uint8_t &I2C_ADDR,uint8_t &MEMORY,const  uint8_t SIDE){
    Wire.beginTransmission(I2C_ADDR);
    if (SIDE == MCPa){
        Wire.write(MCP_A_ADDR);
        Wire.write(MEMORY);
     }
    if (SIDE == MCPb){
        Wire.write(MCP_B_ADDR);
        Wire.write(MEMORY);
    }
    Wire.endTransmission();
}



void readAllMcpValues(const uint8_t &I2C_ADDR, uint8_t &MEMORY, uint8_t &FORCED,const uint8_t SIDE){
    uint8_t value = 0;
    Wire.beginTransmission(I2C_ADDR);
    if (SIDE == MCPa){
        Wire.write(MCP_A_ADDR);
        Wire.endTransmission();
        Wire.requestFrom((int)I2C_ADDR, 1);
        while(Wire.available()) {   
            value = Wire.read();
        } 
    }
    if (SIDE == MCPb){
        Wire.write(MCP_B_ADDR);
        Wire.endTransmission();
        Wire.requestFrom((int)I2C_ADDR, 1);
        while(Wire.available()) {   
            value = Wire.read();
        }       
    }
    MEMORY = ~(MEMORY^((~FORCED)&(value^MEMORY)));    
    
}



void serialCom(){
    if(rsReceiver()) {
        if(isCmd("test")) {
            Serial.println("True");
        }
        if(isCmd("delay")) {
            delay_v = 1;
            Serial.println(getValue().toInt());
        }
        if(isCmd("delay_mu")) {
            delay_mili_micro = getValue().toInt();
            Serial.println(delay_mili_micro);
        }
        if(isCmd("readMCP1")) {
            printAll(HUMAN,MCP1u.MCP1p);
        }
        if(isCmd("BinReadMCP1")) {
             printAll(BINARY,MCP1u.MCP1p);
        }
        if(isCmd("readMCP2")) {
            printAll(HUMAN,MCP2u.MCP2p);
        }
        if(isCmd("BinReadMCP1")) {
             printAll(BINARY,MCP2u.MCP2p);
        }

        if(isCmd("setMCP1aON")) 
            setMcpToOn(MCP1_ADDR,(uint8_t)getValue().toInt(),  MCP1u.MCP1[MCPMa],MCP1u.MCP1[MCPFa],MCPa, true);

        if(isCmd("setMCP1aOFF")) 
            setMcpToOff(MCP1_ADDR,(uint8_t)getValue().toInt(),  MCP1u.MCP1[MCPMa],MCP1u.MCP1[MCPFa],MCPa, true);
 
        if(isCmd("setMCP1bON")) 
            setMcpToOn(MCP1_ADDR,(uint8_t)getValue().toInt(),  MCP1u.MCP1[MCPMa],MCP1u.MCP1[MCPFa],MCPa, true);
 
        if(isCmd("setMCP1bOFF")) 
            setMcpToOff(MCP1_ADDR,(uint8_t)getValue().toInt(),  MCP1u.MCP1[MCPMb],MCP1u.MCP1[MCPFb],MCPb, true);
 
        if(isCmd("setMCP2aON")) 
            setMcpToOn(MCP2_ADDR,(uint8_t)getValue().toInt(),  MCP2u.MCP2[MCPMa],MCP2u.MCP2[MCPFa],MCPa, true);
 
        if(isCmd("setMCP2aOFF")) 
            setMcpToOff(MCP2_ADDR,(uint8_t)getValue().toInt(),  MCP2u.MCP2[MCPMa],MCP2u.MCP2[MCPFa],MCPa, true);
 
        if(isCmd("setMCP2bON")) 
            setMcpToOn(MCP2_ADDR,(uint8_t)getValue().toInt(),  MCP2u.MCP2[MCPMa],MCP2u.MCP2[MCPFa],MCPa, true);
 
        if(isCmd("setMCP2bOFF")) 
            setMcpToOff(MCP2_ADDR,(uint8_t)getValue().toInt(),  MCP2u.MCP2[MCPMb],MCP2u.MCP2[MCPFb],MCPb, true);
       
        if(isCmd("help")) {
            Serial.println("delay \t\t- set loop dlelay");
            Serial.println("delay_mu \t\t- set from Miliseconds (1) to Microseconds (0)");
            Serial.println("readMCP1 \t\t- read all values from MCP1 in human ");
            Serial.println("BinReadMCP1 \t\t- read all values from MCP1 in binary ");
            Serial.println("readMCP2 \t\t- read all values from MCP2 in human ");
            Serial.println("BinReadMCP2 \t\t- read all values from MCP2 in binary ");
            Serial.println("setMCP1aON \t\t- set output from MCP1 side a to on");
            Serial.println("setMCP1aOFF \t\t- set output from MCP1 side a to off");
            Serial.println("setMCP1bON \t\t- set output from MCP1 side b to on");
            Serial.println("setMCP1bOFF \t\t- set output from MCP1 side b to off");
            Serial.println("setMCP2aON \t\t- set output from MCP2 side a to on");
            Serial.println("setMCP2aOFF \t\t- set output from MCP2 side a to off");
            Serial.println("setMCP2bON \t\t- set output from MCP2 side b to on");
            Serial.println("setMCP2bOFF \t\t- set output from MCP2 side b to off");
        }
       
    }
}

void initMcp(const uint8_t &MCP_ADDR,const  uint8_t &MCP_INIT,const uint8_t &MCP_B ,const uint8_t &MCP_A ){   
    Wire.begin();
    Wire.beginTransmission(MCP_ADDR);
    Wire.write(MCP_INIT);
    Wire.write(MCP_A);
    Wire.write(MCP_B);
    Wire.endTransmission();
}

void setup(){
    Serial.begin(2000000);
    //  chipset addres, init seq., A side   , B side 
    initMcp(MCP1_ADDR, MCP_INIT, MCP_TO_IN, MCP_TO_OUT);
    initMcp(MCP2_ADDR, MCP_INIT, MCP_TO_IN, MCP_TO_OUT);
}

void loop(){
    serialCom();
    readAllMcpValues(MCP1_ADDR, MCP1u.MCP1[MCPMa], MCP1u.MCP1[MCPFa], MCPa);
    readAllMcpValues(MCP1_ADDR, MCP2u.MCP2[MCPMb], MCP2u.MCP2[MCPFb], MCPb);
    writeAllMcp(MCP1_ADDR, MCP1u.MCP1[MCPMb], MCPb);
    writeAllMcp(MCP2_ADDR, MCP2u.MCP2[MCPMb], MCPb);
    if (delay_mili_micro > 0)
         delay(delay_v);    
    else
         delayMicroseconds(delay_v);      
}    
