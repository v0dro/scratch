def hello(reduction=None, nothing=None):
    print("red:", reduction)
    print("non: ", nothing)

hello("a", "b")
hello(reduction="a", nothing="b")
hello(nothing="a", reduction="b")
