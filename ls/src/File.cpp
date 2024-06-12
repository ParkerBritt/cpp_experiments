#include "File.hpp"

// -- constructors --
File::File(fs::path path){
    File::path = path;
    isSymLink = fs::is_symlink(path);
    if(isSymLink) symLinkPath = fs::read_symlink(path);
    fileName = path.filename().string();
}
File::File(std::string path){
    File(fs::path(path));
}
// -- end constructors --

// -- getters --
std::string File::getFileName(){
    if(fileName.empty()){
        setFileName();
    }
    return fileName;
}
std::string File::getFormattedLine(){
    std::string formattedLine = fileIconColor.getEscape()+fileIcon+" "+fileNameColor.getEscape()+fileName;
    if(showSymLink && isSymLink){
        formattedLine += symLinkIconColor.getEscape()+' '+symLinkIcon+' '+symLinkPathColor.getEscape()+fs::read_symlink(path).string();
    }
    return formattedLine;
}
size_t File::getLineLen(){
    return lineLen;
}

// -- end getters --

// -- setters --
void File::setFileName(){
    fileName = path.filename().string();
}

void File::setIcon(const std::unordered_map<std::string, std::vector<std::string>> iconNameMap,
    const std::unordered_map<std::string, std::vector<std::string>> extMap){

    // file name mapping
    if(iconNameMap.find(fileName) != iconNameMap.end()){
        fileIcon = iconNameMap.at(fileName)[0];
    }
    // is directory
    else if(std::filesystem::is_directory(path)){ // is dir
        if(std::filesystem::is_empty(path)){
            fileIcon = "";
        }else{
            fileIcon = "";
        }
    }
    else{
        std::string fileExt = path.extension().string();
        // check if file extension exists
        if(fileExt.length() > 0){
            fileExt = fileExt.substr(1);
            // check if match found in map
            if(extMap.count(fileExt) > 0){
                fileIcon = extMap.at(fileExt)[0];
            }
        }
        else{
            fileIcon = "";
        }
    }
    setLineLen();
}

void File::setLineLen(){
    size_t lineLenBuffer = fileIcon.size()+1+fileName.size();
    if(showSymLink && isSymLink){
        lineLenBuffer += 1+symLinkIcon.size()+1+symLinkPath.size();
    }
    lineLen = lineLenBuffer;
}
// -- end setters --
