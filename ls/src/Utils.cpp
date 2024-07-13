#include "Utils.hpp"
#include <string>

std::tuple<unsigned short, unsigned short> getWinSize(){
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    std::tuple<unsigned short, unsigned short> windowSizeTuple(w.ws_row, w.ws_col);
    return windowSizeTuple;
}

std::string utils::tolower(std::string str)
{
    for(auto& c : str){
        c = std::tolower(c);
    }
    return str;
}

std::string utils::getVersion(){
    return "1.0.1";
}

std::string utils::getVersionMessage(){
    return std::string("pls - A custom alternative to ls\n") +
    "version " + utils::getVersion() + "\n" + 
    "https://github.com/ParkerBritt/cpp_experiments/tree/main/ls\n";
}
