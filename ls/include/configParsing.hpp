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
        ConfigParser(std::string configFilePath);
        std::unordered_map<std::string, std::string> getSectionContents(std::string section);
    private:
        std::string configFilePath;
        std::unordered_map<std::string, int> sectionLineIndex; // basically a glossary, maps section name to a line number
        std::unordered_map<std::string, std::unordered_map<std::string, std::string>> sectionMap;
};



#endif
