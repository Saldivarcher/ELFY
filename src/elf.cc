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

  section_header *section_headers =
      (section_header *)calloc(section_size, section_header_size);

  if (file.seekg(e_header.e_shoff, std::ios::beg);
      !file.read(reinterpret_cast<char *>(section_headers), section_size))
    utility::error(7, "Could not parse section header!");

  for (unsigned i = 0; i < e_header.e_shnum; i++)
    shdrs.emplace_back(new section_header(section_headers[i]));

  free(section_headers);
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

void ELF::process() {
  process_elf_header();
  process_section_header();
}

std::vector<std::unique_ptr<symbol_data>> ELF::get_elf_symbols(const shdrs_ptr& section,
                                                  unsigned long &num) {

  auto* symbols = (symbol_data*) calloc(section->sh_size, sizeof(symbol_data));

  file.seekg(section->sh_offset, std::ios::beg);
  file.read(reinterpret_cast<char*>(symbols), section->sh_size);

  std::vector<std::unique_ptr<symbol_data>> v;
  for (unsigned i = 0; i < num; i++)
    v.emplace_back(new symbol_data(symbols[i]));

  free(symbols);
  return v;
}

void ELF::dump_symbols() {
  // 1. Iterate through sections
  // 2. Get symbol table for each section
  // 3. Print out the symbols within the symbol table
  
  // get the string table of a section
  auto get_str_tab = [&](auto& string_section) -> char * {
    // TODO: Seems to be segfaulting here. Fix.
    char *strtab = new char[string_section->sh_size];
    file.seekg(string_section->sh_offset, std::ios::beg);
    file.read(strtab, string_section->sh_size);
    return strtab;
  };

  for (const auto &section : shdrs) {
    if (section->sh_type != SHT::SHT_SYMTAB &&
        section->sh_type != SHT::SHT_DYNSYM)
      continue;

    unsigned long num = section->sh_size / section->sh_entsize;
    auto symtab = get_elf_symbols(section, num);

    auto& string_section = shdrs[section->sh_link];
    char * test = get_str_tab(string_section);

    for (const auto& psym: symtab) {
      std::cout << test + psym->st_name << std::endl;
    }

    delete[] test;
  }
}

} // end of namespace elfy
