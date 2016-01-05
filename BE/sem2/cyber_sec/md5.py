import math

def F(b, c, d):
  lhs = b & c
  rhs = ~b & d
  return (lhs | rhs)

def G(b, c, d):
  lhs = d & b
  rhs = ~d & c
  return (lhs | rhs)  

def H(b,c,d):
  return (b ^ c ^ d)

def I(b, c, d):
  rhs = b | ~d
  return (c ^ rhs)

def pad_bit_string(message):
  message = bytearray(message) #copy our input into a mutable buffer
  orig_len_in_bits = (8 * len(message)) & 0xffffffffffffffff
  message.append(0x80)
  while len(message)%64 != 56:
      message.append(0)
  message += orig_len_in_bits.to_bytes(8, byteorder='little')

  return message

def leftrotate(x, c):
  x &= 0xFFFFFFFF
  return (((x << c) | (x >> (32-c))) & 0xFFFFFFFF)

def compute_checksum(bit_string):
  K = []
  num_chunks = []

  # s specifies the per-round shift amounts
  s = [7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
       5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
       4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
       6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21]

  for i in range(int(len(bit_string)/64)):
    num_chunks.append(bit_string[i * 64:(i + 1) * 64])

  for i in range(64):
    K.append(int((2**32) * abs(math.sin(i + 1))) & 0xFFFFFFFF)

  a0 = 0x67452301
  b0 = 0xefcdab89
  c0 = 0x98badcfe
  d0 = 0x10325476

  for chunk in num_chunks:
    # init hash values for this chunk
    A = a0
    B = b0
    C = c0
    D = d0

    for i in range(64):
      if i >= 0 and i <= 15:
        f = F(B, C, D)
        g = i
      elif i >= 16 and i <= 31:
        f = G(B, C, D)
        g = (5*i + 1) % 16
      elif i >= 32 and i <= 47:
        f = H(B, C, D)
        g = (3*i + 5) % 16
      elif i >= 48 and i <= 63:
        f = I(B, C, D)
        g = (7 * i) % 16

      to_rotate = A + f + K[i] + int.from_bytes(chunk[4*g:4*g+4], byteorder='little')
      new_B = (B + leftrotate(to_rotate, s[i])) & 0xFFFFFFFF
      A, B, C, D = D, new_B, B, C

    a0 += A
    a0 &= 0xFFFFFFFF
    b0 += B
    b0 &= 0xFFFFFFFF
    c0 += C
    c0 &= 0xFFFFFFFF
    d0 += D
    d0 &= 0xFFFFFFFF

  return sum(x<<(32*i) for i, x in enumerate([a0,b0,c0,d0]))

def md5_to_hex(digest):
  raw = digest.to_bytes(16, byteorder='little')
  return '{:032x}'.format(int.from_bytes(raw, byteorder='big'))

def generate_md5_checksum(str):
  bit_string = pad_bit_string(str)
  return md5_to_hex(compute_checksum(bit_string))

if generate_md5_checksum(b"The quick brown fox jumps over the lazy dog") == "9e107d9d372bb6826bd81d3542a419d6":
  print("TEST PASSED!")
else:
  print("TEST FAILED.")
