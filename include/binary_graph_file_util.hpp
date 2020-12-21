#ifndef BINAR_FILE_UTIL_HPPF
#define BINAR_FILE_UTIL_HPP

#include <fstream>
#include <iostream>
#include <unordered_set>
#include <vector>

#define SZ(x) (sizeof(x))
#define ll long long
#define INT_SIZE SZ(int)
#define LL_SIZE SZ(ll)

ll get_vertex_set_size(const char *in_path) {
  using namespace std;
  // Init ifstream
  ifstream in;
  in.open(in_path, ios::binary);
  if (in.fail()) {
    cout << "Error: failed to open input file." << endl;
    return 0;
  }

  // Vertex set
  unordered_set<int> vertexs;
  int u, v;
  while (!in.eof() && in >> u >> v) {
    vertexs.insert(u);
  }
  return vertexs.size();
}

std::vector<std::vector<int>> read_bin_file(const char *in_path) {
  using namespace std;
  // Init ifstream
  ifstream in;
  in.open(in_path, ios::binary);
  if (in.fail()) {
    cout << "Error: failed to open input file." << endl;
    return vector<vector<int>>();
  }

  // Read header size
  ll header_size;
  in.read(reinterpret_cast<char *>(&header_size), LL_SIZE);

  int header_array_size = (header_size - LL_SIZE) / INT_SIZE;
  int *headers = new int[header_array_size];
  in.seekg(LL_SIZE, ios::beg);
  for (int i = 0; i < header_array_size; i++) {
    in.read(reinterpret_cast<char *>(&headers[i]), INT_SIZE);
  }

  int neighbor_size;
  vector<vector<int>> adj(header_array_size);
  for (int i = 0; i < header_array_size; i++) {
    in.seekg(headers[i], ios::beg);
    in.read(reinterpret_cast<char *>(&neighbor_size), INT_SIZE);
    int *neighborhood = new int[neighbor_size];
    in.read(reinterpret_cast<char *>(neighborhood), INT_SIZE * neighbor_size);
    adj[i].assign(neighborhood, neighborhood + neighbor_size);
  }
  return adj;
}

int write_vector(std::ofstream &out, ll &header_pos, ll &body_pos,
                 std::vector<int> &data) {
  using namespace std;

  // Write neighborhood start pos
  out.seekp(header_pos, ios::beg);
  out.write(reinterpret_cast<const char *>(&body_pos), INT_SIZE);

  // Write neighborhood size
  int data_size = data.size();
  out.seekp(body_pos, ios::beg);
  out.write(reinterpret_cast<const char *>(&data_size), INT_SIZE);

  // Write neighborhood ids
  out.write(reinterpret_cast<const char *>(&data[0]), INT_SIZE * data_size);

  // Move to next body pos
  body_pos += INT_SIZE + data_size * INT_SIZE;

  // Move to next header pos
  header_pos += INT_SIZE;
}

int write_bin_file(const char *in_path, const char *out_path) {
  using namespace std;

  // Vertex size
  ll vertex_size = get_vertex_set_size(in_path);

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

  ll header_pos = LL_SIZE;
  ll body_start_pos = LL_SIZE + vertex_size * INT_SIZE;
  ll body_pos = body_start_pos;

  // Write body start pos
  out.seekp(0, ios::beg);
  out.write(reinterpret_cast<const char *>(&body_pos), LL_SIZE);

  // Read graph
  int u, v;
  int last_u;
  vector<int> neighborhood_v;

  while (!in.eof() && in >> u >> v) {
    if (u != last_u) {
      if (neighborhood_v.size() != 0) {
        write_vector(out, header_pos, body_pos, neighborhood_v);
        cout << last_u << ": " << neighborhood_v.size() << endl;
      }

      // Move to next vertex
      last_u = u;
      neighborhood_v.clear();
    }
    neighborhood_v.push_back(v);
  }

  if (neighborhood_v.size() != 0) {
    write_vector(out, header_pos, body_pos, neighborhood_v);
    cout << last_u << ": " << neighborhood_v.size() << endl;
  }

  // Close streams
  in.close();
  out.close();
}

#endif