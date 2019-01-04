
#ifndef HELPERS_h
#define HELPERS_h

#include "vars.h"
#include <Arduino.h>
#include "MCP23017.h"

class Communication{
public:
    void checkPayloadData(SERIALMCPFRAME* data, MCP *mcpc[8]);
    

};

#endif //HELPERS_H
