import time
import serial

# configure the serial connections (the parameters differs on the device you are connecting to)
ser = serial.Serial(
    port='/dev/tty.usbserial-1D110',
    baudrate=1000000,
)
fr = 0
to = 180
sl =1
ser.isOpen()
time.sleep(2)

a = bytearray([0xA0,0xA0,0x00,0x00])
b = "test\n"
ser.write(b)


out = ""
time.sleep(0.1)
while ser.inWaiting() > 0:
    out += ser.read(1)
if out !="":
    print out
 

