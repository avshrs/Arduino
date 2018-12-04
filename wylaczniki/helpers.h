#include <Wire.h>


class ArduMCP{
public:
void initMcp(const uint8_t &MCP_ADDR, const  uint8_t &MCP_SIDE, const uint8_t &MCP_DIRECTION);
void writeMcp(const uint8_t &MCP_ADDR,uint8_t &MCP_MEMORY, const  uint8_t MCP_RW_SIDE);
uint8_t readMcp(const uint8_t &MCP_ADDR, const uint8_t MCP_A);
};