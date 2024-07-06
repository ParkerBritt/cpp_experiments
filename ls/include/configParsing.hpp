#ifndef CONFIG_PARSING_HPP
#define CONFIG_PARSING_HPP

#include <string>
#include <unordered_map>
#include <vector>

namespace ConfigParsing{
    using ConfigMap = std::unordered_map<std::string, std::vector<std::string>>;
}

class ConfigParser{
    public:
        ConfigParser(std::string configFilePath);
        std::unordered_map<std::string, std::vector<std::string>> getSectionContents(std::string section);
        void debugPrintSectionContents();
    private:
        std::string configFilePath;
        std::unordered_map<std::string, int> sectionLineIndex; // basically a glossary, maps section name to a line number
        std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>> sectionMap;
};



#endif
