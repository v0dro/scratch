#include <bits/stdc++.h>

using namespace std;

static int wrd_count[26]={};
static int rem_count[26]={};
static int sol_count[26]={};
static char sol[10001];

// https://carlosbf.medium.com/reverse-shuffle-merge-solution-cfc99de8d5e8

// Hackerrank Reverse shuffle merge Solution
// Runs in O( n ^ 2)
// ======== SOLUTION =========
string reverseShuffleMerge(string s){
  int n = s.size();
  int j = 0;
  const char* s_chars = s.c_str();

  char l_char;
  int  l_char_indx;

  for(int i = 0; i < n ; i++)
    wrd_count[s[i]-'a']++;

  memcpy(rem_count, wrd_count, 26*(sizeof(int)));

  for(int i = 0; i < 26 ; i++)
    wrd_count[i]/=2;

  for(int i = n-1; i >= 0; i--){
    l_char = s_chars[i];
    l_char_indx = l_char - 'a';

    if(sol_count[l_char_indx ] < wrd_count[l_char_indx]) {
      // backtrack if you can and improve your solution.
      while (j > 0 && (l_char < sol[j-1]) &&
            (sol_count[sol[j-1] - 'a'] - 1 + rem_count[sol[j-1] - 'a'] >= (wrd_count[sol[j-1]-'a']))) {
        sol_count[sol[--j]-'a']--;
      }

      // add character to the solution.
      sol[j] = l_char;
      j++;
      // reduce it from the usable solution.
      rem_count[l_char_indx]--;
      // add to the solution.
      sol_count[l_char_indx]++;
    }
    else{
      // reduce it from the usable solution.
      rem_count[l_char_indx]--;
    }
  }

  sol[j] = '\0';
  string sol_str(sol);

  return sol_str;
}
// ======== HACKERRANK's Template =========

int main()
{
  ofstream fout(getenv("OUTPUT_PATH"));

  string s;
  getline(cin, s);

  string result = reverseShuffleMerge(s);

  fout << result << "\n";

  fout.close();

  return 0;
}
