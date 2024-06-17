#include "Utils.hpp"

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
