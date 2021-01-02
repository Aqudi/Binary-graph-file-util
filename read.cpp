#include "binary_graph_file_util.hpp"
#include <fstream>
#include <iostream>

#define uint unsigned int

using namespace std;

int main(int argc, char const *argv[]) {
  const char *in_path = argv[1];

  // Read whole file
  vector<vector<int>> adj = read_bin_file(in_path);
  for (uint i = 0; i < adj.size() && i < 10; i++) {
    cout << i << ": ";
    for (uint j = 0; j < adj[i].size(); j++) {
      cout << adj[i][j] << " ";
    }
    cout << endl;
  }

  for (uint i = adj.size() - 11; i < adj.size(); i++) {
    cout << i << ": ";
    for (uint j = 0; j < adj[i].size(); j++) {
      cout << adj[i][j] << " ";
    }
    cout << endl;
  }

  return 0;
}
