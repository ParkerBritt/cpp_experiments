#include "border.hpp"

std::string Border::getTop(){
    std::string borderTop;
    borderTop+=topLeftCorner;
    for(size_t i=0; i<width; i++){
        borderTop+=horizontal;
    }
    borderTop+=topRightCorner+"\n";
    return borderTop;
}

void Border::setWidth(int width){
    Border::width = width;
}
