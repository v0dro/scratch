import copy

class LeafNode(object):
  """docstring for LeafNode"""
  def __init__(self, freq_value, char):
    self.freq_value = copy.copy(freq_value)
    self.char       = copy.copy(char)

class NormalNode(object):
  def __init__(self, freq_value, left, right):
    self.freq_value = copy.copy(freq_value)
    self.left       = copy.deepcopy(left)
    self.right      = copy.deepcopy(right)

class HuffmanTree(object):
  def __init__(self, freq_table):
    self.root = None
    self.freq_table = freq_table
    self.populate_node_list()
    self.actually_create(self.node_list)

  def actually_create(self, nodes):
    if len(nodes) == 1:
      self.root = nodes[0]
      return
    else:
      nodes.sort(key=lambda x: x.freq_value)
      nodes.reverse()

      left  = nodes.pop()
      right = nodes.pop()

      parent_node = NormalNode(left.freq_value + right.freq_value, left, right)
      nodes.append(parent_node)
      self.actually_create(nodes)

  def populate_node_list(self):
    self.node_list = []
    for tup in self.freq_table:
      node = LeafNode(tup[1], tup[0])
      self.node_list.append(node)

  def traverse(self):
    enconding_map = {}
    code = ""
    self.actually_traverse(self.root, enconding_map, code)
    return enconding_map

  def actually_traverse(self, root_node, enconding_map, code):
    if root_node.__class__.__name__ == "LeafNode":
      enconding_map[root_node.char] = code
      return
    else:
      code += "0"
      self.actually_traverse(root_node.left, enconding_map, code)
      code = code[:-1]
      code += "1"
      self.actually_traverse(root_node.right, enconding_map, code)
      code = code[:-1]