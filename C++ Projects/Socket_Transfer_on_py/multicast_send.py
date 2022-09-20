from socket import *
import struct

group_addr = ("235.7.8.9", 9000)
s_sock = socket(AF_INET, SOCK_DGRAM)
s_sock.settimeout(0.5)

TTL = struct.pack('@i', 2)
s_sock.setsockopt(IPPROTO_IP, IP_MULTICAST_TTL, TTL)

while True:
    rmsg = input('Your Message: ')
    s_sock.sendto(rmsg.encode(), group_addr)
    while True:
        import socket 
        try:
            response, addr = s_sock.recvfrom(1024)
        except timeout:
            break
        else:
            print('{} from {}'.format(response.decode(), addr))

            