#ifndef DEFAULT_HPP
#define DEFAULT_HPP

#include "configParsing.hpp"
#include <memory>

class Defaults{
    public:
        Defaults();
        void setFromConfig(std::shared_ptr<ConfigParser> configParser);
        bool showBorder = true;
        
    private:
};

#endif
