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
