import igraph

# read file
graph = igraph.read("karate.gml")

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

# plotting of graph
layout = graph.layout("kk")
graph.vs["label"] = graph.vs["id"]
#igraph.plot(graph, layout=layout, vertex_size="40")


