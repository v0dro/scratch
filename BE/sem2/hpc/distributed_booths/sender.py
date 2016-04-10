import socket

nums = {
  2000 : '5',
  2001 : '3'
}

for port,num in nums.iteritems():
  s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
  s.connect((socket.gethostname(), port))
  s.send(num)

s.close()