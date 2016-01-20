import socket

SHA_HASH = "2fd4e1c67a2d28fced849ee1bb76e7391b93eb12"

def receive_and_validate_message():
  serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
  serversocket.bind((b'localhost', 12345))
  serversocket.listen(5)

  while True:
     c, addr = serversocket.accept()     # Establish connection with client.
     print('Got connection from', addr)
     c.send('The quick brown fox jumps over the lazy dog')
     c.close()  

receive_and_validate_message()