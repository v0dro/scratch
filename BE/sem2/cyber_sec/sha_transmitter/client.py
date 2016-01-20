from sha1 import calculate_sha1_key
import socket

SHA_HASH = "2fd4e1c67a2d28fced849ee1bb76e7391b93eb12"

def validate_message(message):
  print(calculate_sha1_key(message))
  if calculate_sha1_key(message) == SHA_HASH:
    print("MESSAGE ARRIVED SUCCESSFULLY")
  else:
    print("MESSAGE TAMPERED WITH.")

s    = socket.socket()
port = 12345     
s.connect((b'localhost', port))

message = s.recv(1024)
validate_message(message)
s.close