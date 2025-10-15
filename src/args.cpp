#include "args.h"

namespace eyescan {

arg_map_t parse_args(int argc, char* argv[])
{
    arg_map_t arg_map;

    std::string curr_arg;
    for (int argi = 0; argi < argc; argi++) {
        std::string argstr(argv[argi]);
        
        if (argstr.substr(0, 2) == "--") {
            curr_arg = argstr.substr(2);
            continue;
        } else {
            arg_map[curr_arg].push_back(argstr);
        }
    }

    return arg_map;
}

}