#pragma once

#include <map>
#include <string>
#include <vector>

namespace eyescan {

// Arguments are specified as --arg_name argval1 argval2 ...
using arg_map_t = std::map<std::string, std::vector<std::string>>;

// Parse args from the main arg list
arg_map_t parse_args(int argc, char* argv[]);

// Get the first argument value from a map. If not available, return default.
template <typename value_type>
inline value_type get_arg_or_default(const arg_map_t& args, std::string arg_name, value_type default_val)
{
    auto it = args.find(arg_name);
    return it == args.end() ? default_val : (it->second)[0];
}

// Template specialization for int arguments
template <>
inline int get_arg_or_default(const arg_map_t& args, std::string arg_name, int default_val)
{
    auto it = args.find(arg_name);
    return it == args.end() ? default_val : std::stoi((it->second)[0]);
}

}