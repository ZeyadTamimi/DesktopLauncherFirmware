import socket
import sys
import binascii
# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

host = '192.168.43.197'
port = 80
print ('HOST: ' + str(host) + ', PORT: ' + str(port))
sock.bind((host, port))

sock.listen(1)

# establish a connection
conn, addr = sock.accept()
print('Connected by', addr)

while True:
    size = int.from_bytes(bytes.fromhex(conn.recv(4).decode()), byteorder='big')
    if size:
        size *= 2
        jpg_bytes_total = b''
        
        while size is not 0:
            jpg_bytes = conn.recv(size)
            size -= len(jpg_bytes)
            jpg_bytes_total += jpg_bytes
        
        jpg_as_hex = binascii.unhexlify(jpg_bytes_total.decode())
        with open('new_jpg.jpg', 'wb') as f:
            f.write(jpg_as_hex)