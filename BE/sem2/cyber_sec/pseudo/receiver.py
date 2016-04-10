from generator import RandGen
import socket
import json

def decrypt(message, key):
  text = []
  for cipher in message:
    text.append(str(unichr(cipher - key)))

  return text

g = RandGen(3)
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((socket.gethostname(), 2500))
s.listen(5)
while True:
  c, addr = s.accept()

  key = g.generate()
  cipher = json.loads(c.recv(4096))
  print "Recevied cipher text", str(cipher)
  print decrypt(cipher, key)
  c.close()
