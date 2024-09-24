import curses
import serial
import numpy as np

ser = serial.Serial('/dev/ttyUSB0', 1000000)
angle = 0x7f
goal = 0x7f
factor = 0x01
# ser.open()
def main(stdscr):
    global goal, angle, factor
    while True:
        key = stdscr.getch()
        if key == ord('a'):
            goal = 0x52
        elif key == ord('d'):
            goal = 0xac
        elif key == ord('q'):
            break

        if angle < goal:
            angle += factor
        elif angle > goal:
            angle -= factor
        goal = 0x7f
        stdscr.addstr(0, 0, f'{key}')
        
        ser.write(b'\x00\x00'+ angle.to_bytes(1, 'big'))



curses.wrapper(main)


# ser.write(b'\x00\x00\x7f')
