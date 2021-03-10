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

    def preorder(self):
        if self.data:
            print("data: " , self.data)
            if self.left:
                self.left.preorder()
            if self.right:
                self.right.preorder()

    def postorder_search(self, value):
        if self.left:
            self.left.postorder_search(value)
        if self.right:
            self.right.postorder_search(value)

        if self.data == value:
            return True
                
    def insert(self, value):
        if self.data == None:
            self.data = value
        else:
            if value <= self.data:
                if not self.left:
                    self.left = BinaryTree(self)
                self.left.insert(value)
            elif value > self.data:
                if not self.right:
                    self.right = BinaryTree(self)
                self.right.insert(value)

    def first_common_ancestor(self, n1, n2):
        stack = [self]
        parents = { self.data : None }

        while n1 not in parents or n2 not in parents:
            node = stack.pop()
            # keep saving the pointers when traversing the tree
            if node.left:
                parents[node.left.data] = node.data
                stack.append(node.left)

            if node.right:
                parents[node.right.data] = node.data
                stack.append(node.right)

        # ancestors set() for node p
        ancestors = set()

        while n1:
            ancestors.add(n1)
            n1 = parents[n1]

        while n2 not in ancestors:
            n2 = parents[n2]

        return n2

    def is_descendant(self, parent, child):
        is_in_child = self.postorder_search(child)
        
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

ee = t.first_common_ancestor(-3, 11)
print(ee)
