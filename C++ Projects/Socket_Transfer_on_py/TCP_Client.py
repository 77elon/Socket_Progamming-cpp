import socket

# 소켓 정의(주소 체계, 소켓 타입정의)
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# 서버 주소 입력
svrIP = input(("Server IP(default: 127.0.0.1): "))
if svrIP=='':
    svrIP = '127.0.0.1'

#포트 번호 입력
port = input('port(default: 2500): ')

if port == '':
    port = 2500
else:
    port = int(port)

sock.connect((svrIP, port))
print('Connected to '+ svrIP)

index = 0
test = ['안녕하세요.', '반가워요.', '오늘따라 할 이야기가 많을 것 같네요.', '저도 그렇네요.']

while True:
    msg = input('Sending Message: ')

    # 송신 데이터가 없으면 다시 진행
    if not msg:
        msg = test[index]
        index += 1
        index %= 4
    
    try:
        sock.send(msg.encode('euc-kr'))
    
    #연결이 종료된 경우
    except:
        print('연결이 종료되었습니다.')
        break

    #데이터 읽기
    try:
        msg = sock.recv(1024)
        if not msg:
            print('연결이 종료되었습니다.')
            break
        print(f'Received Message: {msg.decode("euc-kr")}')

    #연결이 종료됨
    except:
        print('연결이 종료되었습니다.')
        break

    sock.close()