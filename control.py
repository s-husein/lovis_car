import serial
import pygame

ser = serial.Serial('/dev/ttyUSB0', 1000000)

theta = 0x7f
goal_theta = 0x7f

fwd = 0x00
bkwd = 0x00

fwd_goal = 0x00
bkwd_goal = 0x00

factor = 0x01

pygame.init()
window=pygame.display.set_mode((200, 200))
clock = pygame.time.Clock()

def send_byte(ang, fwd, bkwd):
    global ser
    ser.write(fwd.to_bytes(1, 'big') + bkwd.to_bytes(1, 'big') + ang.to_bytes(1, 'big'))


def calc_angle(goal):
    global theta, factor
    if theta < goal:
        theta += factor
    elif theta > goal:
        theta -= factor
    else:
        pass

def accel(goal_fwd):
    global fwd, factor
    if fwd < goal_fwd:
        fwd += factor
    elif fwd > goal_fwd:
        fwd -= factor
    else:
        pass

def deccel(goal_bkwd):
    global bkwd, factor
    if bkwd < goal_bkwd:
        bkwd += factor
    elif bkwd > goal_bkwd:
        bkwd -= factor
    else:
        pass

# def brake():
    

run = True
while run:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            run = False

        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                run = False      
        elif event.type == pygame.KEYUP:
            goal_theta = 0x7f
            fwd_goal = 0x00
            bkwd_goal = 0x00

    key = pygame.key.get_pressed()
    if key[pygame.K_a]:
        goal_theta = 0x61
    if key[pygame.K_d]:
        goal_theta = 0x9d
    if key[pygame.K_w]:
        fwd_goal = 0xff
    if key[pygame.K_x]:
        bkwd_goal = 0xff
    if key[pygame.K_s]:
        fwd = bkwd
    
        

    calc_angle(goal_theta)
    accel(fwd_goal)
    deccel(bkwd_goal)
    send_byte(theta, fwd, bkwd)
    clock.tick(200)

pygame.quit()

