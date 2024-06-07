#include "configParsing.hpp"

ConfigParser::ConfigParser(std::string configFilePath){
    // check validity
    if(!fs::exists(configFilePath)){
        throw std::invalid_argument("Config file does not exist: " + configFilePath);
    }
    // find file
    ConfigParser::configFilePath = configFilePath;

    std::unordered_map<std::string, std::string>* keyValueBuffer;

    // read file
    std::ifstream configFile(configFilePath);
    std::string lineBuffer;
    if(configFile.is_open()){
        while(getline(configFile, lineBuffer)){
            if(lineBuffer[0]=='#' || lineBuffer.empty()) continue; // skip comments
            if(lineBuffer[0] == '[' && lineBuffer[lineBuffer.size()-1] == ']'){ // if section header
                std::string sectionName = lineBuffer.substr(1, lineBuffer.size()-2);
                // std::cout << "section name: " << sectionName << std::endl;
                std::unordered_map<std::string, std::string> keyValueMap;
                sectionMap[sectionName] = keyValueMap;
                keyValueBuffer = &sectionMap[sectionName];
                continue;
            }
            if(sectionMap.empty()){
                throw std::runtime_error("Invalid config. Expected header before key value pairs");
            }
            const std::string delimeter = " ";
            const int delimeterIndx = lineBuffer.find(delimeter);
            const std::string key = lineBuffer.substr(0, delimeterIndx);
            const std::string value = lineBuffer.substr(delimeterIndx+1, lineBuffer.size()-delimeterIndx-1);
            // std::cout << "Line:" << lineBuffer << std::endl;
            // std::cout << "Key:" << key << std::endl;
            // std::cout << "Value:" << value << std::endl;
            keyValueBuffer->insert_or_assign(key, value);
        }
        configFile.close();
    }
    else{
        throw std::ios_base::failure("Couldn't open file: " + configFilePath);
    }


}

void ConfigParser::debugPrintSectionContents(){
        for( const auto sections : sectionMap){
        std::cout << "section: " << sections.first << std::endl;
        for( const auto kvPair : sections.second){
            std::cout << "key: " << kvPair.first << " value: " << kvPair.second << std::endl;
        }
    }
}

std::unordered_map<std::string, std::string> ConfigParser::getSectionContents(std::string section){
    return sectionMap[section];
}
