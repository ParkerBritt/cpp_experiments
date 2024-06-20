#include <iostream>
#include <filesystem>
#include "ArgParsing.hpp"
#include "AnsiUtils.hpp"
#include "configParsing.hpp"
#include "File.hpp"
#include <memory>

namespace fs = std::filesystem;

int main(int argc, char* argv[]){
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
        std::cout << "Failed arg parsing" << std::endl;
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
    std::shared_ptr<ConfigParser> configParserPtr = std::make_unique<ConfigParser>(configPath);
    FileCollection fileCollection(configParserPtr);

    std::optional<std::string> filePath = argParser.getArgVal<std::string>("dirPath");

    std::filesystem::path wd;
    if(!filePath){
        std::cout << "setting current path" << std::endl;
        wd = fs::current_path();
    }else{
        if(!fs::exists(*filePath)){
            std::cout << AnsiUtils::color(255,0,0) << '"' << *filePath << '"' << ": No such file or directory" << std::endl;
            return -1;
        }
        wd = fs::path(*filePath);
    }

    for (std::filesystem::directory_entry const& dir_entry : std::filesystem::directory_iterator(wd)){
        const std::filesystem::path curPath = dir_entry.path();
        fileCollection.newFile(curPath);
    }

    std::cout << fileCollection.getFormattedFiles(*flagLong, *flagBorder) << std::endl;

    return 0;

}
