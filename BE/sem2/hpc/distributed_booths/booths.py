BITS = 5

def get_number_from_client(ip, port):
  pass

def add_binary(left, right):
  carry = False
  add = ''
  for a, b in zip(reversed(left), reversed(right)):
    a, b = int(a, 2), int(b, 2)

    if not carry:
      if a == 1 and b == 1:
        add = '0' + add
        carry = True
      elif (a == 1 and b == 0) or (a == 0 and b == 1):
        add = '1' + add
      elif a == 0 and b == 0:
        add = '0' + add
    else:
      if a == 1 and b == 1:
        add = '1' + add
      elif (a == 1 and b == 0) or (a == 0 and b == 1):
        add = '0' + add
      elif a == 0 and b == 0:
        add = '1' + add
        carry = False

  return add


def twos_compliment(bit_string):
  new = bit_string.replace('0', 'z').replace('1', '0').replace('z', '1')
  return add_binary(new, '1'.zfill(len(new)))

""" perform an arithmetic right shift """
def right_shift(acc, multiplier, booth_bit):
  string = acc + multiplier + booth_bit
  shifted = string[0] + string[:-1]

  return [shifted[0:BITS], shifted[BITS:BITS*2], shifted[-1]]

nmulti = multiplier   = 5
nmul = multiplicand = 3

if multiplicand < 0:
  multiplicand = twos_compliment(bin(multiplicand)[3:].zfill(BITS))
else:
  multiplicand = bin(multiplicand)[2:].zfill(BITS)

if multiplier < 0:
  multiplier = twos_compliment(bin(multiplier)[3:].zfill(BITS))
else:
  multiplier = bin(multiplier)[2:].zfill(BITS)

multiplicand_minus = twos_compliment(multiplicand)
booth_bit = '0'
acc = '0'.zfill(BITS)

for _ in xrange(0,BITS):
  if booth_bit == multiplier[-1]: # Q0 and Q-1 are 00 or 11
    acc, multiplier, booth_bit = right_shift(acc, multiplier, booth_bit)
  elif multiplier[-1] == '1' and booth_bit ==  '0': # Q0 and Q-1 are 10
    acc = add_binary(acc, multiplicand_minus)
    acc, multiplier, booth_bit = right_shift(acc, multiplier, booth_bit)
  elif multiplier[-1] == '0' and booth_bit == '1': # Q0 and Q-1 are 01
    acc = add_binary(acc, multiplicand)
    acc, multiplier, booth_bit = right_shift(acc, multiplier, booth_bit)


print "Booths result ", acc + multiplier
print "Actual result ", bin(nmulti*nmul)