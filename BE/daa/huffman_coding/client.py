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

def create_huffman_tree(freq_table):
  return huffman.HuffmanTree(freq_table)

def generate_huffman_codes(huffman_tree):
  return huffman_tree.traverse()

def pretty_print_code_table():
  print "============ HUFFMAN CODES ============"
  print "CHAR FREQ CODE"
  for tup in reversed(sorted(freq_table, key=lambda x: x[1])):
    print tup[0] + "     " + str(tup[1]) + "   " + encoding[tup[0]]


string = "I have been and always shall be your friend"
print "The string is : " + string

freq_table = compute_freq_table(calculate_frequencies())
tree       = create_huffman_tree(freq_table)
encoding   = generate_huffman_codes(tree)
pretty_print_code_table()

encoded = ""
for char in string:
  encoded += encoding[char]

print "Encoded string: " + encoded

# Open a socket and connect to the server.

print "\nConnecting to server at localhost on port 2345..."
s = socket.socket()
s.connect(('localhost', 2345))

print "Sending Huffman tree over socket..."

s.send(json.dumps(freq_table, -1))
# s.send(encoded)