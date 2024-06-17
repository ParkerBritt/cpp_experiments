#ifndef UTILS_HPP
#define UTILS_HPP

#include <sys/ioctl.h>
#include <unistd.h>
#include <tuple>
#include <string>


std::tuple<unsigned short, unsigned short> getWinSize();
namespace utils{
    std::string tolower(std::string str);
}

#endif
