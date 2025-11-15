#pragma once

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

struct Stats {
  uint32_t input_bytes;
  uint32_t input_bits;
  uint32_t alphabet_size;
  uint32_t header_bits;
  uint32_t bitstream_bits;
  uint32_t padding_bits;
  uint32_t total_output_bits;
  double avg_code_length;

  void print() {
    std::cout << "\"input_bytes\": " << input_bytes << std::endl;
    std::cout << "\"input_bits\": " << input_bits << std::endl;
    std::cout << "\"alphabet_size\": " << alphabet_size << std::endl;
    std::cout << "\"header_bits\": " << header_bits << std::endl;
    std::cout << "\"bitstream_bits\": " << bitstream_bits << std::endl;
    std::cout << "\"padding_bits\": " << padding_bits << std::endl;
    std::cout << "\"total_output_bits\": " << total_output_bits << std::endl;
    std::cout << "\"avg_code_length\": " << avg_code_length << std::endl;
  }

  void write(const std::filesystem::path &file_path) {
    std::string path = std::string("output/") + file_path.c_str();
    std::ofstream file(path);
    if (!file.is_open()) {
      std::cerr << "Error: Could not open file " << path << " for writing."
                << std::endl;
      return;
    }
    file << "{\n";
    file << "\t\"input_bytes\": " << input_bytes << ",\n";
    file << "\t\"input_bits\": " << input_bits << ",\n";
    file << "\t\"alphabet_size\": " << alphabet_size << ",\n";
    file << "\t\"header_bits\": " << header_bits << ",\n";
    file << "\t\"bitstream_bits\": " << bitstream_bits << ",\n";
    file << "\t\"padding_bits\": " << padding_bits << ",\n";
    file << "\t\"total_output_bits\": " << total_output_bits << ",\n";
    file << "\t\"avg_code_length\": " << avg_code_length << "\n";
    file << "}";

    file.close();
  }
};

namespace Helper {
// reads a txt file into a string
std::string read_into_str(const std::filesystem::path &file_path) {
  std::string path = std::string("data/") + file_path.c_str();
  std::ifstream file(path);

  if (!file.is_open()) {
    throw std::runtime_error("Could not open file: " + path);
  }

  std::stringstream buffer;
  buffer << file.rdbuf();

  return buffer.str();
}

// writes bits into a file
void write_bits_to_file(const std::string &bit_string,
                        const std::filesystem::path &file_path) {
  // writes as binary file
  std::string path = std::string("output/") + file_path.c_str();
  std::ofstream file(path, std::ios::binary);
  if (!file.is_open()) {
    std::cerr << "Error: Could not open file " << path << " for writing."
              << std::endl;
    return;
  }

  unsigned char byte_buffer = 0; // 8-bit
  int bit_count = 0;             // How many bits are currently in the buffer

  // Loop over each character in the input string
  for (char c : bit_string) {
    if (c != '0' && c != '1') {
      std::cerr << "Warning: Skipping invalid character '" << c << "'"
                << std::endl;
      continue;
    }

    // 1. Shift the buffer to the left to make room for the new bit
    byte_buffer <<= 1;

    // 2. If the character is '1', set the new last bit to 1
    if (c == '1') {
      byte_buffer |= 1;
    }

    // 3. Increment the bit counter
    bit_count++;

    // 4. If the buffer is full (we've processed 8 bits), write it to the file
    if (bit_count == 8) {
      file.put(byte_buffer);

      // Reset the buffer and counter for the next byte
      byte_buffer = 0;
      bit_count = 0;
    }
  }

  /* HANDLES LEFTOVER BITS */
  if (bit_count > 0) {
    byte_buffer <<= (8 - bit_count);

    file.put(byte_buffer);
    std::cout << "Wrote final partial byte (padded with 0s)." << std::endl;
  }

  file.close();
  std::cout << "Successfully wrote bits to " << path << std::endl;
}

// Creates a frequency table from a given string
std::unordered_map<char, uint32_t>
create_frequency_table(const std::string_view &string) {
  std::unordered_map<char, uint32_t> freq_table{0};
  for (const char &ch : string) {
    freq_table[ch]++;
  }
  return freq_table;
}

// Just some stuff for printing containers
template <class T> void print_array(const T &arr) {
  for (const auto &element : arr) {
    std::cout << element;
  }
  std::cout << std::endl;
}

template <class T> void print_map(const T &map) {
  for (const auto &[k, v] : map) {
    switch (k) {
    case '\n':
      std::cout << "\\n";
      break;
    case '\t':
      std::cout << "\\t";
      break;
    case '\r':
      std::cout << "\\r";
      break;
    case '\0':
      std::cout << "\\0";
      break;
    default:
      std::cout << k;
      break;
    }

    std::cout << ": " << v << std::endl;
  }
  std::cout << std::endl;
}

double
get_avg_codelength(const std::unordered_map<char, std::string> &codemap) {
  double avg_code_length = 0;
  for (const auto &[_, v] : codemap) {
    avg_code_length += v.length();
  }
  avg_code_length /= codemap.size();
  return avg_code_length;
}

} // namespace Helper
