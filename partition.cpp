#include "binary_graph_file_util.hpp"
#include <fstream>
#include <iostream>

#define uint unsigned int

using namespace std;

int main(int argc, char const *argv[]) {
  const char *in_path = argv[1];
  uint partition_size = atoi(argv[2]);

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

  for (uint i = 0; i < partition_size; i++) {
    cout << "headers[" << i << "] :" << headers[i] << endl;
  }

  vector<vector<int>> adj(partition_size);
  for (uint i = 0; i < partition_size; i++) {
    adj[i] = read_bin_file_partition(in, headers.size(), headers[i]);
    cout << i << ": ";
    for (uint j = 0; j < adj[i].size(); j++) {
      cout << adj[i][j] << " ";
    }
    cout << endl;
  }

  return 0;
}