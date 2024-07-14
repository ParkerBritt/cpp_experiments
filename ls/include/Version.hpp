#ifndef VERSION_HPP
#define VERSION_HPP

#define VERSION_MAJOR 1
#define VERSION_MINOR 0
#define VERSION_PATCH 1

#include <string>

namespace version{
    std::string getVersion();
    std::string getVersionMessage();
}

#endif
