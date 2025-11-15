#pragma once

#include <iostream>
#include <string>
#include <vector>

struct Parser {
  std::string input_file{};
  std::string output_file{};
  std::string header_type = "none";
  std::string stats_file = "stats.json";
  bool header = false;
  bool stats = false;

  /* CONSTRUCTORS */
  // DEFAULT CONSTRUCTOR
  Parser()
      : input_file(""), output_file(""), header_type("none"),
        stats_file("stats.json"), header(false), stats(false) {};

  // CONSTRUCTING FROM VECTOR OF STD::STRING (ARGUMENTS)
  Parser(const std::vector<std::string> &args) {
    int n = args.size();
    for (int i = 0; i < n; i++) {
      std::string_view arg = args[i]; // CURRENT ARGUMENT
      // CHECKS CASES
      if (arg == "-i") {
        if (i + 1 < n) {
          input_file = args[++i];
        } else {
          printf(
              "`-i` was expecting an input file, but reached end of arguments "
              "list. nothing will be assigned as the input file.\n");
          break;
        }
      } else if (arg == "-o") {
        if (i + 1 < n) {
          output_file = args[++i];
        } else {
          printf(
              "`-o` was expecting an output file, but reached end of arguments "
              "list. nothing will be assigned as the output file.\n");
          break;
        }
      } else if (arg == "--header") {
        if (i + 1 < n) {
          std::string opt = args[i + 1];
          if (opt == "none" || opt == "freq32") {
            header_type = args[++i];
          } else {
            printf("`--header` did not find a `none` or `freq32` option. "
                   "header will be set to `none`.\n");
          }
        } else {
          printf("`--header` was expecting a type of either <none|freq32>, but "
                 "reached end of arguments list. `none` will be assigned as "
                 "the header type.\n");
        }
      } else if (arg == "--stats") {
        (*this).stats = true;
        if (i + 1 < n) {
          std::string opt = args[i + 1];
          if (opt.length() >= 5 &&
              0 == opt.compare(opt.length() - 5, 5, ".json")) {
            stats_file = args[++i];
          } else {
            printf("the value passed for `--stats` is not a json file. the "
                   "designated file will be `stats.json`.");
          }
        } else {
          printf("`--stats` was expecting a file path, but reached end of "
                 "arguments list. `stats.json` will be assigned as the stats "
                 "file.\n");
        }
      }
    }
  }

  /* PRINTING STUFF */
  // PRINTS USAGE (ERROR)
  void print_usage() {
    std::cerr << "Usage: huffencode -i <input.txt> -o <output.huff> --header "
                 "<none|freq32> [--stats <stats.json>]"
              << std::endl;
  }

  // PRINTS MEMBERS
  void print() {
    std::cout << input_file << std::endl;
    std::cout << output_file << std::endl;
    std::cout << header_type << std::endl;
    std::cout << stats_file << std::endl;
    std::cout << std::boolalpha << header << std::endl;
    std::cout << std::boolalpha << stats << std::endl;
  }
};
