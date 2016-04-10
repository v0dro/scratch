import hashlib

class RandGen():
  """docstring for RandGen"""
  def __init__(self, seed):
    self.seed = seed
    self.count = 1

  def generate(self):
    g = self.fibo()
    rand =  g * self.seed
    self.count += 1
    return int(hashlib.sha1(str(rand)).hexdigest(), 16)

  def fibo(self):
    if self.count == 1:
      self.prev = 1
      self.current = 1
    else:
      temp = self.prev
      self.prev = self.current
      self.current += temp

    return self.current

if __name__ == '__main__':
  g = RandGen(3)
  print g.generate()
  print g.generate()
  print g.generate()
  print g.generate()
  print g.generate()
  print g.generate()