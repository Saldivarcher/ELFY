#include "options.h"

#include <iostream>

using namespace elfy;

void Options::process(int argc, char **argv) {
  for (int i = 0; i < argc; i++)
    file_names.emplace_back(argv[i]);
}
