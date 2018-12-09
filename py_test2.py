import time
import serial

# configure the serial connections (the parameters differs on the device you are connecting to)
ser = serial.Serial(
    port='/dev/tty.usbserial-1D110',
    baudrate=1000000,
)

out = ""
while True:
   pass
