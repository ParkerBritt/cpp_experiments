#include <iostream>
#include <filesystem>
#include <vector>
#include <unordered_map>
namespace fs = std::filesystem;

using termColor = int[3];

std::string ansiColor(int r, int g, int b){
    return "\e[38;2;"+std::to_string(r)+";"+std::to_string(g)+";"+std::to_string(b)+"m";
}

std::string ansiColor(const termColor& color){
    return ansiColor(color[0], color[1], color[2]);
}

std::string getIcon(const std::unordered_map<std::string, std::string> iconNameMap, const fs::path& curPath){
    std::string icon;
    std::string fileName = curPath.filename().string();
    if(std::filesystem::is_directory(curPath)){ // is dir
        if(std::filesystem::is_empty(curPath)){
            icon = "";
        }else{
            icon = "";
        }
    }
    else{ // is not dir
        if(iconNameMap.count(fileName) > 0){
            icon = iconNameMap.at(fileName);
        }
        else{
            icon = "";
        }
    }
    return icon;
}

int main(){
    // def colors
    const termColor red = {255,0,0};

    // icon name map
    std::unordered_map<std::string, std::string> iconNameMap;
    iconNameMap["CMakeLists.txt"] = "";

    std::filesystem::path wd = std::filesystem::current_path();
    bool showHidden = false;
    std::vector<std::string> formattedFiles;
    for (std::filesystem::directory_entry const& dir_entry : std::filesystem::directory_iterator(wd)){
        const std::filesystem::path curPath = dir_entry.path();
        std::string fileName = curPath.filename().string();
        std::string icon = getIcon(iconNameMap, curPath);
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
