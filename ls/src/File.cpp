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
    //                     v using 1 as a standin for icon size for now, fix later
    size_t lineLenBuffer = 1+1+fileName.size();
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
    size_t iMax = filesVector.size();
    if(showBorder){
        border = std::make_unique<Border>();
        size_t borderWidth=0;
        if(longMode){
            borderWidth = maxFileNameCnt;
        }
        else{
            for(size_t i=0; i<iMax; i++){
                // std::cout << '1';
                const File curFile = filesVector[i];
                borderWidth+=curFile.getLineLen()+1;

            }
        }
        border->setWidth(borderWidth);
        returnBuffer+=border->getTop();
    }
    if(showBorder && !longMode) returnBuffer += border->vertical;
    char sep = longMode ? '\n' : ' ';
    for(size_t i=0; i<iMax; i++){
        // left border
        if(showBorder && longMode) returnBuffer += border->vertical;
        
        // file contents
        const File curFile = filesVector[i];
        returnBuffer += curFile.getFormattedLine();

        // right border
        if(showBorder && longMode){

            const size_t lineSize = curFile.getLineLen();
            const unsigned int spacerLen = (lineSize<border->getWidth()) ? border->getWidth()-lineSize : 0;
            std::string rightBorderBuffer;
            for(int i=0; i<spacerLen; i++){
                rightBorderBuffer+=" ";
            }
            rightBorderBuffer+=border->vertical;
            returnBuffer+=rightBorderBuffer;
        }

        // separator
        if(i!=iMax) returnBuffer+=sep;
    }
    if(showBorder){
        if(!longMode) returnBuffer+=border->vertical+"\n";
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
