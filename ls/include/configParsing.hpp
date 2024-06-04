#ifndef CONFIG_PARSING_HPP
#define CONFIG_PARSING_HPP

#include <regex>
#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

class ConfigParser{
    public:
        std::string configFilePath;

        ConfigParser(std::string configFilePath);
        std::unordered_map<std::string, std::string> getSectionContents(std::string section);
};


#endif
