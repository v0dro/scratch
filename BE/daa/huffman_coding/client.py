import socket
import json
import huffman

def calculate_frequencies():
  freqs = {}
  for c in string:  
    if freqs.has_key(c):
      freqs[c] += 1
    else:
      freqs[c] = 1

  return freqs

def compute_freq_table(freqs):
  dict_list = []
  for key, value in freqs.iteritems():
    dict_list.append([key, value])

  return dict_list

def pretty_print_code_table():
  print "============ HUFFMAN CODES ============"
  print "CHAR FREQ CODE"
  for tup in reversed(sorted(freq_table, key=lambda x: x[1])):
    print tup[0] + "     " + str(tup[1]) + "   " + encoding[tup[0]]

string = "I have been and always shall be your friend"
print "The string is : " + string

freq_table = compute_freq_table(calculate_frequencies())
tree       = huffman.HuffmanTree(freq_table)
encoding   = tree.traverse()
pretty_print_code_table()

encoded = ""
for char in string:
  encoded += encoding[char]

print "Encoded string: " + encoded

# Open a socket and connect to the server.

print "\nConnecting to server at localhost on port 2345..."
s = socket.socket()
s.connect(('localhost', 2345))

print "Sending Huffman tree and encoded string over socket..."

s.send(json.dumps(freq_table, -1))
# sleep for some time
for x in xrange(1,9999999):
  pass

s.send(encoded)

# ======================== CLIENT OUTPUT ========================
# 
# The string is : I have been and always shall be your friend
# ============ HUFFMAN CODES ============
# CHAR FREQ CODE
#       8   111
# e     5   011
# a     5   100
# n     3   1011
# l     3   1010
# y     2   0100
# r     2   0101
# s     2   11010
# h     2   11011
# d     2   0001
# b     2   0000
# v     1   110010
# w     1   110011
# u     1   11000
# i     1   00101
# o     1   00100
# I     1   00111
# f     1   00110
# Encoded string: 00111111110111001100100111110000011011101111110010110001111100101011001110001001101011111010110111001010101011100000111110100001001100001011110011001010010101110110001
# 
# Connecting to server at localhost on port 2345...
# Sending Huffman tree and encoded string over socket...