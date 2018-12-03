#include <stdint.h>
#include <Wire.h>
//uinon struct
#define  MCPMa  0x00
#define  MCPMb  0x01
#define  MCPFa  0x02
#define  MCPFb  0x03
#define  MCPSa  0x04
#define  MCPSb  0x05
// mcp sides 
#define  MCPa   0x00
#define  MCPb   0x01
//helpers 
#define  HUMAN  0x01
#define  BINARY 0x00
// MCP addresses 
#define  MCP1_ADDR  0x20 //a2 gnd | a1 gnd | a0 gnd 
#define  MCP2_ADDR  0x21 //a2 gnd | a1 gnd | a0 +5v
#define  MCP3_ADDR  0x22 //a2 gnd | a1 +5  | a0 gnd 
#define  MCP4_ADDR  0x23 //a2 gnd | a1 +5  | a0 +5v
#define  MCP5_ADDR  0x24 //a2 +5  | a1 gnd | a0 gnd 
#define  MCP6_ADDR  0x25 //a2 +5  | a1 gnd | a0 +5v
#define  MCP7_ADDR  0x26 //a2 +5  | a1 +5  | a0 gnd
#define  MCP8_ADDR  0x27 //a2 +5  | a1 +5  | a0 +5v
//
#define  MCP_A_ADDR 0x12 // Register Address of Port A
#define  MCP_B_ADDR 0x13 // Register Address of Port B

#define  MCP_INIT   0x00 // MCP init
#define  MCP_TO_OUT 0x00 // Register Address of Port A
#define  MCP_TO_IN  0xFF // Register Address of Port A

char   RS_CHAR;
String RS_BUFFER    = "";
String RS_VALUE     = "";
char   RS_DELIMITER = ',';
int    RS_POSITION  = 0;
char   RS_FLAG      = '\n';

int    delay_v          = 1;   //refresh timer 10 ms
int    delay_mili_micro = 1;

union uMCP{
  uint32_t MCPp;     
  uint8_t  MCP[4]; 
}; 
uMCP MCP1u = {
  0};
uMCP MCP2u = {
  0};
uMCP MCP3u = {0};
uMCP MCP4u = {0};
uMCP MCP5u = {0};
uMCP MCP6u = {0};
uMCP MCP7u = {0};
uMCP MCP8u = {0};

struct SMCP{
       uint8_t MCPAM = 0;
       uint8_t MCPAF = 0;
       uint8_t MCPAS = 0;
       uint8_t MCPBM = 0;
       uint8_t MCPBF = 0;
       uint8_t MCPBS = 0;
};

SMCP MPC1s; 
SMCP MPC2s; 
SMCP MPC3s; 
SMCP MPC4s; 
SMCP MPC5s; 
SMCP MPC6s; 
SMCP MPC7s; 
SMCP MPC8s; 

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

void readAllMcp(const uint8_t &I2C_ADDR, uint8_t &MEMORY, uint8_t &FORCED,const uint8_t SIDE){
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
  MEMORY = MEMORY^((~FORCED)&(value^MEMORY));
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
      printAll(HUMAN,MCP1u.MCPp);
    }
    if(isCmd("BinReadMCP1")) {
      printAll(BINARY,MCP1u.MCPp);
    }
    if(isCmd("readMCP2")) {
      printAll(HUMAN,MCP2u.MCPp);
    }
    if(isCmd("BinReadMCP1")) {
      printAll(BINARY,MCP2u.MCPp);
    }
    if(isCmd("setMCP1aOn")) {
      Serial.println("on");
      setMcpToOn(MCP1_ADDR, getValue().toInt() , MCP1u.MCP[MCPMa], MCP1u.MCP[MCPFa], MCPa, true);
    }
    if(isCmd("setMCP1bOn")) {
      Serial.println("on");
      setMcpToOn(MCP1_ADDR, getValue().toInt() , MCP1u.MCP[MCPMb], MCP1u.MCP[MCPFb], MCPb, true);
    }
    if(isCmd("setMCP1aOff")) {
      setMcpToOff(MCP1_ADDR, getValue().toInt() , MCP1u.MCP[MCPMa], MCP1u.MCP[MCPFa], MCPa, true);
    }
    if(isCmd("setMCP1bOff")) {
      setMcpToOff(MCP1_ADDR, getValue().toInt() , MCP1u.MCP[MCPMb], MCP1u.MCP[MCPFb], MCPb, true);
    }
    if(isCmd("setMCP2aOn")) {
      setMcpToOn(MCP2_ADDR, getValue().toInt() , MCP2u.MCP[MCPMa], MCP2u.MCP[MCPFa], MCPa, true);
    }
    if(isCmd("setMCP2bOn")) {
      setMcpToOn(MCP2_ADDR, getValue().toInt() , MCP2u.MCP[MCPMb], MCP2u.MCP[MCPFb], MCPb, true);
    }
    if(isCmd("setMCP2aOff")) {
      setMcpToOff(MCP2_ADDR, getValue().toInt() , MCP2u.MCP[MCPMa], MCP2u.MCP[MCPFa], MCPa, true);
    }
    if(isCmd("setMCP2bOff")) {
      setMcpToOff(MCP2_ADDR, getValue().toInt() , MCP2u.MCP[MCPMb], MCP2u.MCP[MCPFb], MCPb, true);
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

void initMcp2(const uint8_t &MCP_ADDR, const  uint8_t &_MCP_INIT, const uint8_t &MCP_A, const uint8_t &MCP_B ){   
  Wire.begin();
  Wire.beginTransmission (MCP_ADDR);  // expander has I2C address 0x20
  Wire.write (0x00);   // register 0 is the I/O direction register for Port A
  Wire.write (0x00);   //  0x00 for all pins to output mode, 0xFF for all pins to input mode
  Wire.endTransmission ();
}

//http://www.gammon.com.au/forum/?id=10940

void initMcp(const uint8_t &MCP_ADDR, const  uint8_t &_MCP_INIT, const uint8_t &MCP_A, const uint8_t &MCP_B ){   
  Wire.begin();
  Wire.beginTransmission(MCP_ADDR);
  Wire.write(_MCP_INIT);
  Wire.write(MCP_A);
  Wire.write(MCP_B);
  Wire.endTransmission();
}

void setup(){
  Serial.begin(1000000);
  //  chipset addres, init seq., A side   , B side 
  initMcp(MCP1_ADDR, MCP_INIT, MCP_TO_IN, MCP_TO_OUT);
  initMcp(MCP2_ADDR, MCP_INIT, MCP_TO_IN, MCP_TO_OUT);
}

void loop(){
  serialCom();

  readAllMcp(MCP1_ADDR, MCP1u.MCP[MCPMa], MCP1u.MCP[MCPFa], MCPa);
  readAllMcp(MCP2_ADDR, MCP2u.MCP[MCPMa], MCP2u.MCP[MCPFa], MCPa);

  writeAllMcp(MCP2_ADDR, MCP2u.MCP[MCPMa], MCPb);
  writeAllMcp(MCP1_ADDR, MCP1u.MCP[MCPMa], MCPb);

  if (delay_mili_micro > 0)
    delay(delay_v);    
  else
    delayMicroseconds(delay_v);      
}    

