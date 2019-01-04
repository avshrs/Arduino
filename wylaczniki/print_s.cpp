#include <Arduino.h>
#include "print_s.h"

void PrintBin::print_binary8(uint8_t &v){  
    for (uint8_t i = 0 ; i < (sizeof(v)*8) ; ++i){
       if ((v & (1 << i )) > 0) 
           Serial.print("1");
       else
           Serial.print("0"); 
    }
    Serial.println(" "); 
}

void PrintBin::print_binary3x8(uint8_t &v,uint8_t &v1,uint8_t &v2){  
    for (uint8_t i = 0 ; i < (sizeof(v)*8) ; ++i){
       if ((v & (1 << i )) > 0) 
           Serial.print("1");
       else
           Serial.print("0"); 
    }
    Serial.print(" "); 
    for (uint8_t i = 0 ; i < (sizeof(v1)*8) ; ++i){
       if ((v1& (1 << i )) > 0) 
           Serial.print("1");
       else
           Serial.print("0"); 
    }
    Serial.print(" "); 
    for (uint8_t i = 0 ; i < (sizeof(v2)*8) ; ++i){
       if ((v2& (1 << i )) > 0) 
           Serial.print("1");
       else
           Serial.print("0"); 
    }
    Serial.println(" "); 
}

void PrintBin::print_binary16(uint16_t &v){  
    for (uint8_t i = 0 ; i < (sizeof(v)*8) ; ++i){
       if ((v & (1 << i )) > 0) 
           Serial.print("1");
       else
           Serial.print("0"); 
    }
    Serial.println(" "); 
}

void PrintBin::print_binary32(uint32_t &v){  
    for (uint8_t i = 0 ; i < (sizeof(v)*8) ; ++i){
       if ((v & (1 << i )) > 0) 
           Serial.print("1");
       else
           Serial.print("0"); 
    }
    Serial.println(" "); 
}
