#include <iostream>
#include <filesystem>
#include <vector>
#include <unordered_map>
#include "ArgParsing.hpp"
#include "AnsiUtils.hpp"

namespace fs = std::filesystem;





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


void displayHelp(){
    std::cout << "Help Placeholder" << std::endl;
}

int main(int argc, char* argv[]){
    // def colors
    const AnsiUtils::colorVector red = {255,0,0};

    ArgumentParser argParser = ArgumentParser();
    bool long_flag = false;
    argParser.addArgument('l', argParser.Bool);
    if(!argParser.parseArgs(argc, argv)){
        return -1;
    }
    long_flag = argParser.getArgVal('l');
    std::cout << "FLAG: " << long_flag << std::endl;

    std::string file_path;
    // if (optind < argc) {
    //     // check if file exists
    //     file_path = argv[optind];
    //     if(!fs::exists(file_path)){
    //         std::cout << ansiColor(255,0,0) << '"' << file_path << '"' << ": No such file or directory" << std::endl;
    //         return 0;
    //     }
    // }
    // std::cout << "file path:" << file_path << std::endl;
    // icon name map
    std::unordered_map<std::string, std::string> iconNameMap;
    iconNameMap["CMakeLists.txt"] = "";
    iconNameMap["Desktop"] = "";
    iconNameMap["Pictures"] = "󰉏";
    iconNameMap["Videos"] = "";
    iconNameMap["Games"] = "󰮂";
    iconNameMap["bin"] = "";
    iconNameMap["Projects"] = "";

    // file extension map
    std::unordered_map<std::string, std::string> extMap;
    extMap["txt"] = "";
    extMap["sh"] = "";

    std::filesystem::path wd;
    if(file_path.empty()){
        wd = fs::current_path();
    }else{
        wd = fs::path(file_path);
    }

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
    int long_mode = filesC > 10 || long_flag;
    char sep = long_mode ? '\n' : ' ';
    std::string allFiles;
    for(int i=0; i<filesC; i++){
        allFiles+=formattedFiles[i]+sep;
    }
    std::cout << allFiles;
    return 0;

}
