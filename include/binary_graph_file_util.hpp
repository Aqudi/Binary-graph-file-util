#ifndef BINAR_FILE_UTIL_HPPF
#define BINAR_FILE_UTIL_HPP

#include <fstream>
#include <iostream>
#include <unordered_set>
#include <vector>

#define SZ(x) (sizeof(x))
#define ll unsigned long long
#define INT_SIZE SZ(int)
#define LL_SIZE SZ(ll)

/*
중복되지 않은 정점의 갯수를 구하는 함수
 */
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

/*
Binary 파일에서 헤더를 읽는 함수
*/
std::vector<ll> get_headers_from_bin(std::ifstream &in) {
  using namespace std;
  // Read header size
  ll header_array_size;
  in.read(reinterpret_cast<char *>(&header_array_size), LL_SIZE);
  ll header_size = header_array_size * LL_SIZE;

  ll *header_array = new ll[header_array_size];
  in.seekg(LL_SIZE, ios::beg);
  for (unsigned int i = 0; i < header_array_size; i++) {
    in.read(reinterpret_cast<char *>(&header_array[i]), LL_SIZE);
  }

  vector<ll> headers;
  headers.assign(header_array, header_array + header_array_size);
  return headers;
}

/*
Binary로 저장된 그래프를 읽는 함수
*/
std::vector<std::vector<int>> read_bin_file(const char *in_path) {
  using namespace std;
  // Init ifstream
  ifstream in;
  in.open(in_path, ios::binary);
  if (in.fail()) {
    cout << "Error: failed to open input file." << endl;
    return vector<vector<int>>();
  }

  // Read header
  vector<ll> headers = get_headers_from_bin(in);
  int header_size = headers.size();

  int neighbor_size;
  vector<vector<int>> adj(header_size);
  for (int i = 0; i < header_size; i++) {
    in.seekg(headers[i] * LL_SIZE, ios::beg);
    in.read(reinterpret_cast<char *>(&neighbor_size), INT_SIZE);
    int *neighborhood = new int[neighbor_size];
    in.read(reinterpret_cast<char *>(neighborhood), INT_SIZE * neighbor_size);
    adj[i].assign(neighborhood, neighborhood + neighbor_size);
  }
  in.close();
  return adj;
}

/*
Binary로 저장된 그래프의 일부를 읽는 함수
*/
std::vector<int> read_bin_file_partition(std::ifstream &in, int header_size,
                                         int body_pos) {
  using namespace std;

  int neighbor_size;
  in.seekg(header_size * LL_SIZE + body_pos * INT_SIZE, ios::beg);
  in.read(reinterpret_cast<char *>(&neighbor_size), INT_SIZE);
  int *neighborhood = new int[neighbor_size];
  in.read(reinterpret_cast<char *>(neighborhood), INT_SIZE * neighbor_size);

  vector<int> adj_list;
  adj_list.assign(neighborhood, neighborhood + neighbor_size);
  return adj_list;
}

/*
Vector를 binary file에 써주는 함수
*/
void write_vector(std::ofstream &out, ll &header_size, ll &header_pos,
                  ll &body_pos, std::vector<int> &data) {
  using namespace std;

  // Write neighborhood start pos to header
  out.seekp(header_pos * LL_SIZE, ios::beg);
  out.write(reinterpret_cast<const char *>(&body_pos), LL_SIZE);

  // Write neighborhood size
  int data_size = data.size();
  out.seekp(header_size * LL_SIZE + body_pos * INT_SIZE, ios::beg);
  out.write(reinterpret_cast<const char *>(&data_size), INT_SIZE);

  // Write neighborhood ids
  out.write(reinterpret_cast<const char *>(&data[0]), INT_SIZE * data_size);

  // Move to next body pos
  body_pos += 1 + data_size;

  // Move to next header pos
  header_pos += 1;
}

/*
입력받은 그래프 파일을 읽어서 binary 파일로 만들어주는 함수
*/
int write_bin_file(const char *in_path, const char *out_path) {
  using namespace std;

  // Vertex size
  ll header_size = get_vertex_set_size(in_path);

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

  ll header_pos = 1;
  ll body_pos = 0;

  // Write body start pos
  out.seekp(0, ios::beg);
  out.write(reinterpret_cast<const char *>(&header_size), LL_SIZE);

  // Read graph
  int u, v;
  int last_u;
  vector<int> neighborhood_v;

  while (!in.eof() && in >> u >> v) {
    if (u != last_u) {
      if (neighborhood_v.size() != 0) {
        write_vector(out, header_size, header_pos, body_pos, neighborhood_v);
        // cout << last_u << ": " << neighborhood_v.size() << endl;
      }

      // Move to next vertex
      last_u = u;
      neighborhood_v.clear();
    }
    neighborhood_v.push_back(v);
  }

  if (neighborhood_v.size() != 0) {
    write_vector(out, header_size, header_pos, body_pos, neighborhood_v);
    // cout << last_u << ": " << neighborhood_v.size() << endl;
  }

  out.seekp(0, ios::end);
  ll bytes = out.tellp();

  // Close streams
  in.close();
  out.close();

  return bytes;
}

#endif