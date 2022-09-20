import socket
import time

server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)

server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

# Enable broadcasting mode
server.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)

server.settimeout(0.2)
while True:
    message = input('Broadcast Message: ')
    server.sendto(message.encode("euc-kr"), ('<broadcast>', 1060))
    print("{} sent!".format(message))
