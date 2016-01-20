import math

def pad_message(message):
  message = bytearray(message) #copy our input into a mutable buffer
  orig_len_in_bits = (8 * len(message)) & 0xffffffffffffffff
  message.append(0x80)
  while len(message)%64 != 56:
      message.append(0)
  message += orig_len_in_bits.to_bytes(8, byteorder='big')

  return message

def break_into_32_bit_chunks(chunk):
  broken_chunks = []
  for i in range(16):
    broken_chunks.append(int.from_bytes(chunk[i * 4:(i + 1) * 4], byteorder='big'))

  return broken_chunks

def F(b, c, d):
  return (((b & c) | ((~b) & d)) & 0xFFFFFFFF)

def G(b, c, d):
  return ((b ^ c ^ d) & 0xFFFFFFFF)

def H(b, c, d):
  return (((b & c) | (b & d) | (c & d)) & 0xFFFFFFFF)

def leftrotate(x, c):
  x &= 0xFFFFFFFF
  return (((x << c) | (x >> (32-c))) & 0xFFFFFFFF)

def calculate_sha1_key(message):
  h0 = 0x67452301
  h1 = 0xEFCDAB89
  h2 = 0x98BADCFE
  h3 = 0x10325476
  h4 = 0xC3D2E1F0

  padded_byte_array = pad_message(message)

  for i in range(0, len(padded_byte_array), 64):
    chunk = padded_byte_array[i * 64: (i + 1) * 64]
    w = break_into_32_bit_chunks(chunk)

    for j in range(16,80):
      w.append((leftrotate(w[j-3] ^ w[i-8] ^ w[i-14] ^ w[i-16], 1) & 0xFFFFFFFF))

    a, b, c, d, e = h0, h1, h2, h3, h4

    for j in range(80):
      if j >= 0 and j <= 19:
        f = F(b, c, d)
        k = 0x5A827999
      elif j >= 20 and j <= 39:
        f = G(b, c, d)
        k = 0x6ED9EBA1
      elif j >= 40 and j <= 59:
        f = H(b, c, d)
        k = 0x8F1BBCDC
      elif j >= 60 and j <= 79:
        f = G(b, c, d)
        k = 0xCA62C1D6

      temp = (leftrotate(a, 5) + f + e + k + w[j]) & 0xFFFFFFFF
      e = d
      d = c
      c = leftrotate(b, 30)
      b = a
      a = temp

    h0 += a
    h0 &= 0xFFFFFFFF
    h1 += b
    h1 &= 0xFFFFFFFF
    h2 += c
    h2 &= 0xFFFFFFFF
    h3 += d
    h3 &= 0xFFFFFFFF
    h4 += e
    h4 &= 0xFFFFFFFF

  return hex(sum(x<<(32*i) for i, x in enumerate([h4,h3,h2,h1,h0])))[2:]

if calculate_sha1_key(
  b"The quick brown fox jumps over the lazy dog") == "2fd4e1c67a2d28fced849ee1bb76e7391b93eb12":
  print("TEST PASS!")
else:
  print("TEST FAIL :\\")