class BinaryTree:
    def __init__(self):
        self.data = None
        self.left = None
        self.right = None

    def inorder(self):
        if self.data:
            if self.left:
                self.left.inorder()
            print("data: ", self.data)
            if self.right:
                self.right.inorder()

    def preorder(self):
        if self.data:
            print("data: " , self.data)
            if self.left:
                self.left.preorder()
            if self.right:
                self.right.preorder()
                
    def insert(self, value):
        if self.data == None:
            self.data = value
        else:
            if value <= self.data:
                if not self.left:
                    self.left = BinaryTree()
                self.left.insert(value)
            elif value > self.data:
                if not self.right:
                    self.right = BinaryTree()
                self.right.insert(value)

    def first_common_ancestor(self, n1, n2):
        pass

    def is_descendant(self, parent, child):
        pass
        
t = BinaryTree()
t.insert(4)
t.insert(1)
t.insert(-3)
t.insert(10)
t.insert(11)
t.insert(9)

t.inorder()
print("\n")
t.preorder()
