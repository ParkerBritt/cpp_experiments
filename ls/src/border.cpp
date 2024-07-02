#include "border.hpp"
#include "AnsiUtils.hpp"

Border::Border(){
    borderColor = AnsiUtils::Color(255,255,255);
}

std::string Border::getTop() const{
    std::string borderTop;
    borderTop += borderColor.getEscape();
    borderTop+=topLeftCorner;
    for(size_t i=0; i<width; i++){
        borderTop+=horizontal;
    }
    borderTop+=topRightCorner+"\n";
    return borderTop;
}

std::string Border::getBottom() const{
    std::string borderBottom;
    borderBottom += borderColor.getEscape();
    borderBottom+=bottomLeftCorner;
    for(size_t i=0; i<width; i++){
        borderBottom+=horizontal;
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
