#ifndef ELF_H
#define ELF_H

#include <array>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <string_view>
#include <vector>

#include "utility.h"

constexpr int ELFMAG0 = 0x7f;
constexpr int ELFMAG1 = 'E';
constexpr int ELFMAG2 = 'L';
constexpr int ELFMAG3 = 'F';
constexpr int EI_NIDENT = 16;

namespace elfy {

using elf_half_t = uint16_t;
using elf_word_t = uint32_t;
using elf_qword_t = uint64_t;

struct elf_header {
  // [0-3] Magic numbers
  // [4] either 1 or 2, 32-bit or 64-bit
  // [5] either 1 or 2, little endian or big endian
  // [6] Version of ELF
  // [7] Target operating system
  // [8] Specifies ABI version
  // [9-16] Not used
  char e_ident[EI_NIDENT];

  // Type of object file
  elf_half_t e_type;

  // Target ISA
  // 0x3E is x86-64
  elf_half_t e_machine;

  // Usually just 1 for the original ELF version
  elf_word_t e_version;

  // Memory address for the entry point of the process
  elf_qword_t e_entry;

  // Points to the start of the program header table
  // 0x34 for 32-bit or 0x40 for 64-bit offset
  elf_qword_t e_phoff;

  // Points to the start of the section header table
  elf_qword_t e_shoff;
};

struct section_header {
  // Offset to a string in the .shstrtab section
  elf_word_t sh_name;

  // Identifies the type of this header
  elf_word_t sh_type;

  // Identfies the attributes of the section
  elf_qword_t sh_flags;

  // Virtual address of the section in memory, for sectino that are loaded
  elf_qword_t sh_addr;

  // Offset of the section in the file image
  elf_qword_t sh_offset;

  // Size in bytes of the section in the file image, can be 0
  elf_qword_t sh_size;

  // Contains the section index of an associated section
  elf_word_t sh_link;

  // Contains extra information about the section
  elf_word_t sh_info;
  elf_qword_t sh_addralign;
  elf_qword_t sh_entsize;
};

const static int section_header_size = sizeof(section_header);

class ELF {
public:
  ELF(const char *);

  bool process();
  void dump_symbols();

private:
  elf_header header;

  std::string_view filename;

  // All the binary data of the file
  std::ifstream file;

  bool is_elf();

  void load_file();
  void process_header();

  int get_byte(char *, int);

  // TODO: make unsigned
  static constexpr char magic_numbers[] = {ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3};
  std::string buffer;
};
} // end of namespace elfy

#endif
