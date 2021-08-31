""" Node is defined as
class node:
    def __init__(self, data):
        self.data = data
        self.left = None
        self.right = None
"""

# Wrong solution

def check_less(root, value):
    if root is not None:
        check_less(root.left, value)
        if root.data > value:
            return False
        check_less(root.right, value)
    else:
        return True

def check_more(root, value):
    if root is not None:
        check_more(root.left, value)
        if root.data < value:
            return False
        check_more(root.right, value)
    else:
        return True

def checkBST(root):
    left_ok = check_less(root.left, root.data)
    right_ok = check_more(root.right, root.data)

    if left_ok and right_ok:
        return True
    else:
        return False

# Working solution

def actual_check_bst(root, left, right):
    if root is None:            # 1
        return True

    if left and root.data <= left.data: # 2
        return False

    if right and root.data >= right.data: # 3
        return False

    return actual_check_bst(root.left, left, root) and actual_check_bst(root.right, root, right)

def checkBST(root):
    return actual_check_bst(root, None, None)
