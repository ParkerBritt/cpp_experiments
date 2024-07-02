#ifndef BORDER_HPP
#define BORDER_HPP

#include <string>
#include "AnsiUtils.hpp"

class Border{
    public:
        // member variables
        std::string horizontal="─";
        std::string vertical="│";
        std::string topLeftCorner="╭";
        std::string bottomLeftCorner="╰";
        std::string topRightCorner="╮";
        std::string bottomRightCorner="╯";
        AnsiUtils::Color borderColor;


        //member functions
        Border();
        
        std::string getTop() const;
        std::string getBottom() const;
        void setWidth(unsigned  width);
        unsigned  getWidth();
    private:
        unsigned int horizontalPadding = 0;
        unsigned int verticalPadding = 0;
        unsigned int width = 0;

};


#endif
