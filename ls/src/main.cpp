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
#include <memory>

namespace fs = std::filesystem;

void displayHelp(){
    std::cout << "Help Placeholder" << std::endl;
}

int main(int argc, char* argv[]){
    // File myFile(fs::path("/home/parker/foo.txt"));
    // std::cout << myFile.getFormattedLine();
    // return 0;





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
    // ConfigParser configParser = ConfigParser(configPath);
    std::shared_ptr<ConfigParser> configParserPtr = std::make_unique<ConfigParser>(configPath);
    FileCollection fileCollection(configParserPtr);

    std::optional<std::string> filePath = argParser.getArgVal<std::string>("dirPath");
    // std::cout << "file path:" << file_path << std::endl;
    // icon name map
    // auto iconNameMap = configParserPtr->getSectionContents("Icon Name Mapping");

    // auto colorThemeConfig = configParserPtr->getSectionContents("Color Theme");
    // colorTheme.add("iconDefault", std::stoi(colorThemeConfig["iconDefault"][0]), std::stoi(colorThemeConfig["iconDefault"][1]), std::stoi(colorThemeConfig["iconDefault"][2]));

    // file extension map
    // auto extMap = configParserPtr->getSectionContents("Extension Mapping");

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
    for (std::filesystem::directory_entry const& dir_entry : std::filesystem::directory_iterator(wd)){
        std::string formatedLine;
        const std::filesystem::path curPath = dir_entry.path();
        File file(curPath);
        // file.setIcon(iconNameMap, extMap);
        int lineLen = file.getLineLen();
        if(lineLen>longestFilename){
            longestFilename=lineLen;
            std::cout << formatedLine << " > " << longestFilename << std::endl;
        }
        fileCollection.newFile(curPath);
        // fileCollection.addFile(file);
        // formattedFiles.push_back(file);
    }
    bool long_mode = fileCollection.getCnt() > 10 || *flagLong;
    char sep = long_mode ? '\n' : ' ';
    std::string allFiles;


    allFiles+=fileCollection.getFormattedFiles(long_mode, *flagBorder);
    // {
    //     int i=0;
    //     for(i; i<fileCollection.getCnt(); i++){
            // border left side
            // std::string line;
            // if(*flagBorder){
            //     allFiles+=border->vertical;
            // }
            // allFiles+=formattedFiles[i].getFormattedLine();
            // border right side
            // if(*flagBorder){
            //     std::string foo = "";
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
    //         if(i<fileCollection.getCnt()-1){
    //             allFiles+=sep;
    //         }
    //     }
    // }

    std::cout << allFiles << std::endl;
    return 0;

}
