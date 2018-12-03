char   RS_CHAR;
String RS_BUFFER    = "";
String RS_VALUE     = "";
char   RS_DELIMITER = ',';
int    RS_POSITION  = 0;
char   RS_FLAG      = '\n';

int    delay_v          = 1;   //refresh timer 10 ms
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

void serialCom(){
  if(rsReceiver()) {
    if(isCmd("test")) {
      Serial.println("True");
    }
   /* if(isCmd("delay")) {
      delay_v = 1;
      Serial.println(getValue().toInt());
    }
    if(isCmd("delay_mu")) {
      delay_mili_micro = getValue().toInt();
      Serial.println(delay_mili_micro);
    }
    if(isCmd("readMCP1")) {
      printAll(HUMAN,&MCP1s);
    }
    if(isCmd("BinReadMCP1")) {
      printAll(BINARY,MCP1s);
    }
    if(isCmd("readMCP2")) {
      printAll(HUMAN,MCP2s);
    }
    if(isCmd("BinReadMCP1")) {
      printAll(BINARY,MCP2s);
    }
    if(isCmd("setMCP1aOn")) {
      Serial.println("on");
      setMcpToOn(MCP1_ADDR, getValue().toInt() , MCP1s.MCPAM,  MCP1s.MCPAF, MCPa, true);
    }
    if(isCmd("setMCP1bOn")) {
      Serial.println("on");
      setMcpToOn(MCP1_ADDR, getValue().toInt() , MCP1s.MCPBM,  MCP1s.MCPBF, MCPb, true);
    }
    if(isCmd("setMCP1aOff")) {
      setMcpToOff(MCP1_ADDR, getValue().toInt(), MCP1s.MCPAM,  MCP1s.MCPAF, MCPa, true);
    }
    if(isCmd("setMCP1bOff")) {
      setMcpToOff(MCP1_ADDR, getValue().toInt() ,  MCP1s.MCPBM,  MCP1s.MCPBF, MCPb, true);
    }
  */

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