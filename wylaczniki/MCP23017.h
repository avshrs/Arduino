#ifndef MCP23S17_h
#define MCP23S17_h

#define    IODIRA    (0x00)      // MCP23x17 I/O Direction Register
#define    IODIRB    (0x01)      // 1 = Input (default), 0 = Output

#define    IPOLA     (0x02)      // MCP23x17 Input Polarity Register
#define    IPOLB     (0x03)      // 0 = Normal (default)(low reads as 0), 1 = Inverted (low reads as 1)

#define    GPINTENA  (0x04)      // MCP23x17 Interrupt on Change Pin Assignements
#define    GPINTENB  (0x05)      // 0 = No Interrupt on Change (default), 1 = Interrupt on Change

#define    DEFVALA   (0x06)      // MCP23x17 Default Compare Register for Interrupt on Change
#define    DEFVALB   (0x07)      // Opposite of what is here will trigger an interrupt (default = 0)

#define    INTCONA   (0x08)      // MCP23x17 Interrupt on Change Control Register
#define    INTCONB   (0x09)      // 1 = pin is compared to DEFVAL, 0 = pin is compared to previous state (default)

#define    IOCON     (0x0A)      // MCP23x17 Configuration Register

#define    GPPUA     (0x0C)      // MCP23x17 Weak Pull-Up Resistor Register
#define    GPPUB     (0x0D)      // INPUT ONLY: 0 = No Internal 100k Pull-Up (default) 1 = Internal 100k Pull-Up 

#define    INTFA     (0x0E)      // MCP23x17 Interrupt Flag Register
#define    INTFB     (0x0F)      // READ ONLY: 1 = This Pin Triggered the Interrupt

#define    INTCAPA   (0x10)      // MCP23x17 Interrupt Captured Value for Port Register
#define    INTCAPB   (0x11)      // READ ONLY: State of the Pin at the Time the Interrupt Occurred

#define    GPIOA     (0x12)      // MCP23x17 GPIO Port Register
#define    GPIOB     (0x13)      // Value on the Port - Writing Sets Bits in the Output Latch

#define    OLATA     (0x14)      // MCP23x17 Output Latch Register
#define    OLATB     (0x15)      // 1 = Latch High, 0 = Latch Low (default) Reading Returns Latch State, Not Port Value!


#include <Arduino.h>

class MCP {
    public:
        MCP(uint8_t, uint8_t);                   // Constructor to instantiate a discrete IC as an object, address 0-7, chipSelect any valid pin
        void init();                             
        void byteWrite(uint8_t, uint8_t);        // Typically only used internally, but allows the user to write any register if needed, so it's public
        void pinMode(uint8_t, uint8_t);          // Sets the mode (input or output) of a single I/O pin
        void pullupMode(uint8_t, uint8_t);       // Selects internal 100k input pull-up of a single I/O pin
        void pullupMode(unsigned int);           // Selects internal 100k input pull-up of all I/O pins at once
        void inputInvert(uint8_t, uint8_t);      // Selects input state inversion of a single I/O pin (writing 1 turns on inversion) 
        
        void digitalWrite(uint8_t, uint8_t);     // Sets an individual output pin HIGH or LOW
        void digitalWrite(unsigned int);         // Sets all output pins at once. If some pins are configured as input, those bits will be ignored on write
        uint8_t digitalRead(uint8_t);            // Reads an individual input pin
        uint8_t byteRead(uint8_t);               // Reads an individual register and returns the byte. Argument is the register address
        unsigned int digitalRead(void);          // Reads all input  pins at once. Be sure it ignore the value of pins configured as output!
};


#endif //MCP23S17