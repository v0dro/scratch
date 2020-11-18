#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

// https://www.geeksforgeeks.org/longest-monotonically-increasing-subsequence-size-n-log-n/

int find_index(vector<int>& tail, int begin, int end, int pivot) {
  // edge: where begin=0 and size of arr is 1 so end is 0.
  int index;

  while (end - begin > 1) {
    // subtracting end-begin gives the effective length of the array.
    // adding begin to half of that length will give the offset in the mid point
    // after the current begin.
    index = begin + (end - begin) / 2;
        
    if (tail[index] >= pivot) {
      end = index;
    }
    else {
      begin = index;
    }
  }

  return end;
}

void LIS_nlogn(vector<int>& D) {
  vector<int> tail(D.size(), 0);
  int length = 1;

  if (D.size() == 0) {
    cout << "len: " << 0;
    return;
  }

  tail[0] = D[0];
  for (int i = 1; i < D.size(); ++i) {
    // 0th position of tail always stores the smallest tail element.
    // this means that the smallest element that was found so far should be kept here.
    if (tail[0] > D[i]) {       
      tail[0] = D[i];
    }
    else if (tail[length-1] < D[i]) { // length 'th position of tail always stores the largest tail element.
      // 
      tail[length++] = D[i];
    }
    else {
      // if the element under consideration is not greater than the last element in
      // the LIS and not smaller than the smallest element in the smallest increasing subsequence,
      // find an appropriate place within the tail array at which you can insert the
      // element that fits between the currently read last elements of the array.
      int index = find_index(tail, -1, length-1, D[i]);
      tail[index] = D[i];
      // cout << "D[i] " << D[i] << " index: " << index << endl;
    }
  }

  cout << "tail :: ";
  for (int j = 0; j < tail.size(); ++j) {
    std::cout << tail[j] << " ";
  }
  cout << endl;

  cout << "len : " << length << endl; 
       
}

void LIS(vector<int>& D) {
  vector<vector<int> > L(D.size());

  L[0].push_back(D[0]);
  for (int i = 1; i < D.size(); ++i) {
    for (int j = 0; j < i; ++j) {
      if ((D[j] < D[i]) && (L[i].size() < L[j].size() + 1)) {
        // replace with a new subsequence in the currently longest place.
        // This ensures that the subsequence at the ith position is always
        // the currently longest subsequence.
        L[i] = L[j];
      }
    }
    
    L[i].push_back(D[i]);
  }

}

int main(int argc, char *argv[])
{
  int a[] = {6, 2, 4, 3, 7, 4, 5};
  vector<int> arr(a, a + sizeof(a)/sizeof(a[0]));


  int a1[] = {22, 33, 44, 55, 66, 77, 88, 99};
  vector<int> arr1(a1, a1 + sizeof(a1)/sizeof(a1[0]));
  // LIS(arr);

  fstream in;
  in.open("lis.txt", ios::in);

  int num;
  in >> num;
  cout << "n: " << num << endl;
  vector<int> arr2;

  while (!in.eof()) {
    int t;
    in >> t;
    arr2.push_back(t);
  }

  LIS_nlogn(arr2);

  cout << "i: " << find_index(arr1, 0, arr1.size()-1, 69);

  in.close();
  
  return 0;
}
