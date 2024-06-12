#include <iostream>
#include <filesystem>
#include <vector>
#include <unordered_map>
#include <regex>
#include "ArgParsing.hpp"
#include "AnsiUtils.hpp"
#include "configParsing.hpp"
#include "border.hpp"
#include <algorithm>
#include "File.hpp"

namespace fs = std::filesystem;

std::string getIcon(const std::unordered_map<std::string, std::vector<std::string>> iconNameMap,
        const std::unordered_map<std::string, std::vector<std::string>> extMap,
        const fs::path& curPath){
    std::string icon;
    std::string fileName = curPath.filename().string();
    if(iconNameMap.count(fileName) > 0){
        icon = iconNameMap.at(fileName)[0];
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
            icon = extMap.at(fileExt)[0];
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
    std::unique_ptr<Border> border(nullptr);
    AnsiUtils::ColorTheme colorTheme;
    colorTheme.add("symLink", 255, 255, 255);
    colorTheme.add("symLinkArrow", 255, 255, 255);
    // colorTheme.add("iconDefault", 255, 255, 255);
    colorTheme.add("textDefault", 255, 255, 255);

    // def colors
    const AnsiUtils::colorVector red = {255,0,0};

    // -- start arg parsing --
    // create arg parser
    ArgumentParser argParser = ArgumentParser();

    // add arguments
    argParser.addArgument('l', argParser.Bool);
    argParser.addArgument('a', argParser.Bool);
    argParser.addArgument('c', argParser.String);
    argParser.addArgument('s', argParser.Bool);
    argParser.addArgument('b', argParser.Bool);
    argParser.addArgument("dirPath", argParser.Positional);

    // parse args
    if(!argParser.parseArgs(argc, argv)){
        return -1;
    }

    // read bool flags
    std::optional<bool> flagLong = argParser.getArgVal<bool>('l');
    std::optional<bool> flagShowAll = argParser.getArgVal<bool>('a');
    std::optional<bool> flagSymlink = argParser.getArgVal<bool>('s');
    std::optional<bool> flagBorder = argParser.getArgVal<bool>('b');

    // read token arguments
    std::optional<std::string> configPathArg = argParser.getArgVal<std::string>('c');
    // -- end arg parsing --

    // -- start config parsing --
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
    auto iconNameMap = configParser.getSectionContents("Icon Name Mapping");

    auto colorThemeConfig = configParser.getSectionContents("Color Theme");
    colorTheme.add("iconDefault", std::stoi(colorThemeConfig["iconDefault"][0]), std::stoi(colorThemeConfig["iconDefault"][1]), std::stoi(colorThemeConfig["iconDefault"][2]));

    // file extension map
    auto extMap = configParser.getSectionContents("Extension Mapping");

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
    int longestFilename=0;
    std::vector<File> formattedFiles;
    size_t filesCnt = 0;
    for (std::filesystem::directory_entry const& dir_entry : std::filesystem::directory_iterator(wd)){
        std::string formatedLine;
        const std::filesystem::path curPath = dir_entry.path();
        File file(curPath);
        file.setIcon(iconNameMap, extMap);
        int lineLen = file.getLineLen();
        if(lineLen>longestFilename){
            longestFilename=lineLen;
            std::cout << formatedLine << " > " << longestFilename << std::endl;
        }
        formattedFiles.push_back(file);
        filesCnt++;
    }
    int long_mode = filesCnt > 10 || *flagLong;
    char sep = long_mode ? '\n' : ' ';
    std::string allFiles;


    if(*flagBorder){
        border = std::make_unique<Border>();
        border->setWidth(longestFilename);
        allFiles+=border->getTop();
    }
    {
        int i=0;
        for(i; i<filesCnt; i++){
            // border left side
            std::string line;
            if(*flagBorder){
                allFiles+=border->vertical;
            }
            allFiles+=formattedFiles[i].getFormattedLine();
            // border right side
            // if(*flagBorder){
            //     const unsigned int spacerLen = (line.size()<border->getWidth()) ? border->getWidth()-line.size() : 0;
            //     std::string rightBorderBuffer;
            //     std::cout << "spacerLen: " << spacerLen << std::endl;
            //     std::cout << "borderWidth: " << border->getWidth() << std::endl;
            //     std::cout << "lineLen: " << line.size() << std::endl;
            //     for(int i=0; i<spacerLen; i++){
            //         std::cout << "adding" << std::endl;
            //         rightBorderBuffer+=" ";
            //     }
            //     rightBorderBuffer+=border->vertical;
            // }
            if(i<filesCnt-1){
                allFiles+=sep;
            }
        }
    }

    std::cout << allFiles << std::endl;
    return 0;

}
