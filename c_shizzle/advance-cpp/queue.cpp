#include <queue>
#include <iostream>
using namespace std;

int main() {
  queue<int> q;

  q.push(1);
  q.push(2);
  cout << "front: " << q.front()<< " back: " << q.back() <<  " size: " << q.size()<< endl;

  cout << "pop\n";
  q.pop();
  
  cout << "front: " << q.front() <<  endl;
}
