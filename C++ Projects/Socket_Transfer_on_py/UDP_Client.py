import socket
BUFFSIZE = 512
port = 9000

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
msg = 'Hello UDP Server'
svrIP = input(("Server IP(default: 127.0.0.1): "))
if svrIP=='':
    svrIP = '127.0.0.1'
sock.sendto(msg.encode("euc-kr"), (svrIP, port))

while True:
    data, addr = sock.recvfrom(BUFFSIZE)
    print('Server Says: ', data.decode("euc-kr"))
    msg = input('Sending Messages: ', )
    sock.sendto(msg.encode("euc-kr"), addr)
