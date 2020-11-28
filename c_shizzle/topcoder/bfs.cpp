#include <iostream>
#include <vector>
#include <queue>
using namespace std;

// from the ICPC book section 4.2.2

// BFS helps in getting 'layers' of the graph. Allows to see how many
// layers away a given node is from the graph.

// The reason why a queue is used in this case is becuase we want
// to keep a track of the nodes in the next layer beyond each layer.
// This needs to be done since in BFS we lose track of the first node
// when we reach the last node. Queue is First In First Out.

// In the STL, the 'front' function will return the first element that
// was pushed, and the pop() function will pop the front part of the queue.

typedef pair<int, int> ii;      // pair of (neighbour, weight)
typedef vector<ii> vii;
typedef vector<int> vi;

#define VISITED 1
#define UNVISITED -1

vi tracker, dist;
queue<int> q;

void bfs(int node, vector<vii>& graph) {
  dist[node] = 0;               // distance from the source node to source node is 0.
  q.push(node);
  tracker[node] = VISITED;

  while(!q.empty()) {
    int curr_node = q.front();
    q.pop();
    cout << "n: " << curr_node << endl;
    for (int i = 0; i < graph[curr_node].size(); ++i) {
      ii n = graph[curr_node][i];

      if (tracker[n.first] == UNVISITED) {
        dist[n.first] = dist[curr_node] + 1;
        tracker[n.first] = VISITED;
        q.push(n.first);
      }
    }
  }

  for (int i = 0; i < 7; ++i) {
    cout << "d[i]: " << dist[i] << endl;
  }
}

void bfs_no_tracker(int node, vector<vii>& graph) {
  dist[node] = 0;               // distance from the source node to source node is 0.
  q.push(node);

  while(!q.empty()) {
    int curr_node = q.front();
    q.pop();
    cout << "n: " << curr_node << endl;
    for (int i = 0; i < graph[curr_node].size(); ++i) {
      ii n = graph[curr_node][i];

      if (dist[n.first] == -1)  {
        dist[n.first] = dist[curr_node] + 1;
        
        q.push(n.first);
      }
    }
  }

  for (int i = 0; i < 7; ++i) {
    cout << "d[i]: " << dist[i] << endl;
  }
}

int main(int argc, char *argv[]) {
  int m = 7;
  int w = 0;
  vector<vii> graph(m);
  tracker.resize(m, UNVISITED);
  dist.resize(m, -1);            // stores distance from the 0th node.

  graph[0].push_back(ii(2, w));
  graph[0].push_back(ii(1, w));

  graph[1].push_back(ii(0, w));
  graph[1].push_back(ii(2, w));
  graph[1].push_back(ii(4, w));

  graph[2].push_back(ii(0, w));
  graph[2].push_back(ii(1, w));
  graph[2].push_back(ii(5, w));

  graph[4].push_back(ii(1, w));

  graph[5].push_back(ii(2, w));
  graph[5].push_back(ii(6, w));

  graph[6].push_back(ii(5, w));

  bfs(0, graph);
  for (int i = 0; i < dist.size(); ++i) {
    dist[i] = -1;
  }
  bfs_no_tracker(0, graph);
}
