#include <string>
#include <vector>

namespace tinyrefl
{

// TODO: define this
//
bool reflect_file(
    const std::string&              filepath,
    const std::string&              cpp_standard,
    const std::vector<std::string>& compile_options);
} // namespace tinyrefl
