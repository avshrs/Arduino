#include <Wire.h>


class ArduMCP{
public:
void initMcp(const uint8_t &MCP_ADDR, const  uint8_t &MCP_SIDE, const uint8_t &MCP_DIRECTION);
void writeMcp(const uint8_t &MCP_ADDR,uint8_t &MCP_MEMORY, const  uint8_t MCP_RW_SIDE);
uint8_t readMcp(const uint8_t &MCP_ADDR, const uint8_t MCP_A);
void readAllMcp(const uint8_t &I2C_ADDR, uint8_t &MEMORY, uint8_t &FORCED,uint8_t &STATE, const uint8_t SIDE);
void print_binary8(uint8_t &v);
void setMcpToOn(const uint8_t &I2C_ADDR,uint8_t PIN, uint8_t &MEMORY, uint8_t &FORCED,const uint8_t SIDE, bool FORCE);
void setMcpToOff(const uint8_t &I2C_ADDR, uint8_t PIN, uint8_t &MEMORY, uint8_t &FORCED,const uint8_t SIDE, bool FORCE);
void print_binary(uint32_t &v);
void print_binary16(uint16_t &v);


};