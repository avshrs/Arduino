#include <stdint.h>
#include <Wire.h>


uint8_t MCPMa = 0;
uint8_t MCPMb = 1;
uint8_t MCPFa = 2;
uint8_t MCPFb = 3;
uint8_t MCPa  = 0;
uint8_t MCPb  = 1;
uint8_t zero = 0;
uint8_t one  = 1;
uint8_t MCP1_ADDR  = 0x20;
uint8_t MCP2_ADDR  = 0x21;
uint8_t MCP_A_ADDR = 0x12; // Register Address of Port A
uint8_t MCP_B_ADDR = 0x13; // Register Address of Port B
uint8_t MCP_INIT   = 0x00; // MCP init
uint8_t MCP_TO_OUT = 0x00; // Register Address of Port A
uint8_t MCP_TO_IN  = 0x01; // Register Address of Port A


int delay_v = 10;   //refresh timer 10 ms
int delay_mili_micro = 1;

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

char RS_CHAR = "";
String RS_BUFFER = "";
String RS_VALUE = "";
char RS_DELIMITER = ",";
uint16_t RS_POSITION = 0;

bool receiver() {
    if(Serial.available()) {
      RS_CHAR = char(Serial.read());
      if(RS_CHAR == "\n") {
        RS_VALUE = RS_BUFFER;
        RS_BUFFER = "";
        return true;
      }
      else {
        RS_BUFFER += RS_CHAR;
        return false;
        }
    }
    else 
        return false;
}

String Cmd() {
    RS_POSITION = RS_VALUE.indexOf(RS_DELIMITER);
    if(RS_POSITION > -1) {
        return RS_VALUE.substring(0, RS_POSITION); 
    }   
}
bool isCmd(String CMD){
    if (CMD == Cmd())
        return true;
    else 
        return false;
}

String getValue() {
    RS_POSITION = RS_VALUE.indexOf(RS_DELIMITER);

    if(RS_POSITION > -1) {
      return RS_VALUE.substring(RS_POSITION + 1, RS_VALUE.length());
    }
    else {
      return "";
    }
  }

void print_binary(uint32_t &v)
{  
    for (uint8_t i = 0 ; i< sizeof(v)*8 ; ++i){
       if ((v & (1 << i )) > 0) 
           Serial.print("1");
       else
           Serial.print("0"); 
        Serial.println(" "); 
    }
}

void printAll(uint8_t &HUMAN,uint32_t &MEMORY){
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

void stateMcpToOn(uint8_t &I2C_ADDR,uint8_t PIN, uint8_t &MEMORY, uint8_t &FORCED,uint8_t SIDE, bool FORCE){
    uint8_t mask = (1 << PIN);
    if((MEMORY & mask) != mask){
        MEMORY |= mask;
        if(FORCE)
            FORCED ^= mask;
        Wire.beginTransmission(I2C_ADDR);
        if (SIDE == MCPa){
            Wire.write(MCP_A_ADDR);
            Wire.write(MEMORY);
            Serial.println(MEMORY);   
        }
        if (SIDE == MCPb){
            Wire.write(MCP_B_ADDR);
            Wire.write(MEMORY);  
            Serial.println(MEMORY);    
        }
        Wire.endTransmission();    
    }
 }




void getMcpValue(uint8_t &I2C_ADDR,uint8_t PIN, uint8_t &MEMORY, uint8_t &FORCED,uint8_t SIDE){
    uint8_t value = 0;
    Wire.beginTransmission(I2C_ADDR);
    if (SIDE == MCPa){
        Wire.write(MCP_A_ADDR);
        Wire.requestFrom((int)I2C_ADDR, 1);
        while(Wire.available()) {   
            value = Wire.read();
        } 
    }
    if (SIDE == MCPb){
        Wire.write(MCP_B_ADDR);
        Wire.requestFrom((int)I2C_ADDR, 1);
        while(Wire.available()) {   
            value = Wire.read();
        }       
    }
    Wire.endTransmission();    
    uint8_t mask = (1 << PIN);
    if (( FORCED & mask) != mask){
      if (( MEMORY & mask) > (value & mask))
            MEMORY |= mask;
      if (( MEMORY & mask) < (value & mask))
            MEMORY &= ~mask;
    }
}

void stateMcpToOff(uint8_t &I2C_ADDR, uint8_t PIN, uint8_t &MEMORY, uint8_t &FORCED,uint8_t SIDE, bool FORCE){
        
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


void serialCom(){

    if(receiver()) {
        
        if(isCmd("test")) {
            Serial.println("True");
        }
        if(isCmd("delay")) {
            delay_v = getValue().toInt();
        }
        if(isCmd("delay_mu")) {
            delay_mili_micro = getValue().toInt();
        }

        if(isCmd("gMCP1")) {
            printAll(zero,MCP1u.MCP1p);
           
        }
        
        if(isCmd("gMCP2")) {
             printAll(one,MCP1u.MCP1p);
        }
        
        
       

    }
}

void initMcp(uint8_t &MCP_ADDR, uint8_t &MCP_INIT,uint8_t &MCP_A,uint8_t &MCP_B ){   
    Wire.begin();
    Wire.beginTransmission(MCP_ADDR);
    Wire.write(MCP_INIT);
    Wire.write(MCP_A);
    Wire.write(MCP_B);
    Wire.endTransmission();
}

void setup() {
    Serial.begin(115200);
    initMcp(MCP1_ADDR, MCP_INIT, MCP_TO_OUT, MCP_TO_OUT);
    initMcp(MCP2_ADDR, MCP_INIT, MCP_TO_OUT, MCP_TO_OUT);
}






void loop() {
    serialCom();
    for(uint8_t i=0; i<16;++i){  
        stateMcpToOn(MCP2_ADDR,i,MCP2u.MCP2[MCPMa],MCP2u.MCP2[MCPFa],MCPa,true);    
        serialCom();
        if (delay_mili_micro > 0)
             delay(delay_v);    
        else
             delayMicroseconds(delay_v);      
        stateMcpToOff(MCP2_ADDR,i,MCP2u.MCP2[MCPMa],MCP2u.MCP2[MCPFa],MCPa,true);
        serialCom();
        if (delay_mili_micro > 0)
             delay(delay_v);    
        else
             delayMicroseconds(delay_v);       
        stateMcpToOn(MCP1_ADDR,i,MCP1u.MCP1[MCPMb],MCP1u.MCP1[MCPFb],MCPb,true);    
        serialCom();
        if (delay_mili_micro > 0)
            delay(delay_v);    
        else
            delayMicroseconds(delay_v);      
        stateMcpToOff(MCP1_ADDR,i,MCP1u.MCP1[MCPMb],MCP1u.MCP1[MCPFb],MCPb,true);
        serialCom();
        if (delay_mili_micro > 0)
             delay(delay_v);    
        else
             delayMicroseconds(delay_v);       
   
     }    
}
