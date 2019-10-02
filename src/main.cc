#include "elf.h"
#include "utility.h"

using namespace elfy;

int main(int argc, char **argv) {
  if (argc < 2)
    utility::error(1, "Need more arguments!");

  ELF elf(argv[1]);

  if (!elf.process())
    utility::error(1, "Not an elf file!");

  elf.dump_symbols();
}
