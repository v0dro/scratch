import igraph
import matplotlib.pyplot as py
import plotly.graph_objs as go

# read file
graph = igraph.read("karate.gml")
graph = graph.as_directed()
# vertices with highest degree
degrees = graph.degree()
a = zip(degrees, graph.vs["id"])
a.sort(key=lambda x: x[0], reverse=True)

# diameter of graph
print graph.get_diameter()

# average path length
print graph.average_path_length()

# global clustering co-efficient
print graph.transitivity_undirected()

# histogram of degree of distribution
py.hist(degrees)
py.ylim([0,15])
py.xlim([0,13])
py.xlabel("Degrees")
py.ylabel("Number of points")
py.show()

# nodes with highest PageRank value
indegree = graph.indegree()
a = zip(indegree, graph.vs["id"])
a.sort(key=lambda x: x[0], reverse=True)
print a

# division of network into modules
comms = graph.community_edge_betweenness(clusters=2)
clusters = comms.as_clustering()
members = clusters.membership
arr = []
for num in members:
    if num == 0:
        arr.append("red")
    elif num == 1:
        arr.append("blue")

graph.vs["color"] = arr

# plotting of graph
layout = graph.layout( "kk")
graph.vs["label"] = graph.vs["id"]
igraph.plot(graph, "graph.png", layout=layout, vertex_size="40", directed=True)

