import socket

HOST = 'YOUR IP V4'
PORT = 8080

def main():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind((HOST, PORT))]
    sock.listen()
    #print('socket now listening')

    while True:
        client, address = sock.accept()
        print("Write D to show distance: ")
        message = input()
        client.sendall(bytes(message, 'utf-8'))
        content = client.recv(1024)
        if len(content) == 0:
           break
        else:
            print(content)
    client.close()
    # end while
# end function

if __name__ == '_main_':
    while True:
        main()