#include "elf.h"
#include "utility.h"

using namespace elfy;

int main(int argc, char **argv) {
  if (argc < 2)
    utility::error(1, "Need more arguments!");

  ELF elf(argv[1]);

  elf.process();

  elf.dump_symbols();
}
