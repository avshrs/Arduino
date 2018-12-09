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


a = bytearray([0b11111100,0b11111111])
for i in range(0,9):
    ser.write(a)
    out = ""
    time.sleep(0.01)
    while ser.inWaiting() > 0:
        out += ser.read()
    if out !="":
        print out