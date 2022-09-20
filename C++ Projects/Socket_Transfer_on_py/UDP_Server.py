import socket
port = 9000
BUFFER = 512

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(('', port))
print('수신 대기 중')

while True:
    data, addr = sock.recvfrom(BUFFER)
    print('Received Message: ', data.decode("euc-kr"))
    print(addr)
    sock.sendto(data, addr)
    