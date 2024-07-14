#include "Version.hpp"
#include <string>

std::string version::getVersion(){
    return std::to_string(VERSION_MAJOR)+"."+std::to_string(VERSION_MINOR)+"."+std::to_string(VERSION_PATCH);
}

std::string version::getVersionMessage(){
    return std::string("pls - A custom alternative to ls\n") +
    "version " + version::getVersion() + "\n" + 
    "https://github.com/ParkerBritt/cpp_experiments/tree/main/ls\n";
}
