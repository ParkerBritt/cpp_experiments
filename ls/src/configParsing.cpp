#include "configParsing.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

ConfigParser::ConfigParser(std::string configFilePath){
    // check validity
    if(!fs::exists(configFilePath)){
        throw std::invalid_argument("Config file does not exist: " + configFilePath);
    }
    // find file
    ConfigParser::configFilePath = configFilePath;

    std::unordered_map<std::string, std::vector<std::string>>* keyValueBuffer;

    // read file
    std::ifstream configFile(configFilePath);
    std::string lineBuffer;
    if(configFile.is_open()){
        while(getline(configFile, lineBuffer)){
            if(lineBuffer[0]=='#' || lineBuffer.empty()) continue; // skip comments
            if(lineBuffer[0] == '[' && lineBuffer[lineBuffer.size()-1] == ']'){ // if section header
                std::string sectionName = lineBuffer.substr(1, lineBuffer.size()-2);
                // std::cout << "section name: " << sectionName << std::endl;
                std::unordered_map<std::string, std::vector<std::string>> keyValueMap;
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
            // std::cout << "key: " << key << std::endl;
            std::vector<std::string> values;

            // get values
            {
                size_t delimLen = delimeter.length();
                size_t posStart = delimeterIndx+delimLen;
                size_t posEnd;
                std::string value;
                while( (posEnd = lineBuffer.find(delimeter, posStart) ) != std::string::npos  ){
                    value = lineBuffer.substr(posStart, posEnd - posStart);
                    posStart = posEnd + delimLen;
                    values.push_back(value);
                    // std::cout << "value: " << value << std::endl;
                }
                value = lineBuffer.substr(posStart);
                // std::cout << "value: " << value << std::endl;
                values.push_back(value);
            }
            // const std::string value = lineBuffer.substr(delimeterIndx+1, lineBuffer.size()-delimeterIndx-1);
            keyValueBuffer->insert_or_assign(key, values);
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
            std::cout << "key: " << kvPair.first << " value: " << kvPair.second[0] << std::endl;
        }
    }
}

std::unordered_map<std::string, std::vector<std::string>> ConfigParser::getSectionContents(std::string section){
    return sectionMap[section];
}
