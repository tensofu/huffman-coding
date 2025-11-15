/* Huffman Encoding Program */

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <functional>
#include <iostream>
#include <queue>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "utils/helper.h"
#include "utils/huffman.h"
#include "utils/parser.h"

int main(int argc, char *argv[]) {
  if (argc <= 1) {
    printf("insufficient arguments.use the following commandline structure "
           "`./huffencode -i <input.txt> -o <output.huff> --header "
           "<none|freq32> --stats <stats.json>`\n");
    return 1;
  }

  /* ARGUMENT PARSER THINGY */
  std::vector<std::string> arguments(argv, argv + argc);
  Parser parser(arguments);
  // parser.print();
  std::cout << std::endl;

  /* Reads input file then extracts contents */
  std::string text_to_encode = Helper::read_into_str(parser.input_file);
  // Helper::print_array(text_to_encode);

  /* MAKES FREQUENCY TABLE */
  std::unordered_map<char, uint32_t> freq_table =
      Helper::create_frequency_table(text_to_encode);
  // Helper::print_map(freq_table);

  /* MAKES CODE MAP*/
  Huffman::Tree tree(freq_table);
  std::unordered_map<char, std::string> codes = tree.generate_codes();
  // Helper::print_map(codes);

  /* WRITES INTO BINARY */
  // writes a bitcode string
  std::string bitcode_string = "";
  int padding = 0;
  for (const char &ch : text_to_encode) {
    bitcode_string += codes[ch];
  }
  // Pads the end with '0'
  padding =
      (bitcode_string.size() % 8) == 0 ? 0 : 8 - (bitcode_string.size() % 8);
  for (int i = 0; i < padding; i++) {
    bitcode_string += '0';
  }
  // actually writes as bits with padding
  Helper::write_bits_to_file(bitcode_string, parser.output_file);

  /* STATS */
  if (parser.stats) {
    Stats stats{};
    stats.input_bytes = text_to_encode.size();
    stats.input_bits = text_to_encode.size() * 8;
    stats.alphabet_size = freq_table.size();
    stats.header_bits = 0;
    stats.bitstream_bits = bitcode_string.size() - padding;
    stats.padding_bits = padding;
    stats.total_output_bits =
        stats.header_bits + stats.bitstream_bits + stats.padding_bits;
    stats.avg_code_length = Helper::get_avg_codelength(codes);

    stats.write(parser.stats_file);

    // for csv
    std::cout << parser.input_file << "," << parser.header_type << "," << stats.input_bytes << ","
              << stats.alphabet_size << "," << stats.bitstream_bits << ","
              << stats.header_bits << "," << stats.padding_bits << ","
              << stats.total_output_bits << ","
              << static_cast<double>(stats.total_output_bits) / (8 * stats.input_bytes) << std::endl;
  }

  return 0;
}
