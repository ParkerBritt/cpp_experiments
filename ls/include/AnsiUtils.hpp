#ifndef ANSI_UTILS_HPP
#define ANSI_UTILS_HPP

#include <string>
#include <unordered_map>
#include <iostream>

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
    class ColorTheme{
        public:
            ColorTheme() = default;
            void add(std::string, int r, int g, int b);
            void add(std::string name, AnsiUtils::Color color);

            std::string get(std::string name); 
        private:
            std::unordered_map<std::string, AnsiUtils::Color> themeMap;
    };

    std::string color(int r, int g, int b);
    std::string color(const colorVector& color);
    std::string reset();
    std::string bold();
}

#endif
