#include "border.hpp"

std::string Border::getTop() const{
    std::string borderTop;
    borderTop+=topLeftCorner;
    if(width>2){
        for(size_t i=0; i<width-2; i++){
            borderTop+=horizontal;
        }
    }
    borderTop+=topRightCorner+"\n";
    return borderTop;
}

std::string Border::getBottom() const{
    std::string borderBottom;
    borderBottom+=bottomLeftCorner;
    if(width>2){
        for(size_t i=0; i<width-2; i++){
            borderBottom+=horizontal;
        }
    }
    borderBottom+=bottomRightCorner;
    return borderBottom;
}

void Border::setWidth(unsigned int width){
    Border::width = width;
}

unsigned int Border::getWidth(){
    return Border::width;
}
