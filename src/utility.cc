#include "utility.h"

namespace elfy {
namespace utility {
void error(int error_code, const std::string &error_msg) {
  std::cerr << error_msg << std::endl;
  std::exit(error_code);
}
} // namespace utility
} // namespace elfy
