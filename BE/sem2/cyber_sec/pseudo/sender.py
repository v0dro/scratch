from generator import RandGen
import socket
import json

def encrypt(message, key):
  cipher = []
  for letter in message:
    cipher.append(ord(letter) + key)

  return cipher

gen = RandGen(3)

while True:
  message = raw_input("Enter your message: ")
  sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
  sock.connect((socket.gethostname(), 2500))

  key = gen.generate()
  cipher = json.dumps(encrypt(message, key))
  sock.send(cipher)
  sock.close()


