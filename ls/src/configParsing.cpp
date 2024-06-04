#include "configParsing.hpp"

ConfigParser::ConfigParser(std::string configFilePath){
    // check validity
    if(!fs::exists(configFilePath)){
        throw std::invalid_argument("Config file path does not exist: " + configFilePath);
    }
    // find file
    ConfigParser::configFilePath = configFilePath;

}

std::unordered_map<std::string, std::string> ConfigParser::getSectionContents(std::string section){
    std::unordered_map<std::string, std::string> dataMap;

    // read file
    std::ifstream configFile(configFilePath);
    std::string lineBuffer;
    if(configFile.is_open()){
        while(getline(configFile, lineBuffer)){
            std::cout << lineBuffer << std::endl;
        }
        configFile.close();
    }
    else{
        throw std::ios_base::failure("Couldn't open file: " + configFilePath);
    }

    return dataMap;
}
