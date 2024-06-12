#ifndef BORDER_HPP
#define BORDER_HPP

#include <string>

class Border{
    public:
        // member variables
        std::string horizontal="─";
        std::string vertical="│";
        std::string topLeftCorner="╭";
        std::string bottomLeftCorner="╰";
        std::string topRightCorner="╮";
        std::string bottomRightCorner="╯";


        //member functions
        Border() = default;
        
        std::string getTop();
        void setWidth(int width);
    private:
        int horizontalPadding = 0;
        int verticalPadding = 0;
        int width = 0;

};


#endif
