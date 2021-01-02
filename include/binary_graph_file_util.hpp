#ifndef BINAR_FILE_UTIL_HPPF
#define BINAR_FILE_UTIL_HPP

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <time.h>
#include <unordered_set>
#include <vector>

#define SZ(x) (sizeof(x))
#define uint unsigned int
#define ull unsigned long long
#define INT_SIZE SZ(uint)
// #define DEBUG true

/*
전부 숫자인지 확인
*/
bool is_all_number(std::string &s) {
  using namespace std;
  for (uint i = 0; i < s.size(); i++) {
    if ((s[i] >= '0' && s[i] <= '9') == false) {
      return false;
    }
  }
  return true;
}

/*
중복되지 않은 정점의 갯수를 구하는 함수
 */
void calculate_vertex_degrees(std::ifstream &in, std::map<int, int> &degrees) {
  using namespace std;

  string str, temp;
  stringstream ss;

  int u;
  vector<string> tokens;

  while (!in.eof()) {
    getline(in, str);
    ss.clear();
    ss << str;

    tokens.clear();
    while (ss >> temp) {
      tokens.push_back(temp);
    }

    if (tokens.size() >= 2) {
      bool valid = true;
      for (string it : tokens) {
        valid &= is_all_number(it);
      }
      if (valid) {
        ss.clear();
        ss << tokens[0];
        ss >> u;
        degrees[u] += 1;
      }
    }
  }

  in.clear();
  in.seekg(0, ios::beg);
}

/*
Binary 파일에서 헤더를 읽는 함수
*/
void get_headers_from_bin(std::ifstream &in, uint &header_size,
                          std::vector<uint> &headers) {
  using namespace std;

  // Read header size
  in.read(reinterpret_cast<char *>(&header_size), INT_SIZE);

  // Read headers
  uint *header_array = new uint[header_size];
  in.read(reinterpret_cast<char *>(&header_array[0]), INT_SIZE * header_size);
  headers.assign(header_array, header_array + header_size);

  // Add header_size meta field
  header_size += 1;
}

/*
Binary로 저장된 그래프를 읽는 함수
*/
std::vector<std::vector<int>> read_bin_file(const char *in_path) {
  using namespace std;
  double start_clock = clock();

  // Init ifstream
  ifstream in;
  in.open(in_path, ios::binary);
  if (in.fail()) {
    cout << "Error: failed to open input file." << endl;
    return vector<vector<int>>();
  }

  // Read header
  uint header_size;
  vector<unsigned int> headers;
  get_headers_from_bin(in, header_size, headers);

  int neighbor_size;
  in.seekg(header_size * INT_SIZE, ios::beg);

  // Remove total header size
  header_size -= 1;

  vector<vector<int>> adj(header_size);
  for (uint i = 0; i < header_size; i++) {
    in.read(reinterpret_cast<char *>(&neighbor_size), INT_SIZE);
    int *neighborhood = new int[neighbor_size];
    in.read(reinterpret_cast<char *>(neighborhood), INT_SIZE * neighbor_size);
    adj[i].assign(neighborhood, neighborhood + neighbor_size);
  }
  in.close();

  double end_clock = clock();
  double elapsed_time = (end_clock - start_clock) / CLOCKS_PER_SEC;
  cout << "input: " << in_path << ", elapsed time: " << elapsed_time << " secs."
       << endl;
  return adj;
}

/*
Binary로 저장된 그래프의 일부를 읽는 함수
*/
std::vector<std::vector<int>> read_bin_file_partition(std::ifstream &in, int header_size,
                                         int start, uint num_partition) {
  using namespace std;

  header_size += 1;
  in.seekg(header_size * INT_SIZE + start * INT_SIZE, ios::beg);

  vector<vector<int>> adj(num_partition);
  int neighbor_size;
  for(uint i=0; i<num_partition; i++) {
    in.read(reinterpret_cast<char *>(&neighbor_size), INT_SIZE);
    int *neighborhood = new int[neighbor_size];
    in.read(reinterpret_cast<char *>(neighborhood), INT_SIZE * neighbor_size);
    adj[i].assign(neighborhood, neighborhood + neighbor_size);
  }
  return adj;
}

/*
Vector를 binary file에 써주는 함수
*/
void write_vector(std::ofstream &out, std::vector<int> &data) {
  using namespace std;

  // Write neighborhood size
  int data_size = data.size();
  out.write(reinterpret_cast<const char *>(&data_size), INT_SIZE);

  // Write neighborhood ids
  out.write(reinterpret_cast<const char *>(&data[0]), INT_SIZE * data_size);
}

/*
입력받은 그래프 파일을 읽어서 binary 파일로 만들어주는 함수
*/
int write_bin_file(const char *in_path, const char *out_path) {
  using namespace std;

  double start_clock = clock();
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

  map<int, int> degrees;
  calculate_vertex_degrees(in, degrees);

  // Write header
  uint header_size = degrees.size();
  out.seekp(0, ios::beg);
  out.write(reinterpret_cast<const char *>(&header_size), INT_SIZE);
  uint body_pos = 0;
  for (auto it : degrees) {
    out.write(reinterpret_cast<const char *>(&body_pos), INT_SIZE);
    body_pos += 1 + it.second;
  }

  // Read graph
  string str, temp;
  stringstream ss;

  vector<string> tokens;

  int u, v;
  int last_u;
  vector<int> neighborhood_v;

  while (!in.eof()) {
    getline(in, str);
    ss.clear();
    ss << str;

    tokens.clear();
    while (ss >> temp) {
      tokens.push_back(temp);
    }

    if (tokens.size() >= 2) {
      bool valid = true;
      for (string it : tokens) {
        valid &= is_all_number(it);
      }
      if (valid) {
        ss.clear();
        ss << tokens[0];
        ss >> u;
        ss.clear();
        ss << tokens[1];
        ss >> v;

        if (u != last_u) {
          if (neighborhood_v.size() != 0) {
            write_vector(out, neighborhood_v);
          }
          // Move to next vertex
          last_u = u;
          neighborhood_v.clear();
        }
        neighborhood_v.push_back(v);
      }
    }
  }

  if (neighborhood_v.size() != 0) {
    write_vector(out, neighborhood_v);
    neighborhood_v.clear();
  }

  out.seekp(0, ios::end);
  ull bytes = out.tellp();

  // Close streams
  in.close();
  out.close();

  double end_clock = clock();
  double elapsed_time = (end_clock - start_clock) / CLOCKS_PER_SEC;
  cout << "input: " << in_path << ", elapsed time: " << elapsed_time << " secs."
       << endl;
  return bytes;
}

#endif