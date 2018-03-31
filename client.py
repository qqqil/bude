
import socket

address = ('172.19.80.17', 5000)
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(address)

s.send('hihi'.encode())

data = s.recv(512)
print('the data received is',data)

s.send('hihi'.encode())

s.close()