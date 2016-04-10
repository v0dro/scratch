import socket
import json
import hashlib
from math import pow

def egcd(a, b):
    if a == 0:
        return (b, 0, 1)
    else:
        g, y, x = egcd(b % a, a)
        print "G ", g, " y ", y, " x ", x
        return (g, x - (b // a) * y, y)

def modinv(a, m):
    g, x, y = egcd(a, m)
    if g != 1:
        raise Exception('modular inverse does not exist')
    else:
        return x % m

def H(m):
  return int(hashlib.sha1(m).hexdigest(), 16)

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.bind((socket.gethostname(), 2500))
sock.listen(5)
print "Listening...\n"
data = None
while True:
  c, addr = sock.accept()
  data = c.recv(4096)
  data = json.loads(data)
  
  p = data['p']
  q = data['q']
  g = data['g']
  y = data['y']
  message = data['message']
  r = data['r']
  s = data['s']

  w = modinv(s, q)
  u1 = (H(message) * w) % q
  u2 = (r * w) % q
  v = ((pow(g, u1) * pow(y, u2)) % p) % q

  print "v -> ", v

  if v == s:
    print "Message authentic."

  c.close()

