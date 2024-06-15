#ifndef UTILS_HPP
#define UTILS_HPP

#include <sys/ioctl.h>
#include <unistd.h>
#include <tuple>


std::tuple<unsigned short, unsigned short> getWinSize();

#endif
