#include "binary_graph_file_util.hpp"
#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {
  const char *in_path = "test.txt";
  const char *out_path = "test.bin";

  cout << write_bin_file(in_path, out_path) << endl << endl;
  cout << read_bin_file(out_path) << endl;

  return 0;
}
