#include "binary_graph_file_util.hpp"
#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {
  const char *in_path = "test.txt";
  const char *out_path = "test.bin";

  int bytes = write_bin_file(in_path, out_path);
  cout << "Write byte: " << bytes << endl;

  // Read partition
  ifstream in;
  in.open(out_path, ios::binary);
  if (in.fail()) {
    cout << "Error: failed to open input file." << endl;
    return 0;
  }
  vector<ll> headers = get_headers_from_bin(in);
  for (unsigned int i = 0; i < 3; i++) {
    cout << "headers[" << i << "] :" << headers[i] << endl;
  }

  vector<vector<int>> adj(3);
  for (unsigned int i = 0; i < 3; i++) {
    adj[i] = read_bin_file_partition(in, headers.size(), headers[i]);
    cout << i << ": ";
    for (unsigned int j = 0; j < adj[i].size(); j++) {
      cout << adj[i][j] << " ";
    }
    cout << endl;
  }

  return 0;
}
