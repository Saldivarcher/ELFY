#include "elf.h"

#include <cassert>
#include <cstring>

namespace elfy {

ELF::ELF(const char *f) : filename(f), file() { load_file(); }

inline bool ELF::is_elf() {
  return std::memcmp(e_header.e_ident, magic_numbers, 4) == 0;
}

void ELF::load_file() {
  std::ifstream ifs(filename.data(), std::ios::binary);
  if (!ifs)
    utility::error(2, "Could not open binary file!");

  file.swap(ifs);
}

void ELF::process_section_header() {
  const int section_size = e_header.e_shnum * section_header_size;

  char section_headers[section_size];

  if (file.seekg(e_header.e_shoff); !file.read(section_headers, section_size))
    utility::error(7, "Could not parse section header!");

  int j = 0;
  for (unsigned i = 0; i < e_header.e_shnum; i++) {
    // TODO: Clean this up
    char s[section_size];
    std::strncpy(s, section_headers + j, section_header_size);

    if (auto *sh = reinterpret_cast<section_header *>(s))
      shdrs.push_back(*sh);
    j += section_header_size;
  }

  assert(shdrs.size() == e_header.e_shnum);
}

void ELF::process_elf_header() {
  // Get the first 16 bytes of the "ELF" file
  file.read(e_header.e_ident, EI_NIDENT);
  if (!is_elf())
    utility::error(5, "This file is not an ELF file!");

  // Get the rest of the elf header
  if (!file.read(reinterpret_cast<char *>(&e_header.e_type),
                 elf_header_size - EI_NIDENT))
    utility::error(6, "Could not parse elf header!");
}

bool ELF::process() {
  process_elf_header();
  process_section_header();

  return true;
}

void ELF::dump_symbols() {}

} // end of namespace elfy
