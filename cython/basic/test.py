class A():
  def foo(self):
    print type(self)
    pass

  @classmethod
  def bar(cls):
    print type(cls)
    pass

A.bar()
A().foo()
# import hello

# A().foo()