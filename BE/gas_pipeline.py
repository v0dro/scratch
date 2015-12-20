# Name: Sameer Deshmukh
# Class: BE I
# Roll: 119
# Title: Determing the best gas company for piping towns with gas pipes.

import xml.etree.ElementTree as ET
import copy
import random

COMPANIES = ['A', 'B', 'C']

def find_lightest_edge_such_that_one_end_point_visited_and_other_not_visited(visited, not_visited):
  lightest_node = None
  min_distance = 99999

  for city in visited:
    for other_city in not_visited:
      dist = graph[city][other_city]
      if dist < min_distance and graph[city][other_city] != 99999: 
        min_distance = dist
        lightest_node = other_city

  return lightest_node
    

def calculate_total_min_distance():
  visited = []
  total_distance = 0
  for city_name, distances in spanning_tree.iteritems():
    for connected_city, distance in distances.iteritems():
      if distance:
        total_distance += distance

  return total_distance

def find_max(points):
  maxi = max(points.values())
  for c in COMPANIES:
    if maxi == points[c]:
      return c

def evaluate_companies(data, min_distance):
  points = {}
  for company in COMPANIES:
    points[company] = 0

  for company in COMPANIES:
    ng_prices = []
    ng_prices.append((company, data[company]['natural_gas_price']))

    annual_costs = []
    annual_costs.append((company, data[company]['fixed_annual_costs']))

    lifetime = []
    lifetime.append((company, data[company]['technical_lifetime']))

    if data[company]['pipeline_capacity'] >= min_distance:
      points[company] += 1

  min_ng_price_company = min(ng_prices, key=lambda x: x[1])
  points[min_ng_price_company[0]] += 1

  min_annual_costs = min(annual_costs, key=lambda x: x[1])
  points[min_annual_costs[0]] += 1

  max_life = max(lifetime, key=lambda x: x[1])
  points[max_life[0]] += 1

  return find_max(points)


e = ET.parse('pipeline_data.xml').getroot()
graph = {}
spanning_tree = {}

print "Reading data from XML..."

# Parsing XML and generating the graph as a python dict
for node in e:
  node_name = node[0].text
  graph[node_name] = {}
  spanning_tree[node_name] = {}
  for distances in node[1]:
    graph[node_name][distances.tag] = int(distances.text)
    spanning_tree[node_name][distances.tag] = None

included_nodes = {}
for key in graph.keys():
  included_nodes[key] = False

visited     = []
not_visited = graph.keys()

print "Generating minimal spanning tree..."

current = 'C1'

# Calculate min spanning tree
while 1:
  visited.append(current)

  not_visited = list(set(graph.keys()) - set(visited))
  other_city = find_lightest_edge_such_that_one_end_point_visited_and_other_not_visited(visited, not_visited)

  if len(not_visited) == 0:
    break
  spanning_tree[current][other_city] = graph[current][other_city]
  current = other_city

print "Calculating total distance of spanning tree..."

total_distance = calculate_total_min_distance()
data = {}
for company in COMPANIES:
  print "Generating details about company " + company + " : "
  data[company] = {}

  data[company]['natural_gas_price'] = random.random()*1000
  data[company]['pipeline_capacity'] = random.random()*100
  data[company]['fixed_annual_costs'] = random.random()*10000
  data[company]['technical_lifetime'] = random.random()*1000

  print "Details about company " + company
  print data[company]

winner = evaluate_companies(data, total_distance)

print "\n\nThe contract should go to : " + winner

# OUTPUT
# Reading data from XML...
# Generating minimal spanning tree...
# Calculating total distance of spanning tree...
# Generating details about company A : 
# Details about company A
# {'natural_gas_price': 579.028483420752, 'pipeline_capacity': 73.08864844320901, 'fixed_annual_costs': 7298.100966916219, 'technical_lifetime': 655.0264361136783}
# Generating details about company B : 
# Details about company B
# {'natural_gas_price': 410.4632728154841, 'pipeline_capacity': 48.035156316991866, 'fixed_annual_costs': 4439.93138424177, 'technical_lifetime': 794.3881594253236}
# Generating details about company C : 
# Details about company C
# {'natural_gas_price': 229.2045541813612, 'pipeline_capacity': 88.35347831266586, 'fixed_annual_costs': 1438.2185279233738, 'technical_lifetime': 221.7023500927624}
#
#
# The contract should go to : C