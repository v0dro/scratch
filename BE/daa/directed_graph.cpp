#include <iostream>
#include <stack>
#include <queue>
using namespace std;

class Graph
{
	int **adj_matrix;
	int nodes;

	bool
	node_not_discovered(int* discovered, int node)
	{
	  if (!discovered[node]) {
	    return true;
	  }

	  return false;
	}

	void
	init_array(int* array, int nds, int num)
	{
		for (int i = 0; i < nds; ++i) {
			array[i] = num;
		}
	}

	public:

	Graph(int nodes)
	{
	  adj_matrix = new int*[nodes];
	  for (int i = 0; i < nodes; ++i) {
	    adj_matrix[i] = new int[nodes];
	  }

	  this->nodes = nodes;
	}

	void
	add_node(int from, int to)
	{ 
	  if (from >= this->nodes or to >= this->nodes) {
	    cout << "Wrong nodes.\n";
	  }
	  adj_matrix[from][to] = 1;
	}

	void
	print_dfs()
	{
	  stack<int> stack;
	  int *discovered = new int[this->nodes];
	  int curr;

	  init_array(discovered, this->nodes, 0);
	  stack.push(0);

	  while (!stack.empty()) {
	  	curr = stack.top();
	  	stack.pop();

	  	if (node_not_discovered(discovered,curr)) {
	  		discovered[curr] = 1;
	  		cout << curr << "->";
	  		for (int v = 0; v < this->nodes; ++v) {
	  			if (adj_matrix[curr][v]) {
	  				stack.push(v);
	  			}
	  		}
	  	}
	  }

	  cout << "\n";
	}

	void
	print_bfs()
	{
	  queue<int> queue;
	  int *discovered = new int[this->nodes];
	  int curr;

	  init_array(discovered, this->nodes, 0);
	  queue.push(0);
	  discovered[0] = 1;

	  while (!queue.empty()) {
	    curr = queue.front();
	    cout << curr << "->";
	    queue.pop();

	    for (int v = 0; v < this->nodes; ++v) {
	      if (adj_matrix[curr][v] and node_not_discovered(discovered,v)) {
	        queue.push(v);
	        discovered[v] = 1;
	      }
	    }
	  }

	  cout << "\n";
	}

	void
	print_mat()
	{
	  for (int i = 0;i < this->nodes;++i) {
	    for (int j = 0;j < this->nodes;++j) {
	      cout << adj_matrix[i][j] << " ";
	    }
	    cout << "\n";
	  }
	}
};

int
main()
{
  Graph graph(9);
  graph.add_node(0,1);
  graph.add_node(0,2);
  graph.add_node(0,3);
  graph.add_node(2,4);
  graph.add_node(3,4);
  graph.add_node(4,3);
  graph.add_node(4,5);
  graph.add_node(5,6);
  graph.add_node(6,7);
  graph.add_node(6,8);
   graph.add_node(7,6);
   graph.add_node(7,8);
  graph.print_mat();
  
  cout << "DEPTH FIRST SEARCH\n";
  graph.print_dfs();

  cout << "BREADTH FIRST SEARCH\n";
  graph.print_bfs();

  return 0;
}
