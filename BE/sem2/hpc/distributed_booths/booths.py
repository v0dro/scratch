multiplier   = get_number_from_client('localhost', 1000)
multiplicand = get_number_from_client('localhost', 1001)
bits = 5

if multiplicand < 0:
  multiplicand = twos_compliment(bin(multiplicand)[2:].zfill(bits))
else:
  multiplicand = bin(multiplicand)[2:].zfill(bits)

if multiplier < 0:
  multiplier = twos_compliment(bin(multiplier)[2:].zfill(bits))
else:
  multiplier = bin(multiplier)[2:].zfill(multiplier)


def get_number_from_client(ip, port):
  pass

def twos_compliment(bit_string):
  pass