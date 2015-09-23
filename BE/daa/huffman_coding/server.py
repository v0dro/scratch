import socket
import huffman
import json

#create an INET, STREAMing socket
serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#bind the socket to a public host, and a port
print "Server binding on localhost at port 2345..."
serversocket.bind(('localhost', 2345))
#become a server socket
serversocket.listen(5)

while 1:
  (clientsocket, address) = serversocket.accept()
  freq_list               = json.loads(clientsocket.recv(5000))
  encoded_string          = clientsocket.recv(5000)

  tree = huffman.HuffmanTree(freq_list)
  # print encoded
# Receive huffman codes from the client
# Convert to letters and display them.