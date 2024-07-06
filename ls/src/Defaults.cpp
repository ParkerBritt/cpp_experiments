#include "Defaults.hpp"
#include "configParsing.hpp"
#include <iostream>
#include <memory>

Defaults::Defaults(){
    // std::cout << "constructor" << std::endl;;
}

void Defaults::setFromConfig(std::shared_ptr<ConfigParser> configParser){
    ConfigParsing::ConfigMap configMap = configParser->getSectionContents("Defaults");
    if(configMap.find("show_border") != configMap.end()){
        showBorder = "true" == configMap["show_border"][0];
    }
}


