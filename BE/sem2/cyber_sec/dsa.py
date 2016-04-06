"""
author: @v0dro
"""

# Note: Learn the modular inverse precisely. Important stuff.

import math
import hashlib
import random

def egcd(a, b):
    if a == 0:
        return (b, 0, 1)
    else:
        g, y, x = egcd(b % a, a)
        return (g, x - (b // a) * y, y)

def modinv(a, m):
    g, x, y = egcd(a, m)
    if g != 1:
        raise Exception('modular inverse does not exist')
    else:
        return x % m

def get_tuples():
  # a prime number of length 512 bits
  p = 23 & 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
  # 160 bit prime factor of p-1 -> 1,2,11
  q = 11 & 0xFFFFFFFFFF
  # h is a number less than p-1 such that h^((p-1)/q) is greater than 1
  h = 2

  g = math.pow(h, float((p-1))/q) % p

  return [p, q, g]

def get_key_pair(p,q,g):
  # the private key is a number less than q
  x = 7
  #public key
  y = math.pow(g, x) % p

  return [x, y]

def H(m):
  return int(hashlib.sha1(m).hexdigest(), 16)

message = "I am ted."
p, q, g = get_tuples()
x, y = get_key_pair(p,q,g)

# prepare DSA signature
k = random.randint(1, q-1)
r = (int(math.pow(g, k)) % p) % q
# s = int((math.pow(k, -1) * (H(message) + x*r))) % q
k_inv = modinv(k, q)
s = int(k_inv * (H(message) + x*r)) % q

print "sender signatures: r -> ", str(r), " s -> ", str(s)

w = int(modinv(s, q))
u1 = (H(message) * w) % q
u2 = (r*w) % q
v = (int(math.pow(g, u1) * math.pow(y, u2)) % p) % q

print "verification of credentials: v -> ", str(v), " r -> ", str(r)