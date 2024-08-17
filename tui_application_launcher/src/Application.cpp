#include "Application.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <boost/filesystem.hpp>

namespace bfs = boost::filesystem;

launcher::Application::Application(bfs::path desktopPath){
    Application::desktopPath = desktopPath;

    std::string desktopFilePathStr = desktopPath.string();
    // std::cout << "foo:" << desktopFilePathStr << std::endl;
    std::ifstream desktopFileStream(desktopFilePathStr);

    if(!desktopFileStream.is_open()){
        std::cerr << "failed to open file: " << desktopFilePathStr << std::endl; // add file path
    }

    std::string curLine;

    while (getline(desktopFileStream, curLine)){
        if(curLine.substr(0, 5)=="Name="){
            Application::appName = curLine.substr(5);
            break;
        }
    }

    desktopFileStream.close();

}
