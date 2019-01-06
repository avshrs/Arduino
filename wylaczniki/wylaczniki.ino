#include <Wire.h>
#include <Arduino.h>
#include "print_s.h"
#include "vars.h"
#include "MCP23017.h"
#include "helpers.h"
#include <EtherCard.h>
#include <IPAddress.h>

int delay_v = 10;  

char textToSend[] = "test 123";

MCP *mcpc[8];

PrintBin pb;

Communication comm;

SERIALMCPFRAME* data_udp;
//-begin-------------------------------------------------------------------------//

// ethernet mac address - must be unique on your network
static byte mymac[] = { 0x70,0x69,0x69,0x2D,0x30,0x31 };

byte Ethernet::buffer[700]; // tcp/ip send and receive buffer
static uint32_t timer;
const char website[] PROGMEM = "192.168.1.110";
const int dstPort PROGMEM = 1234;
const int srcPort PROGMEM = 4321;

//callback that prints received packets to the serial port

void udpSerialPrint(uint16_t dest_port, uint8_t src_ip[IP_LEN], uint16_t src_port, const char *data, uint16_t len){
    IPAddress src(src_ip[0],src_ip[1],src_ip[2],src_ip[3]);
    data_udp =(SERIALMCPFRAME*)data;
      comm.checkPayloadData(data_udp,mcpc,delay_v);
}

//-end---------------------------------------------------------------------------//


void setup(){
    Serial.begin(1000000);
    mcpc[MPC1] = new MCP(MCP1_ADDR, MCP_IN, MCP_PULLUP, MCP_OUT, MCP_NOT_PULLUP);
    mcpc[MPC2] = new MCP(MCP2_ADDR, MCP_IN, MCP_PULLUP, MCP_OUT, MCP_NOT_PULLUP);
   
   
    //-----begin---------------------------------------------------------------------//   
    if (ether.begin(sizeof Ethernet::buffer, mymac, SS) == 0)
        Serial.println(F("Failed to access Ethernet controller"));
    if (!ether.dhcpSetup())
        Serial.println(F("DHCP failed"));

    ether.printIp("IP:  ", ether.myip);
    ether.printIp("GW:  ", ether.gwip);
    ether.printIp("DNS: ", ether.dnsip);
    
    if (!ether.dnsLookup(website))
        Serial.println("DNS failed");

    ether.printIp("SRV: ", ether.hisip);

    //register udpSerialPrint() to port 1337
    ether.udpServerListenOnPort(&udpSerialPrint, 1337);

    //register udpSerialPrint() to port 42.
    ether.udpServerListenOnPort(&udpSerialPrint, 42);
    //-----end-----------------------------------------------------------------------//

}

void loop(){
    mcpc[MPC1]->readAll(SIDEA);
    mcpc[MPC1]->writeAll(mcpc[MPC1]->McpMemory[SIDEA],SIDEB,NFORCE);

    mcpc[MPC2]->readAll(SIDEA);
    mcpc[MPC2]->writeAll(mcpc[MPC2]->McpMemory[SIDEA],SIDEB,NFORCE);
    
    //--------------------------------------------------------------------------//

    ether.packetLoop(ether.packetReceive());
    if (millis() > timer) {
      timer = millis() + 5000;
     //static void sendUdp (char *data,uint8_t len,uint16_t sport, uint8_t *dip, uint16_t dport);
     ether.sendUdp(textToSend, sizeof(textToSend), srcPort, ether.hisip, dstPort );
    }
    //--------------------------------------------------------------------------//
    
    delay(delay_v);    
}    
