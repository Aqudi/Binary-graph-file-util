#ifndef BINAR_FILE_UTIL_HPPF
#define BINAR_FILE_UTIL_HPP

#include <fstream>
#include <iostream>
#include <vector>

#define SZ(x) (sizeof(x))
#define INT_SIZE SZ(int)

int **read_bin_file(const char *in_path) {
  using namespace std;
  // Init ifstream
  ifstream in;
  in.open(in_path, ios::binary);
  if (in.fail()) {
    cout << "Error: failed to open input file." << endl;
    return 0;
  }

  // Read header size
  int header_size;
  in.read(reinterpret_cast<char *>(&header_size), INT_SIZE);
  cout << "header_size: " << header_size << endl;

  int neighbor_size, neighbor;
  for (int cusor = INT_SIZE; cusor < header_size && !in.eof();
       cusor += INT_SIZE) {
    // Read header
    in.read(reinterpret_cast<char *>(&neighbor_size), INT_SIZE);
    cout << cusor << " " << neighbor_size << endl;

    // in.seekg(header_size + cusor, ios::beg);
    // for (int j = 0; j < neighbor_size; j++) {
    //   in.read(reinterpret_cast<char *>(&neighbor), INT_SIZE);
    //   cout << i << " : " << neighbor << endl;
    // }
  }
}

int write_bin_file(const char *in_path, const char *out_path) {
  using namespace std;
  // Init ifstream
  ifstream in;
  in.open(in_path);
  if (in.fail()) {
    cout << "Error: failed to open input file." << endl;
    return 0;
  }

  // Init ofstream
  ofstream out(out_path, ios::out | ios::binary);
  if (out.fail()) {
    cout << "Error: failed to open output file." << endl;
    return 0;
  }

  // Read graph
  int vertex_size = 0;
  int header_size = 0;
  int neighborhood_size;
  int cusor = 0;

  int u, v;
  int last_u;
  vector<int> neighborhood_v;

  while (!in.eof() && in >> u >> v) {
    if (u != last_u) {
      if (neighborhood_v.size() != 0) {
        header_size = vertex_size * INT_SIZE;
        neighborhood_size = neighborhood_v.size();

        cout << last_u << ": " << neighborhood_v.size() << endl;

        // Write current vertex's neighborhood size
        out.seekp(header_size, ios::beg);
        out.write(reinterpret_cast<const char *>(&neighborhood_size), INT_SIZE);

        // Write current vertex's neighborhood
        /*out.seekp(0, ios::end);
        out.write(reinterpret_cast<const char *>(&neighborhood_v[0]),
                  INT_SIZE * neighborhood_v.size());*/

        // Set next neighborhood start point
        // cusor += neighborhood_v.size();
      }

      // Move to next vertex
      vertex_size++;
      last_u = u;
      neighborhood_v.clear();
    }
    neighborhood_v.push_back(v);
    cout << u << " " << v << endl;
  }

  if (neighborhood_v.size() != 0) {
    header_size = vertex_size * INT_SIZE;
    neighborhood_size = neighborhood_v.size();

    cout << last_u << ": " << neighborhood_v.size() << endl;

    // Write current vertex's neighborhood size
    out.seekp(header_size + cusor, ios::beg);
    out.write(reinterpret_cast<const char *>(&neighborhood_size), INT_SIZE);

    // Set next neighborhood start point
    cusor += neighborhood_v.size();
  }

  // Write header size
  header_size += INT_SIZE;
  out.seekp(0, ios::beg);
  out.write(reinterpret_cast<const char *>(&header_size), INT_SIZE);

  // Close streams
  in.close();
  out.close();
}

#endif