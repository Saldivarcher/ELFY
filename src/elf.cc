#include "elf.h"

#include <cstring>

namespace elfy {

ELF::ELF(const char *f) : filename(f), file() {
  load_file();
}

inline bool ELF::is_elf() {
  return std::memcmp(header.e_ident, magic_numbers, 4) == 0;
}

void ELF::load_file() {
  std::ifstream ifs(filename.data(), std::ios::binary | std::ios::ate);
  if (!ifs)
    utility::error(2, "Could not open binary file!");

  buffer.resize(ifs.tellg());
  // Reset back to the beginning of the stream
  ifs.seekg(0);

  file.swap(ifs);
}

int get_byte(char *field, int size) {
  switch (size) {
  case 1:
    return *field;

  case 2:
    return (field[0]) | (field[1] << 8);

  case 3:
    return (field[0]) | (field[1] << 8) | (field[2] << 16);

  // My machine has 4-byte int's so no need to check for 8-bytes :)
  case 4:
  case 5:
  case 6:
  case 7:
  case 8:
    return (field[0]) | (field[1] << 8) | (field[2] << 16) | (field[3] << 24);

  default:
    utility::error(6, "Bad byte?");
    exit(-1);
  }
}

// Assuming little endian :)
void ELF::process_header() {
  file.read(header.e_ident, EI_NIDENT);
  if (!is_elf())
    utility::error(5, "This file is not an ELF file!");
    
  
  file.read(reinterpret_cast<char*>(&header.e_type), sizeof(elf_header) - EI_NIDENT);
  return;
}

bool ELF::process() {
  process_header();

  return true;
}

void ELF::dump_symbols() {

}

} // end of namespace elfy
