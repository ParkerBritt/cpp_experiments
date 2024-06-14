#include "File.hpp"

// ---- Class File ----
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
std::string File::getFormattedLine() const{
    std::string formattedLine = fileIconColor.getEscape()+fileIcon+" "+fileNameColor.getEscape()+fileName;
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
// ---- end class File ----

// ---- start class FileCollection ----
FileCollection::FileCollection(std::shared_ptr<ConfigParser> configParser){
    iconNameMap = configParser->getSectionContents("Icon Name Mapping");
    iconExtMap = configParser->getSectionContents("Extension Mapping");
}

std::string FileCollection::getFormattedFiles(bool longMode, bool showBorder){
    std::string returnBuffer;
    if(showBorder){
        border = std::make_unique<Border>();
        border->setWidth(maxFileNameCnt);
        returnBuffer+=border->getTop();
    }
    char sep = longMode ? '\n' : ' ';
    size_t iMax = filesVector.size();
    for(size_t i=0; i<iMax; i++){
        // left border
        if(showBorder) returnBuffer += border->vertical;
        
        // file contents
        const File curFile = filesVector[i];
        returnBuffer += curFile.getFormattedLine();

        // right border
        if(showBorder){

            const size_t lineSize = curFile.getLineLen();
            const unsigned int spacerLen = (lineSize<border->getWidth()) ? border->getWidth()-lineSize : 0;
            std::string rightBorderBuffer;
            std::cout << "spacerLen: " << spacerLen << std::endl;
            std::cout << "borderWidth: " << border->getWidth() << std::endl;
            std::cout << "lineLen: " << lineSize << std::endl;
            for(int i=0; i<spacerLen; i++){
                std::cout << "adding" << std::endl;
                rightBorderBuffer+=" ";
            }
            rightBorderBuffer+=border->vertical;
            returnBuffer+=rightBorderBuffer;
        }

        // separator
        if(i!=iMax) returnBuffer+=sep;
    }
    if(showBorder){
        returnBuffer+=border->getBottom();
    }
    return returnBuffer;
}

void FileCollection::newFile(fs::path path){
    File file(path);
    file.setIcon(iconNameMap, iconExtMap);
    FileCollection::addFile(file);
}

void FileCollection::addFile(File file){
    filesVector.push_back(file);
    const size_t lineLen = file.getLineLen();
    if(lineLen>maxFileNameCnt) maxFileNameCnt = lineLen;
    filesCnt++;
}

size_t FileCollection::getCnt(){
    return filesCnt;
}
