#ifndef ANSI_UTILS_HPP
#define ANSI_UTILS_HPP

#include <string>

namespace AnsiUtils{
    using colorVector = int[3];
    class Color{   
        public:
            Color(int colorVector[3]);
            Color(int r, int g, int b);
            Color(std::string colorHex);
            std::string getEscape();
        private:
            int r,g,b;
    };

    std::string color(int r, int g, int b);
    std::string color(const colorVector& color);
    std::string reset();
    std::string bold();
}

#endif
