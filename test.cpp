#include "binary_graph_file_util.hpp"
#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {
  const char *in_path = "test.txt";
  const char *out_path = "test.bin";

  write_bin_file(in_path, out_path);

  vector<vector<int>> adj = read_bin_file(out_path);
  for (unsigned int i = 0; i < adj.size(); i++) {
    cout << i << ": ";
    for (unsigned int j = 0; j < adj[i].size(); j++) {
      cout << adj[i][j] << " ";
    }
    cout << endl;
  }
  return 0;
}
