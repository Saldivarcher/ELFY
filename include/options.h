#ifndef OPTIONS_H
#define OPTIONS_H

#include <string_view>
#include <vector>

namespace elfy {

class Options {
public:
  Options() = default;

  void process(int, char **);

private:
  std::vector<std::string_view> file_names;
};

} // end of namespace elfy

#endif
