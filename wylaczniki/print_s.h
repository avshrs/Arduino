#ifndef PRINTBIN_h
#define PRINTBIN_h
#include <Arduino.h>

class PrintBin{
    public:
    void print_binary8(uint8_t &v);
    void print_binary3x8(uint8_t &v,uint8_t &v1,uint8_t &v2);
    void print_binary16(uint16_t &v);
    void print_binary32(uint32_t &v);
    

};

#endif