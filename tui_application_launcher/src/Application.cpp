#include "Application.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <boost/filesystem.hpp>
#include "IconMap.hpp"
#include <boost/algorithm/string/case_conv.hpp>

namespace bfs = boost::filesystem;
using namespace launcher;

Application::Application(bfs::path desktopPath){
    Application::desktopPath = desktopPath;

    std::string desktopFilePathStr = desktopPath.string();
    std::ifstream desktopFileStream(desktopFilePathStr);

    // read file
    if(!desktopFileStream.is_open()){
        std::cerr << "failed to open file: " << desktopFilePathStr << std::endl; // add file path
    }

    std::string curLine;

    bool nameFound = false;
    bool execFound = false;
    while (getline(desktopFileStream, curLine)){
        if(!nameFound && curLine.substr(0, 5)=="Name="){
            Application::appName = curLine.substr(5);
        }
        else if(!execFound && curLine.substr(0, 5)=="Exec="){
            Application::execCommand = curLine.substr(5);
        }
        if(nameFound && execFound) break;
    }

    desktopFileStream.close();

    // set icon
    lowerAppName = appName;
    boost::algorithm::to_lower(lowerAppName);
    // default icon
    Application::icon = "󰘔";
    if(iconNameMap.find(lowerAppName) != iconNameMap.end()){
        Application::icon = iconNameMap[lowerAppName];
    }
    else{
        for(auto i : iconKeywordMap){
            std::string keyword = i.first;
            std::string icon = i.second;

            // find keyword in app name
            size_t foundPos = lowerAppName.find(keyword);
            // skip if keyword not found
            if(foundPos == std::string::npos) continue;

            Application::icon = icon;
        }
    }
}

// getters
std::string Application::getAppName() const{
    return appName;
};
std::string Application::getExecCommand() const{
    return execCommand;
};
std::string Application::getDisplayName() const{
    return icon + " " + Application::getAppName();
}
std::string Application::getLowerAppName() const{
    return lowerAppName;
}
bfs::path Application::getDesktopPath() const{
    return desktopPath;
}

// operators
bool Application::operator<(const Application& rhs) const{
    return appName < rhs.getAppName();
}
