#include <cmath>
#include <iostream>
#include <chrono>
#include <fstream>
#include <string>
#include <cstring>
#include <bitset>
#include <typeinfo>

std::string encrypt(std::string input_file, int key) {
  std::string message;
  std::string line;
  
  std::ifstream input_stream;
  input_stream.open(input_file);

  while(!input_stream.eof()) {
    getline(input_stream, line);
    message += line + "\n";
  }

  input_stream.close();
  
  std::string encrypted_message;
  for(int i = 0; i < message.length(); i++) {
    int ascii_value = int(message[i]);
    encrypted_message += std::bitset<8>(ascii_value + i + key).to_string();
  }

  return encrypted_message;    
}

std::string decrypt(std::string encrypted_message, int key) {
  std::string decrypted_message;

  for(int i = 0; i < encrypted_message.length(); i += 8) {
    std::string value = encrypted_message.substr(i, 8);
    int ascii_value = std::bitset<8>(value).to_ulong();
    ascii_value -= (i/8) + key;
    decrypted_message += char(ascii_value);  
  }

  return decrypted_message;
}

void write_file(std::string output_file, std::string content) {
  std::ofstream output_stream(output_file);
  output_stream << content;
  output_stream.close();
  
}

int main(int argc, char* argv[]) {
  if (argc != 5) {
    std::cerr << "Usage: " << argv[0] << " -e OR -d <integer_key> -i <input_file>\n";
    return 1;
  }

  std::string type;
  std::string operation;
  int key = 0;
  std::string input_file;
  std::string output_file;
  
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-e") == 0) {
      type = argv[i];
      key = std::stoi(argv[++i]);
      operation = "encryption";
      output_file = "encrypted_message.txt";
    }
    else if (strcmp(argv[i], "-d") == 0) {
      type = argv[i];
      key = std::stoi(argv[++i]);
      operation = "decryption";
      output_file = "decrypted_message.txt";
    }
    else if (strcmp(argv[i], "-i") == 0) {
      input_file = argv[++i];
    }
  }
    
  auto start = std::chrono::high_resolution_clock::now();
  
  if (type == "-e") {
    std::string encrypted_message = encrypt(input_file, key);
    write_file(output_file, encrypted_message);
  }
  else if (type == "-d") {
    std::ifstream input_stream(input_file);
    if (!input_stream.is_open()) {
      std::cerr << "Error opening input file: " << input_file << std::endl;
      return 1;
    }

    std::string encrypted_message((std::istreambuf_iterator<char>(input_stream)), std::istreambuf_iterator<char>());
    input_stream.close();
    
    std::string decrypted_message = decrypt(encrypted_message, key);
    write_file(output_file, decrypted_message);   
  }
  else {
    std::cerr << "Usage: " << argv[0] << " -e OR -d <integer_key> -i <input_file>\n";
    return 1;
  }
  
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
 
  double seconds = duration.count() / 1'000'000.0;
  std::cout << operation << " took " << seconds << " seconds\n";
  
  return 0;
}
