# Throw an error and check if the subclass is caught.

# Possible approach: redefine the class and make change the
# way the inheritance tree works.
# https://stackoverflow.com/questions/9539052/how-to-dynamically-change-base-class-of-instances-at-runtime

# Defined in C
class LinAlgErrorC():
    pass

class LinAlgErrorPython(Exception):
    pass

LinAlgErrorC = type("LinAlgErrorC", (LinAlgErrorPython, ), dict(LinAlgErrorC.__dict__))

try:
    raise LinAlgErrorC("super error.")
except LinAlgErrorPython as error:
    print("caught this python shit.")
    print(error)
except LinAlgErrorC as error:
    print("caught another C shit")
    print(error)
