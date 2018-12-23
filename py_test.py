import time
import serial
from ctypes import *
import struct


# configure the serial connections (the parameters differs on the device you are connecting to)
ser = serial.Serial(
    port='/dev/tty.usbserial-1D110',
    baudrate=1000000,
)
ser = serial.Serial()
ser.baudrate = 1000000
ser.port = '/dev/tty.usbserial-1D110'
ser.parity="N"
ser.bytesize = 8
ser.stopbits=1
ser.open()
ser.is_open
print ser
aaa=0.05
time.sleep(2)
def readp(i):
        out=""
        time.sleep(0.1).
        while ser.inWaiting() > 0:
                out += ser.read()
        if out !="":
                print "{0} {1}".format(i,out)
  

while True:
        for i in range(0,8):  

                HEAD = 0xb1   #0xA0 read value from side = A /0xB0 read value from side = b //0xA1 write walue A //0xB1 write walue  B
                MCPADDRESS = 0x20  # //sensor number i2c adres eg 0x20, 0x21 to 0x27
                PIN = i
                RW = 1
                frame = int('{:08b}'.format(HEAD)+'{:08b}'.format(MCPADDRESS)+'{:08b}'.format(PIN)+'{:08b}'.format(RW), 2)
                frame_be = struct.pack('>I', frame)
                ser.write(frame_be)
                
                out = ""
                readp(i) 
                time.sleep(aaa)
                HEAD = 0xb1   #0xA0 read value from side = A /0xB0 read value from side = b //0xA1 write walue A //0xB1 write walue  B
                MCPADDRESS = 0x21  # //sensor number i2c adres eg 0x20, 0x21 to 0x27
                PIN = i
                RW = 1
                frame = int('{:08b}'.format(HEAD)+'{:08b}'.format(MCPADDRESS)+'{:08b}'.format(PIN)+'{:08b}'.format(RW), 2)
                frame_be = struct.pack('>I', frame)
                ser.write(frame_be)
                readp(i)
                time.sleep(aaa)
        for i in range(0,8): 
                HEAD = 0xb1   #0xA0 read value from side = A /0xB0 read value from side = b //0xA1 write walue A //0xB1 write walue  B
                MCPADDRESS = 0x20  # //sensor number i2c adres eg 0x20, 0x21 to 0x27
                PIN = i
                RW = 0
                frame = int('{:08b}'.format(HEAD)+'{:08b}'.format(MCPADDRESS)+'{:08b}'.format(PIN)+'{:08b}'.format(RW), 2)
                frame_be = struct.pack('>I', frame)
                ser.write(frame_be)
                readp(i)
                out = ""
                time.sleep(aaa)        
                HEAD = 0xb1   #0xA0 read value from side = A /0xB0 read value from side = b //0xA1 write walue A //0xB1 write walue  B
                MCPADDRESS = 0x21  # //sensor number i2c adres eg 0x20, 0x21 to 0x27
                PIN = i
                RW = 0
                frame = int('{:08b}'.format(HEAD)+'{:08b}'.format(MCPADDRESS)+'{:08b}'.format(PIN)+'{:08b}'.format(RW), 2)
                frame_be = struct.pack('>I', frame)
                ser.write(frame_be)
                readp(i)

                time.sleep(aaa)