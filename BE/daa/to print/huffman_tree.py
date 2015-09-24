# Name: Sameer Deshmukh
# Class: BE I
# Roll: 405119
# Title: Text compression using a Huffman Tree and decompression after sending over socket.

# File - huffman.py

import copy

class LeafNode(object):
  """docstring for LeafNode"""
  def __init__(self, freq_value, char):
    self.freq_value = copy.copy(freq_value)
    self.char       = copy.copy(char)

class NormalNode(object):
  def __init__(self, freq_value, left, right):
    self.freq_value = copy.copy(freq_value)
    self.left       = copy.deepcopy(left)
    self.right      = copy.deepcopy(right)

class HuffmanTree(object):
  def __init__(self, freq_table):
    self.root = None
    self.freq_table = freq_table
    self.populate_node_list()
    self.actually_create(self.node_list)

  def actually_create(self, nodes):
    if len(nodes) == 1:
      self.root = nodes[0]
      return
    else:
      nodes.sort(key=lambda x: x.freq_value)
      nodes.reverse()

      left  = nodes.pop()
      right = nodes.pop()

      parent_node = NormalNode(left.freq_value + right.freq_value, left, right)
      nodes.append(parent_node)
      self.actually_create(nodes)

  def populate_node_list(self):
    self.node_list = []
    for tup in self.freq_table:
      node = LeafNode(tup[1], tup[0])
      self.node_list.append(node)

  def traverse(self):
    enconding_map = {}
    code = ""
    self.actually_traverse(self.root, enconding_map, code)
    return enconding_map

  def actually_traverse(self, root_node, enconding_map, code):
    if root_node.__class__.__name__ == "LeafNode":
      enconding_map[root_node.char] = code
      return
    else:
      code += "0"
      self.actually_traverse(root_node.left, enconding_map, code)
      code = code[:-1]
      code += "1"
      self.actually_traverse(root_node.right, enconding_map, code)
      code = code[:-1]

# File - server.py
import socket
import huffman
import json

def recreate_message(encoded_string, root):
  message = ""
  while len(encoded_string) > 0:
    traversal = root
    while traversal.__class__.__name__ != "LeafNode":
      if encoded_string[0] == "0":
        traversal = traversal.left
      elif encoded_string[0] == "1":
        traversal = traversal.right
      encoded_string = encoded_string[1:]

    message += traversal.char

  return message

#create an INET, STREAMing socket
serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#bind the socket to a public host, and a port
print "Server binding on localhost at port 2345..."
serversocket.bind(('localhost', 2345))
#become a server socket
serversocket.listen(5)

while 1:
  (clientsocket, address) = serversocket.accept()

  print "Receiving information over socket..."
  freq_list               = json.loads(clientsocket.recv(1234))
  encoded_string          = clientsocket.recv(1234)

  print "Received encoded string : " + encoded_string

  print "Decoding information..."
  tree = huffman.HuffmanTree(freq_list)
  message = recreate_message(encoded_string, tree.root)

  print "Message: " + message

# ======================== SERVER OUTPUT ========================
#
# Server binding on localhost at port 2345...
# Receiving information over socket...
# Received encoded string : 00111111110111001100100111110000011011101111110010110001111100101011001110001001101011111010110111001010101011100000111110100001001100001011110011001010010101110110001
# Decoding information...
# Message: I have been and always shall be your friend

# File - client.py
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