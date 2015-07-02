#include <iostream>
using namespace std;

class Graph
{ 
  int **adj_matrix, nodes;

 public:
  Graph(int nodes)
  {
    adj_matrix = new int*[nodes];
    for (int j = 0; j < nodes; ++j) {
      adj_matrix[j] = new int[nodes];
    }

    this->nodes = nodes;
  };

  int 
  add_node(int from, int to)
  {
    if (from >= this->nodes or to >= this->nodes) {
      cout << "Cannot create such a graph. From : " << from << ". To: " << to << endl;
    }

    adj_matrix[from][to] = 1;
  };

  void
  print_bfs()
  {
    
  };

  void
  print_dfs()
  {

  };
};

Graph
create_directed_graph(int nodes)
{
  Graph graph(nodes);
  graph.add_node(0,1);
  graph.add_node(0,2);
  graph.add_node(0,3);
  graph.add_node(1,2);
  graph.add_node(3,4);

  return graph;
}

int
test_bfs()
{ 
  Graph graph = create_directed_graph(5);
  cout << "======BFS Search======" << endl;
  graph.print_bfs();
}

int
test_dfs()
{
  Graph graph = create_directed_graph(5);
  cout << "======DFS Search======" << endl;
  graph.print_dfs();
}

int
main()
{
  test_bfs();
  test_dfs();

  return 0;
}