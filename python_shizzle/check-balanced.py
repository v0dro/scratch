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

    # heights of two subtress never differ by more than 1.
    def is_balanced(self):
        left_height = left_subtree_height(self.left)
        right_height = right_subtree_height(self.right)

        if right_height - left_height > 1:
            return False
        else:
            return True

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
        height = 0
        return self.actual_get_height(self)

t = BinaryTree()
t.binsert(4)
t.binsert(1)
t.binsert(-3)
t.binsert(10)
t.binsert(11)
t.binsert(9)
t.binsert(23)
t.binsert(-1)

# t.inorder()

print(t.get_height())
