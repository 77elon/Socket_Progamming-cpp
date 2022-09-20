import socket

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP) # UDP
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

# Enable broadcasting mode
sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)

sock.bind(("", 10000))
while True:
    # Thanks @seym45 for a fix
    data, addr = sock.recvfrom(1024)
    print("received message: {}".format(data.decode("euc-kr")))