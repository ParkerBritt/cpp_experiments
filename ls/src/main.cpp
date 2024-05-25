#include <iostream>
#include <filesystem>
#include <vector>
#include <unordered_map>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

namespace fs = std::filesystem;

using termColor = int[3];

std::string ansiColor(int r, int g, int b){
    return "\e[38;2;"+std::to_string(r)+";"+std::to_string(g)+";"+std::to_string(b)+"m";
}

std::string ansiColor(const termColor& color){
    return ansiColor(color[0], color[1], color[2]);
}


std::string getIcon(const std::unordered_map<std::string, std::string> iconNameMap,
        const std::unordered_map<std::string, std::string> extMap,
        const fs::path& curPath){
    std::string icon;
    std::string fileName = curPath.filename().string();
    if(iconNameMap.count(fileName) > 0){
        icon = iconNameMap.at(fileName);
        return icon;
    }
    if(std::filesystem::is_directory(curPath)){ // is dir
        if(std::filesystem::is_empty(curPath)){
            icon = "";
        }else{
            icon = "";
        }
        return icon;
    }
    std::string fileExt = curPath.extension().string();
    // check if file extension exists
    if(fileExt.length() > 0){
        fileExt = fileExt.substr(1);
        // check if match found in map
        if(extMap.count(fileExt) > 0){
            icon = extMap.at(fileExt);
            return icon;
        }
    }
    icon = "";
    return icon;
}

int main(int argc, char* argv[]){
    // def colors
    const termColor red = {255,0,0};

    int opt;

      while ((opt = getopt(argc, argv, "l")) != -1) {
        if (opt == 'l') {
            std::cout << "long mode" << std::endl;
        } else if (opt == ':') {
            std::cout << "Option requires a value" << std::endl;
        } else if (opt == '?') {
            std::cout<< "Unknown option: " << optopt << std::endl;
        }
      }

    // icon name map
    std::unordered_map<std::string, std::string> iconNameMap;
    iconNameMap["CMakeLists.txt"] = "";
    iconNameMap["Desktop"] = "";
    iconNameMap["Pictures"] = "󰉏";
    iconNameMap["Videos"] = "";
    iconNameMap["Games"] = "󰮂";
    iconNameMap["bin"] = "";

    // file extension map
    std::unordered_map<std::string, std::string> extMap;
    extMap["txt"] = "";
    extMap["sh"] = "";

    std::filesystem::path wd = std::filesystem::current_path();
    bool showHidden = false;
    std::vector<std::string> formattedFiles;
    for (std::filesystem::directory_entry const& dir_entry : std::filesystem::directory_iterator(wd)){
        const std::filesystem::path curPath = dir_entry.path();
        std::string fileName = curPath.filename().string();
        std::string icon = getIcon(iconNameMap, extMap, curPath);
        if(fileName.substr(0,1)=="."){ continue; }
        formattedFiles.push_back(icon+" "+fileName);
        // std::cout << icon << " " << fileName << sep;
    }
    size_t filesC = formattedFiles.size();
    char sep = filesC > 10 ? '\n' : ' ';
    std::string allFiles;
    for(int i=0; i<filesC; i++){
        allFiles+=formattedFiles[i]+sep;
    }
    std::cout << allFiles;
    return 0;

}
