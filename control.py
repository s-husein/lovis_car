import socket
import pygame

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
targ_ip = "192.168.18.90"
targ_port = 1234

sock.connect((targ_ip, targ_port))

theta = 0x7f
goal_theta = 0x7f

fwd = 0x00
bkwd = 0x00

fwd_goal = 0x00
bkwd_goal = 0x00

factor = 0x02
motor_fac = 0x05

pygame.init()
window=pygame.display.set_mode((200, 200))
clock = pygame.time.Clock()

def send_byte(ang, fwd, bkwd):
    global sock
    data = fwd.to_bytes(1, 'big') + bkwd.to_bytes(1, 'big') + ang.to_bytes(1, 'big')
    sock.send(data)


def calc_angle(goal):
    global theta, factor
    if theta < goal:
        theta += factor
    elif theta > goal:
        theta -= factor
    else:
        pass

def accel(goal_fwd):
    global fwd, motor_fac
    if fwd < goal_fwd:
        fwd += motor_fac
    elif fwd > goal_fwd:
        fwd -= motor_fac
    else:
        pass

def deccel(goal_bkwd):
    global bkwd, motor_fac
    if bkwd < goal_bkwd:
        bkwd += motor_fac
    elif bkwd > goal_bkwd:
        bkwd -= motor_fac
    else:
        pass

def brake():
    global ang
    

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
        fwd = 0xff
        bkwd = 0xff
        fwd_goal = 0xff
        bkwd_goal = 0xff
        
    
        

    calc_angle(goal_theta)
    accel(fwd_goal)
    deccel(bkwd_goal)
    send_byte(theta, fwd, bkwd)
    clock.tick(50)

pygame.quit()

