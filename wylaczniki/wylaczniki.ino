//#include <stdint.h>
#include <Wire.h>
#include <Arduino.h>
#include "print_s.h"
#include "vars.h"
#include "MCP23017.h"
#include "helpers.h"
#include </tmp/a/Arduino/wylaczniki/src/EtherCard.h>
#include <IPAddress.h>


int delay_v = 10;  
MCP *mcpc[8];
PrintBin pb;
Communication comm;

SERIALMCPFRAME* data_udp;

// ethernet mac address - must be unique on your network
static byte mymac[] = { 0x70,0x69,0x69,0x2D,0x30,0x31 };

byte Ethernet::buffer[500]; // tcp/ip send and receive buffer

//callback that prints received packets to the serial port
void udpSerialPrint(uint16_t dest_port, uint8_t src_ip[IP_LEN], uint16_t src_port, const char *data, uint16_t len){
  IPAddress src(src_ip[0],src_ip[1],src_ip[2],src_ip[3]);
  
  data_udp =(SERIALMCPFRAME*)data;
  comm.checkPayloadData(data_udp,mcpc,delay_v);

}


void setup(){
    Serial.begin(1000000);
    mcpc[MPC1] = new MCP(MCP1_ADDR, MCP_IN, MCP_PULLUP, MCP_OUT, MCP_NOT_PULLUP);
    mcpc[MPC2] = new MCP(MCP2_ADDR, MCP_IN, MCP_PULLUP, MCP_OUT, MCP_NOT_PULLUP);
   /*
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
    ether.udpServerListenOnPort(&udpSerialPrint, 42);*/
}

void loop(){
    mcpc[MPC1]->readAll(GPIOA);
    mcpc[MPC1]->writeAll(mcpc[MPC1]->McpMemory[SIDEA],GPIOB,NFORCE);

    pb.print_binary3x8(mcpc[MPC1]->McpMemory[SIDEA], mcpc[MPC1]->McpForce[SIDEA], mcpc[MPC1]->McpState[SIDEA]);

    mcpc[MPC2]->readAll(GPIOA);
    mcpc[MPC2]->writeAll(mcpc[MPC2]->McpMemory[SIDEA],GPIOB,NFORCE);

    pb.print_binary3x8(mcpc[MPC2]->McpMemory[SIDEA],mcpc[MPC2]->McpForce[SIDEA],mcpc[MPC2]->McpState[SIDEA]);

    //ether.packetLoop(ether.packetReceive());

    delay(delay_v);    
}    
