class BinaryTree:
    def __init__(self, _parent=None):
        self.data = None
        self.parent = _parent
        self.left = None
        self.right = None

    def inorder(self):
        if self.data:
            if self.left:
                self.left.inorder()
            print("data: ", self.data)
            if self.right:
                self.right.inorder()

    def binsert(self, value):
        if self.data == None:
            self.data = value
        else:
            if value <= self.data:
                if not self.left:
                    self.left = BinaryTree(self)
                self.left.binsert(value)
            elif value > self.data:
                if not self.right:
                    self.right = BinaryTree(self)
                self.right.binsert(value)

    def insert(self, value):
        self.data = value

    def left_subtree_height(self, subtree):
        if subtree:
            left_subtree_height()
        else:
            return 0

    def actual_get_height(self, root):
        if root:
            if not root.data:
                return 0

            left_height = self.actual_get_height(root.left)
            right_height = self.actual_get_height(root.right)

            return max(left_height, right_height) + 1
        else:
            return 0

    def get_height(self):
        return self.actual_get_height(self)

    # https://www.geeksforgeeks.org/how-to-determine-if-a-binary-tree-is-balanced/
    def actual_is_balanced(self, root):
        if root:
            left_height = 0
            right_height = 0
            
            if root and not root.data:
                return True

            if root.left:
                left_height = root.left.get_height()

            if root.right:
                right_height = root.right.get_height()


            if (abs(left_height - right_height) <= 1) and (self.actual_is_balanced(root.left)) \
               and (self.actual_is_balanced(root.right)):
                return True
            else:
                return False
        else:
            return True

    # heights of two subtress never differ by more than 1.
    def is_balanced(self):
        return self.actual_is_balanced(self)



t = BinaryTree()
t.binsert(4)
t.binsert(1)
t.binsert(-3)
t.binsert(10)
t.binsert(11)
t.binsert(9)
t.binsert(23)
t.binsert(-1)
t.binsert(8)
t.binsert(2)

# t.inorder()

# print(t.left.get_height())
print(t.is_balanced())
