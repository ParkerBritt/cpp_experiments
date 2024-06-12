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

    // Class Color
    Color::Color(){
        Color(0,0,0);
    }
    Color::Color(int colorVector[3]){
        r=colorVector[0];
        g=colorVector[1];
        b=colorVector[2];
    }
    Color::Color(int r, int g, int b){
        Color::r = r;
        Color::g = g;
        Color::b = b;
    }
    Color::Color(std::string colorHex){
    }
    std::string Color::getEscape(){
        return "\e[38;2;"+std::to_string(r)+";"+std::to_string(g)+";"+std::to_string(b)+"m";
    }

    // Class Color Theme
    void ColorTheme::add(std::string name, int r, int g, int b){
        themeMap.emplace(name, AnsiUtils::Color(r, g, b));
    }
    void ColorTheme::add(std::string name, AnsiUtils::Color color){
        themeMap.emplace(name, color);
    }
    std::string ColorTheme::get(std::string name){
        return themeMap.at(name).getEscape();
    }
}
