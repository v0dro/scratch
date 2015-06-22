#include <iostream>
#include <cstdlib>
#include <cstring>
#include <algorithm>
using namespace std;

#define NODES 10

class BST
{ 
  struct node {
    long long int data;
    struct node* pLeft; 
    struct node* pRight;
  } *pHead;
 public:
 
  BST()
  {
    pHead = NULL;
  };

  bool 
  is_empty()
  {
    if (pHead) 
      return false;
      
    return true;
  };

  void 
  add_node(long long int key)
  {
    if (!pHead) {
      pHead = new struct node;
      pHead->data   = key;
      pHead->pLeft   = NULL;
      pHead->pRight  = NULL;
    }
    else {
      node* pTemp = pHead;

      while(pTemp) {
        if (key < pTemp->data) {
          if (pTemp->pLeft) {
            pTemp = pTemp->pLeft;
          }
          else {
            node* pNewNode = new struct node;
            pNewNode->data = key;
            pNewNode->pLeft = NULL;
            pNewNode->pRight = NULL;
            
            pTemp->pLeft   = pNewNode;
            pTemp = NULL;
          }
        }
        else if (key > pTemp->data) {
          if (pTemp->pRight) {
            pTemp = pTemp->pRight;
          }
          else {
            node* pNewNode = new struct node;
            pNewNode->data = key;
            pNewNode->pLeft = NULL;
            pNewNode->pRight = NULL;
            
            pTemp->pRight  = pNewNode;
            pTemp = NULL;
          }
        }
      }
    }
  };

  long long int
  find_number(long long int key)
  {
    node* pTemp;

    if (!is_empty()) {
      pTemp = pHead;

      while (pTemp) {
        if (key == pTemp->data) {
          return key;
        }
        else if (key < pTemp->data) {
          pTemp = pTemp->pLeft;
        }
        else { // key > pTemp->data
          pTemp = pTemp->pRight;
        }
      }
    } else {
      cout << "Tree is empty";
    }

    return 0;
  };
  
  long long int
  max(void)
  { 
    node* pTemp;
    
    if (pHead) {
      pTemp = pHead;
      while (pTemp->pRight) {
        pTemp = pTemp->pRight;
      }
      return pTemp->data;
    }
    else {
      cout << "No tree present.";
    }
    return 0;
  };

  long long int
  min(void)
  {
    node* pTemp;

    if (pHead) {
      pTemp = pHead;
      while (pTemp->pLeft) {
        pTemp = pTemp->pLeft;
      }
      return pTemp->data;
    }
    else {
      cout << "No tree present.\n";
    }
    
    return 0;
  };

  void
  print(char how[12])
  {
    if (strcmp(how, "ascending")) {
      
    }
    else if (strcmp(how, "descending")) {
    }
  }
};

int main()
{
  // create a tree
  BST bst;
  long long int numbers[100];

  if (bst.is_empty() == true) {
    cout << "PASS. The BST is empty.\n";
  }
  // add nodes
  for (int i = 0; i < NODES; ++i) {
    numbers[i] = rand();
    bst.add_node(numbers[i]);
  }

  if (bst.is_empty() == false) {
    cout << "PASS. The BST has stuff.\n";
  }

  // find the max element
  if (*max_element(numbers, numbers+100) == bst.max()) {
    cout << "PASS. Max element matches.\n";
  }

  // find min element
  if (*min_element(numbers, numbers+100) == bst.min()) {
    cout << "PASS. Min element matches.\n";
  }

  // search elements
  long long int temp;
  for (int i = 0; i < NODES/10; i += 5) {
    temp = numbers[i];
    if (temp == bst.find_number(temp)) {
      cout << "PASS. Found number " << temp << endl;
    }
  }

  // return false for elements that are not present
  temp = -999; // because rand() generates only +ve numbers
  if (!bst.find_number(temp)) {
    cout << "PASS. Absent number " << temp << " was not found!\n";
  }

  cout << "==============PRINTING IN ASCENDING==============\n";
  bst.print("ascending");

  cout << "==============PRINTING IN DESCENDING==============\n";
  bst.print("descending");
}
