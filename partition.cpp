#include "binary_graph_file_util.hpp"
#include <fstream>
#include <iostream>

#define DEBUG true
#define uint unsigned int

using namespace std;

int main(int argc, char const *argv[]) {
  const char *in_path = argv[1];
  uint num_partition = atoi(argv[2]);

  // Read partition
  ifstream in;
  in.open(in_path, ios::binary);
  if (in.fail()) {
    cout << "Error: failed to open input file." << endl;
    return 0;
  }

  uint header_size;
  vector<unsigned int> headers;
  get_headers_from_bin(in, header_size, headers);

  vector<vector<int>> adj;
  read_bin_file_partition(in, adj, header_size, headers[0], num_partition);

  for (uint i = 0; i < adj.size(); i++) {
    cout << i << ": ";
    for (uint j = 0, idx = 0; j < adj[i].size() && idx++ < 10; j++) {
      cout << adj[i][j] << " ";
    }
    cout << endl;
  }

  adj.clear();
  read_bin_file_partition(in, adj, header_size,
                          headers[headers.size() - num_partition],
                          num_partition);

  for (uint i = 0; i < adj.size(); i++) {
    cout << headers.size() - num_partition + i << ": ";
    for (uint j = 0, idx = 0; j < adj[i].size() && idx++ < 10; j++) {
      cout << adj[i][j] << " ";
    }
    cout << endl;
  }
  
  in.close();
  return 0;
}
