import curses
import serial

ser = serial.Serial('/dev/ttyUSB0', 1000000)
# ser.open()
ser.write(b'\x00\x00\xd9')
ser.close()