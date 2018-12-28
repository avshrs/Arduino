import socket
import time
import serial
from ctypes import *
import struct

UDP_IP = "192.168.1.249"
UDP_PORT = 1337


for i in range(0,8):
   # time.sleep(0.2)
    HEAD = 0xb1  # 10110001  #0xA0 read value from side = A /0xB0 read value from side = b //0xA1 write walue A //0xB1 write walue  B
    HEAD_ = '{:08b}'.format(HEAD)
    MCPADDRESS = 0x20  # //sensor number i2c adres eg 0x20, 0x21 to 0x27
    MCPADDRESS_  = '{:08b}'.format(MCPADDRESS)
    PIN = i
    PIN_ = '{:08b}'.format(PIN)
    RW = 1
    RW_ = '{:08b}'.format(RW)
    HEAD_ = '{:08b}'.format(HEAD)
    #frame = int(HEAD_+MCPADDRESS_[::-1]+PIN_[::-1]+RW_[::-1], 2)
    frame = int(HEAD_+MCPADDRESS_+PIN_+RW_, 2)
    frame_be = struct.pack('>I', frame)

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP
    sock.sendto(frame_be , (UDP_IP, UDP_PORT))

for i in range(7,-1,-1):
    #time.sleep(0.2)
    HEAD = 0xb1  # 10110001  #0xA0 read value from side = A /0xB0 read value from side = b //0xA1 write walue A //0xB1 write walue  B
    HEAD_ = '{:08b}'.format(HEAD)
    MCPADDRESS = 0x20  # //sensor number i2c adres eg 0x20, 0x21 to 0x27
    MCPADDRESS_  = '{:08b}'.format(MCPADDRESS)
    PIN = i
    PIN_ = '{:08b}'.format(PIN)
    RW = 0
    RW_ = '{:08b}'.format(RW)
    HEAD_ = '{:08b}'.format(HEAD)
    #frame = int(HEAD_+MCPADDRESS_[::-1]+PIN_[::-1]+RW_[::-1], 2)
    frame = int(HEAD_+MCPADDRESS_+PIN_+RW_, 2)
    frame_be = struct.pack('>I', frame)

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP
    sock.sendto(frame_be , (UDP_IP, UDP_PORT))