
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
// mcp sides 
#define  MCPa_INIT   0x00
#define  MCPb_INIT   0x01

#define  MCP_A_RW 0x12 // Register Address of Port A
#define  MCP_B_RW 0x13 // Register Address of Port B

#define  MCP_TO_OUT 0x00 // Register Address of Port A
#define  MCP_TO_IN  0xFF // Register Address of Port A


struct SMCP{
       uint8_t MCPAM = 0;
       uint8_t MCPAF = 0;
       uint8_t MCPAS = 0;
       uint8_t MCPBM = 0;
       uint8_t MCPBF = 0;
       uint8_t MCPBS = 0;
};

SMCP MCP1s; 
SMCP MCP2s; 
SMCP MCP3s; 
SMCP MCP4s; 
SMCP MCP5s; 
SMCP MCP6s; 
SMCP MCp7s; 
SMCP MCP8s; 

struct McpConf{
       uint8_t MCPAM = 0;
       uint8_t MCPAF = 0;
       uint8_t MCPAS = 0;
       uint8_t MCPBM = 0;
       uint8_t MCPBF = 0;
       uint8_t MCPBS = 0;
};