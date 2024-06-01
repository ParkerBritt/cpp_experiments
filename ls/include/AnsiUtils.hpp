#ifndef ANSI_UTILS_HPP
#define ANSI_UTILS_HPP

#include <string>

namespace AnsiUtils{
    using colorVector = int[3];

    std::string color(int r, int g, int b);
    std::string color(const colorVector& color);
    std::string reset();
    std::string bold();
}

#endif
