#include <iostream>
#include <filesystem>
#include <vector>
#include <unordered_map>
#include <regex>
#include "ArgParsing.hpp"
#include "AnsiUtils.hpp"
#include "configParsing.hpp"

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

struct ColorTheme{
    AnsiUtils::Color symLink = AnsiUtils::Color(0,0,255);
};

int main(int argc, char* argv[]){
    ColorTheme colorTheme;

    // def colors
    const AnsiUtils::colorVector red = {255,0,0};

    ArgumentParser argParser = ArgumentParser();

    argParser.addArgument('l', argParser.Bool);
    argParser.addArgument('a', argParser.Bool);
    argParser.addArgument('c', argParser.String);
    argParser.addArgument('s', argParser.Bool);
    argParser.addArgument("dirPath", argParser.Positional);

    if(!argParser.parseArgs(argc, argv)){
        return -1;
    }

    std::optional<bool> flagLong = argParser.getArgVal<bool>('l');
    std::optional<bool> flagShowAll = argParser.getArgVal<bool>('a');
    std::optional<bool> flagSymlink = argParser.getArgVal<bool>('s');

    std::optional<std::string> configPathArg = argParser.getArgVal<std::string>('c');
    std::string configPath;
    if(configPathArg){
        configPath = *configPathArg;
    }
    else{
        configPath = std::string(std::getenv("HOME"))+"/.config/pls.config";
    }
    ConfigParser configParser = ConfigParser(configPath);

    std::optional<std::string> filePath = argParser.getArgVal<std::string>("dirPath");
    // std::cout << "file path:" << file_path << std::endl;
    // icon name map
    std::unordered_map<std::string, std::string> iconNameMap = configParser.getSectionContents("Icon Name Mapping");

    // file extension map
    std::unordered_map<std::string, std::string> extMap = configParser.getSectionContents("Extension Mapping");

    std::filesystem::path wd;
    if(!filePath){
        wd = fs::current_path();
    }else{
        if(!fs::exists(*filePath)){
            std::cout << AnsiUtils::color(255,0,0) << '"' << *filePath << '"' << ": No such file or directory" << std::endl;
            return -1;
        }
        wd = fs::path(*filePath);
    }

    bool showHidden = false;
    std::vector<std::string> formattedFiles;
    for (std::filesystem::directory_entry const& dir_entry : std::filesystem::directory_iterator(wd)){
        std::string formatedLine;
        const std::filesystem::path curPath = dir_entry.path();
        std::string fileName = curPath.filename().string();
        std::string icon = getIcon(iconNameMap, extMap, curPath);
        if(fileName[0]=='.'){ continue; }
        formatedLine = icon+" "+fileName;
        if(*flagSymlink && fs::is_symlink(curPath)){
            formatedLine+="  "+colorTheme.symLink.getEscape()+fs::read_symlink(curPath).string()+AnsiUtils::reset();
        }
        formattedFiles.push_back(formatedLine);
    }
    size_t filesC = formattedFiles.size();
    int long_mode = filesC > 10 || flagLong;
    char sep = long_mode ? '\n' : ' ';
    std::string allFiles;

    {
        int i=0;
        for(i; i<filesC-1; i++){
            allFiles+=formattedFiles[i]+sep;
        }
        // last file without sep
        allFiles+=formattedFiles[i];
    }

    std::cout << allFiles << std::endl;
    return 0;

}
