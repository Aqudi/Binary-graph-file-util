#include "binary_graph_file_util.hpp"
#include <fstream>
#include <iostream>

#define uint unsigned int

using namespace std;

int main(int argc, char const *argv[]) {
  const char *in_path = argv[1];
  const char *out_path = argv[2];

  int bytes = write_bin_file(in_path, out_path);
  cout << "Write byte: " << bytes << endl;
  return 0;
}
