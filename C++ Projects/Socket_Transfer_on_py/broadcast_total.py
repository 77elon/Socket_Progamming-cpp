#UDP Broadcasting Program
import argparse, socket


BUFSIZE = 1024

def client(network, port):
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
    text = '브로드캐스팅 메세지'
    sock.sendto(text.encode(), (network, port))
    message = input('Broadcast Message: ')
    sock.sendto(message.encode(), (network, port))

    


def server(interface, port):
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((interface, port))
    print('{}에서 수신 대기 중'. format(sock.getsockname()))
    while True:
        data, address = sock.recvfrom(BUFSIZE)
        text = data.decode()
        print('클라이언트({})의 브로드캐스팅 메세지 {!r}'.format(address, text))

if __name__ == '__main__':
    role = {'client' : client, 'server' :server}

    parser = argparse.ArgumentParser()
    parser.add_argument('-mode', choices=role)
    parser.add_argument('-host', default="<broadcast>")
    parser.add_argument('-p', type=int, default=10000)
    args = parser.parse_args()
    
    function = role[args.mode]
    function(args.host, args.p)
