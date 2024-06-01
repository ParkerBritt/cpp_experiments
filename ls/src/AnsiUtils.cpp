#include "AnsiUtils.hpp"

namespace AnsiUtils{
    std::string color(int r, int g, int b){
        return "\e[38;2;"+std::to_string(r)+";"+std::to_string(g)+";"+std::to_string(b)+"m";
    }

    std::string color(const colorVector& color){
        return AnsiUtils::color(color[0], color[1], color[2]);
    }

    std::string reset(){
        return "\e[0m";
    }

    std::string bold(){
        return "\e[1m";
    }
}
