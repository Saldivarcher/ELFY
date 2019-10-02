#ifndef UTILITY_H
#define UTILITY_H

#include <dbg.h>
#include <iostream>
#include <string>

namespace elfy {
namespace utility {
void error(int error_code, const std::string &error_msg);
} // end of namespace utility
} // end of namespace elfy

#endif
