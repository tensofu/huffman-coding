#include <fstream>
#include <iostream>
#include <random>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("insufficient arguments. run using the structure `./make_data  "
           "<single-symbol> <three-symbols>`\n");
    printf("\t note: <single-symbol> is a single character, and "
           "<three-symbols> is a string of three characters. \n");
    return 1;
  }

  if (strlen(argv[1]) != 1) {
    printf("The first argument needs to be a single character.\n");
    return 1;
  }

  if (strlen(argv[2]) != 3) {
    printf("The second argument needs to be 3 characters long.\n");
    return 1;
  }

  const int count = 10000;

  // list of valid chars
  std::vector<unsigned char> valid_chars;
  valid_chars.reserve(98);
  for (int i = 32 ;i < 127; i++) {
    valid_chars.push_back(static_cast<unsigned char>(i));
  }
  valid_chars.push_back('\n');
  valid_chars.push_back('\t');

  // Random device
  std::random_device rd;
  std::mt19937 engine(rd());

  // Generates a random string of characters (R)
  std::uniform_int_distribution<int> dist(0, valid_chars.size() - 1);
  std::string filename = "gen-data/random.txt";
  std::ofstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error: Could not open file " << filename << " for writing."
              << std::endl;
    return 1; // Exit with an error code
  }

  for (int i = 0; i < count; i++) {
    int random_int = dist(engine);
    file.put(valid_chars[random_int]);
  }

  file.close();

  // Generates string of a single character (S)
  std::string filename1 = "gen-data/single_symbol.txt";
  std::ofstream file1(filename1);
  if (!file1.is_open()) {
    std::cerr << "Error: Could not open file " << filename1 << " for writing."
              << std::endl;
    return 1; // Exit with an error code
  }

  for (int i = 0; i < count; i++) {
    file1.put(argv[1][0]);
  }

  file1.close();

  // Generates string of a single character (T)
  std::uniform_int_distribution<int> dist1(0, 2);
  std::string filename2 = "gen-data/three_symbols.txt";
  std::ofstream file2(filename2);
  if (!file2.is_open()) {
    std::cerr << "Error: Could not open file " << filename2 << " for writing."
              << std::endl;
    return 1; // Exit with an error code
  }

  for (int i = 0; i < count; i++) {
    int random_int = dist1(engine);
    file2.put(argv[2][random_int]);
  }

  file2.close();

  return 0;
}
