//#include <stdint.h>
#include <Wire.h>
#include "vars.h"
#include "helpers.h"
ArduMCP mcpArdu;
#include "print_s.h"
#include <EtherCard.h>
#include <IPAddress.h>


#define STATIC 0  // set to 1 to disable DHCP (adjust myip/gwip values below)
SERIALMCPFRAME* data_udp;
#if STATIC
// ethernet interface ip address
static byte myip[] = { 192,168,1,200 };
// gateway ip address
static byte gwip[] = { 192,168,1,1 };
#endif

// ethernet mac address - must be unique on your network
static byte mymac[] = { 0x70,0x69,0x69,0x2D,0x30,0x31 };

byte Ethernet::buffer[500]; // tcp/ip send and receive buffer

//callback that prints received packets to the serial port
void udpSerialPrint(uint16_t dest_port, uint8_t src_ip[IP_LEN], uint16_t src_port, const char *data, uint16_t len){
  IPAddress src(src_ip[0],src_ip[1],src_ip[2],src_ip[3]);
  
  data_udp =(SERIALMCPFRAME*)data;
  binSerialCom();
  mcpArdu.print_binary8(data_udp->HEAD);
  mcpArdu.print_binary8(data_udp->MCPADDRESS);
  mcpArdu.print_binary8(data_udp->PIN);
  mcpArdu.print_binary8(data_udp->RW);


}

int    delay_v          = 10;   //refresh timer 10 ms
int    delay_mili_micro = 0;

uint8_t binData[4]={0}; 
uint8_t byte_counter = 0;

uint8_t buffer = 0;
/*
bool binReceiver() {
    if(Serial.available()) {
        binData[byte_counter] = Serial.read();
        ++byte_counter;
        if (byte_counter > 3){
            byte_counter = 0;
            data_udp =(SERIALMCPFRAME*)binData;
            return true;
        }
    }
    else 
      return false;
}
*/




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
    mcpArdu.print_binary(MEMORY);
}

void binSerialCom(){
      
        if(data_udp->HEAD == 0xb1){  //#0xA0 read value from side = A /0xB0 read value from side = b //0xA1 write walue A //0xB1 write walue  B
          Serial.println("0xb1"); 
          if (data_udp->MCPADDRESS == 0x20){
              Serial.println("0x20"); 
           if (data_udp->RW != 0){
              mcpArdu.setMcpToOn(data_udp->MCPADDRESS,data_udp->PIN, MCP1s.MCPAM, MCP1s.MCPAF,MCP_B_RW, true);
              
              Serial.println(0x01); 
           }
           else{
                mcpArdu.setMcpToOff(data_udp->MCPADDRESS,data_udp->PIN, MCP1s.MCPAM, MCP1s.MCPAF,MCP_B_RW, true);
                Serial.println(0x00); 
           }
          }
          if (data_udp->MCPADDRESS == 0x21){
            if (data_udp->RW != 0){
                mcpArdu.setMcpToOn(data_udp->MCPADDRESS,data_udp->PIN, MCP2s.MCPAM, MCP2s.MCPAF,MCP_B_RW, true);
                Serial.println(0x01);
            }
            else{
                mcpArdu.setMcpToOff(data_udp->MCPADDRESS,data_udp->PIN, MCP2s.MCPAM, MCP2s.MCPAF,MCP_B_RW, true);
                Serial.println(0x00); 
            }
          }
          data_udp->MCPADDRESS= 0;
          data_udp->HEAD = 0;
          data_udp->PIN= 0;
        }
        
    
}






void setup(){
  Serial.begin(1000000);
  mcpArdu.initMcp(MCP1_ADDR, MCPa_INIT, MCP_TO_IN);
  mcpArdu.initMcp(MCP1_ADDR, MCPb_INIT, MCP_TO_OUT);
  mcpArdu.initMcp(MCP2_ADDR, MCPa_INIT, MCP_TO_IN);
  mcpArdu.initMcp(MCP2_ADDR, MCPb_INIT, MCP_TO_OUT);
  Serial.println(F("\n[backSoon]"));
  // Change 'SS' to your Slave Select pin, if you arn't using the default pin
  if (ether.begin(sizeof Ethernet::buffer, mymac, SS) == 0)
    Serial.println(F("Failed to access Ethernet controller"));
    
  

  if (!ether.dhcpSetup())
      Serial.println(F("DHCP failed"));

    ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);
  ether.printIp("DNS: ", ether.dnsip);

  //register udpSerialPrint() to port 1337
  ether.udpServerListenOnPort(&udpSerialPrint, 1337);

  //register udpSerialPrint() to port 42.
  ether.udpServerListenOnPort(&udpSerialPrint, 42);
  
}

void loop(){
  binSerialCom();
  //Serial.println(F("\n[backSoon]l"));
  ether.packetLoop(ether.packetReceive());

  mcpArdu.readAllMcp(MCP1_ADDR, MCP1s.MCPAM, MCP1s.MCPAF, MCP1s.MCPAS, MCP_A_RW);
  mcpArdu.writeMcp(MCP1_ADDR, MCP1s.MCPAM, MCP_B_RW);
  mcpArdu.readAllMcp(MCP2_ADDR, MCP2s.MCPAM, MCP2s.MCPAF, MCP2s.MCPAS, MCP_A_RW);
  mcpArdu.writeMcp(MCP2_ADDR, MCP2s.MCPAM, MCP_B_RW);


  if (delay_mili_micro > 0)
    delay(delay_v);    
  else
    delayMicroseconds(delay_v);      
}    
