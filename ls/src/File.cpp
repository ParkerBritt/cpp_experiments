#include "File.hpp"
#include "AnsiUtils.hpp"
#include "Utils.hpp"
#include <csignal>
#include <stdexcept>
#include <string>
#include <tuple>
#include <optional>

// ---- Class File ----
// -- constructors --
File::File(fs::path path){
    File::path = path;
    if(!fs::exists(path)){
        throw fs::filesystem_error("File does not exist ", std::make_error_code(std::errc::no_such_file_or_directory));
    }
    isSymLink = fs::is_symlink(path);
    if(isSymLink) symLinkPath = fs::read_symlink(path);
    setFileName();
    setLineLen();
}
File::File(std::string path) : File(fs::path(path)){
}
// -- end constructors --

// -- getters --
std::string File::getFileName(){
    if(fileName.empty()){
        setFileName();
    }
    return fileName;
}

std::string File::getCompFileName(){
    std::string fileName = utils::tolower(getFileName());
    return fileName;
}

std::string File::getFormattedLine() const{
    std::string formattedLine = fileIconColor.getEscape()+fileIcon+" "/* +fileNameColor.getEscape() */+fileName;
    if(showSymLink && isSymLink){
        formattedLine += symLinkIconColor.getEscape()+' '+symLinkIcon+' '+symLinkPathColor.getEscape()+fs::read_symlink(path).string();
    }
    return formattedLine;
}
size_t File::getLineLen() const{
    return lineLen;
}

// -- end getters --

// -- setters --
void File::setFileName(){
    fileName = path.filename().string();
}

std::optional<std::tuple<std::string, std::string>> File::parseConfigIconMap(const std::unordered_map<std::string, std::vector<std::string>> configMap, std::string key){
    // search through icons in the given map and extract the icon and the color of the icon
    // returns icon and the color of the icon as an escape sequence if successful and nullopt if not found

    bool foundIcon = configMap.find(key) != configMap.end();
    if(!foundIcon) return std::nullopt;

    std::string iconColorEscape = "";
    std::string icon = "";

    std::vector<std::string> configValues = configMap.at(key);
    icon = configValues[0];

    if(configValues.size()>1){
        std::string rawConfigColor = configValues[1];

        // get r,g,b components
        if(rawConfigColor.size()>11){
            throw std::runtime_error("Config read fail: color value too long for: " + key);
        }

        int commaPos = rawConfigColor.find(',');
        int r = atoi(rawConfigColor.substr(0, commaPos).c_str());
        int nextCommaPos = rawConfigColor.find(',', commaPos+1);
        int g = atoi(rawConfigColor.substr(commaPos+1, nextCommaPos-commaPos).c_str());
        int b = atoi(rawConfigColor.substr(nextCommaPos+1, rawConfigColor.size()-1-nextCommaPos).c_str());
        iconColorEscape = AnsiUtils::Color(r,g,b).getEscape();
    }

    return std::make_tuple(icon, iconColorEscape);
}

void File::setIcon(const std::unordered_map<std::string, std::vector<std::string>> iconNameMap,
    const std::unordered_map<std::string, std::vector<std::string>> extMap){

    // file name mapping
    std::string iconColorEscape = "";
    auto fileNameValues = parseConfigIconMap(iconNameMap, fileName);
    if(fileNameValues){
        fileIcon = std::get<0>(*fileNameValues);
        iconColorEscape = std::get<1>(*fileNameValues);
    }

    // is directory
    else if(std::filesystem::is_directory(path)){ // is dir
        bool isDirEmpty;

        try{
            isDirEmpty = std::filesystem::is_empty(path);
        }
        catch (const std::filesystem::filesystem_error& e){
            isDirEmpty = false;
        }

        if(isDirEmpty){
            fileIcon = "";
        }else{
            fileIcon = "";
        }
    }
    else{
        std::string fileExt = path.extension().string();
        if(fileExt.length() > 0){
            fileExt = fileExt.substr(1);
        }

        auto fileExtValues = parseConfigIconMap(extMap, fileExt);
        if(fileExtValues){
            fileIcon = std::get<0>(*fileExtValues);
            iconColorEscape = std::get<1>(*fileExtValues);
        }

    }
    fileIcon = iconColorEscape + fileIcon;
    setLineLen();
}

void File::setLineLen(){
    //                     v using 1 as a standin for icon size for now, fix later
    size_t lineLenBuffer = 1+1+fileName.size();
    if(showSymLink && isSymLink){
        lineLenBuffer += 1+symLinkIcon.size()+1+symLinkPath.size();
    }
    lineLen = lineLenBuffer;
}
// -- end setters --
// ---- end class File ----

