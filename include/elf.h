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

  // Interpretation of this field depends on the target architecture.
  elf_word_t e_flags;

  // Contains the size of this header, normally 64 Bytes for 64-bit and 52 Bytes
  // for 32-bit format.
  elf_half_t e_ehsize;

  // Contains the size of a program header table entry.
  elf_half_t e_phentsize;

  // Contains the number of entries in the program header table.
  elf_half_t e_phnum;

  // Contains the size of a section header table entry.
  elf_half_t e_shentsize;

  // Contains the number of entries in the section header table.
  elf_half_t e_shnum;

  // Contains index of the section header table entry that contains the section
  // names.
  elf_half_t e_shstrndx;
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

  // Contains the required alignment of the section
  elf_qword_t sh_addralign;

  // Contains the size, in bytes, of each entry, for sections that contain
  // fixed-size entries
  elf_qword_t sh_entsize;
};

struct symbol_data {
  // Index into the string table of the name of the symbol, or 0 for scratch
  // register
  elf_word_t st_name;

  // Register number
  char st_info;

  // Unused
  char st_other;

  // Bind is typically `STB_GLobal`
  elf_half_t st_shndx;

  elf_qword_t st_value;
  elf_qword_t st_size;
};

enum SHT { SHT_SYMTAB = 2, SHT_DYNSYM = 11 };

static constexpr int elf_header_size = sizeof(elf_header);
static constexpr int section_header_size = sizeof(section_header);

using shdrs_ptr = std::unique_ptr<section_header>;

class ELF {
public:
  ELF(const char *);

  void process();
  void dump_symbols();

private:
  elf_header e_header;
  std::vector<shdrs_ptr> shdrs;

  std::string_view filename;

  // All the binary data of the file
  std::ifstream file;

  bool is_elf();

  std::unique_ptr<symbol_data> get_elf_symbols(const shdrs_ptr&, unsigned long &);

  void load_file();
  void process_elf_header();
  void process_section_header();

  // To check against the first 4 bytes in a file
  static constexpr char magic_numbers[] = {ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3};
};
} // end of namespace elfy

#endif
