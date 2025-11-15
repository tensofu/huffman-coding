# Huffman Coding 

A simple Huffman coding assignment, for use with ASCII characters.

Things to note:
- `--header` was not implemented yet.
- `results.csv` was made manually. If the `--stats` option is enabled when running the program, then it will display a `.csv` entry into the console.
- If there is padding, then the padded bits appended to the end as `0`.

### Usage

1. Compile `huffencode.cpp` using the C++17 or later standard

```bash
g++ huffencode.cpp -std=c++17 -o huffencode
```

2. Attempt some of the following commands, according to this commandline structure: `huffencode -i <input.txt> -o <output.huff> --header <none|freq32> --stats <stats.json>`. All files will be in the `output/` directory.

```bash
# Without --header or --stats
./huffencode -i english_10k.txt -o english_10k.huff 
./huffencode -i random_10k.txt -o random_10k.huff
./huffencode -i single_symbol_10k.txt -o single_symbol_10k.huff
./huffencode -i three_symbols_10k.txt -o three_symbols_10k.huff

# With --stats
./huffencode -i english_10k.txt -o english_10k.huff --stats english_10k.json
./huffencode -i random_10k.txt -o random_10k.huff --stats random_10k.json
./huffencode -i single_symbol_10k.txt -o single_symbol_10k.huff --stats single_symbol_10k.json
./huffencode -i three_symbols_10k.txt -o three_symbols_10k.huff --stats three_symbols_10k.json

# Edge cases (nothing in file)
./huffencode -i empty.txt -o empty.huff 
./huffencode -i empty.txt -o empty.huff --stats empty.json
```

3. Generate the files into `gen-data/` using `make_data.cpp` It follows the structure `./make_data <single-symbol> <three-symbols>`.

```bash
# Compile
g++ make_data.cpp -std=c++17 -o make_data

# Example usage
./make_data A CBA
```
