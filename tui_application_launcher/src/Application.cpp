#include "Application.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <boost/filesystem.hpp>

namespace bfs = boost::filesystem;

launcher::Application::Application(bfs::path desktopPath){
    Application::desktopPath = desktopPath;

    std::string desktopFilePathStr = desktopPath.string();
    std::ifstream desktopFileStream(desktopFilePathStr);

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

}

// getters
std::string launcher::Application::getAppName() const{
    return appName;
};
std::string launcher::Application::getExecCommand() const{
    return execCommand;
};

// operators
bool launcher::Application::operator<(const Application& rhs) const{
    return appName < rhs.getAppName();
}
