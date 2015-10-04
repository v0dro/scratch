# Name: Sameer Deshmukh
# Class: BE I
# Roll: 405119
# Title: Finding the minimmum cost of setting up a gas connection between cities
# using Prim's algorithm.

# The graph looks something like this:
#    4
# A---- B
#  \    |
# 1 \   | 2
#    \  |
#     \ | 
# C---- D
#    3

# The minimmum spanning tree eventually created looks like this:
#    
# A     B
#  \    |
# 1 \   | 2
#    \  |
#     \ | 
# C---- D
#    3

import xml.etree.ElementTree as ET
import copy

def find_minimmum_value_unincluded_node(node_distances, included_nodes):
  min_distance = 99999
  selected_node = None

  for node, distance in node_distances.iteritems():
    if included_nodes[node] == False and node_distances[node] < min_distance:
      min_distance = node_distances[node]
      selected_node = node

  return selected_node

e = ET.parse('data.xml').getroot()
graph = {}
spanning_tree = {}

# Parsing XML and generating the graph as a python dict
for node in e:
  node_name = node[0].text
  graph[node_name] = {}
  spanning_tree[node_name] = {}
  for distances in node[1]:
    graph[node_name][distances.tag] = int(distances.text)
    spanning_tree[node_name][distances.tag] = None

print "-------- GRAPH --------"
print graph

included_nodes = {}
for key in graph.keys():
  included_nodes[key] = False

for node in graph.keys():
  min_key_value = find_minimmum_value_unincluded_node(graph[node], included_nodes)

  if min_key_value == None:
    break

  spanning_tree[node][min_key_value] = graph[node][min_key_value]

print "-------- SPANNING TREE --------"
print spanning_tree

# ################# OUTPUT #################
#
# -------- GRAPH --------
# {'A': {'A': 99999, 'C': 99999, 'B': 4, 'D': 1}, 
#  'C': {'A': 99999, 'C': 99999, 'B': 99999, 'D': 3}, 
#  'B': {'A': 4, 'C': 99999, 'B': 99999, 'D': 2}, 
#  'D': {'A': 1, 'C': 3, 'B': 2, 'D': 99999}}
# -------- SPANNING TREE --------
# {'A': {'A': None, 'C': None, 'B': None, 'D': 1}, 
#  'C': {'A': None, 'C': None, 'B': None, 'D': 3}, 
#  'B': {'A': None, 'C': None, 'B': None, 'D': 2}, 
#  'D': {'A': 1, 'C': None, 'B': None, 'D': None}}