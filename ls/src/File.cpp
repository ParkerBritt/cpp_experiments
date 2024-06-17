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

std::string File::getCompFileName(){
    std::string fileName = utils::tolower(getFileName());
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
    auto winSize = getWinSize();
    unsigned short winWidth = std::get<1>(winSize);
    size_t columns = 4;

    std::string returnBuffer;
    size_t iMax = filesVector.size();

    for(size_t i=0; i<iMax; i++){
        size_t fileIndex = i;
        // fileIndex = fileIndex>filesVector.size() ? filesVector.size()-1 : fileIndex;
        const File curFile = filesVector.at(fileIndex);

        // file contents
        returnBuffer += curFile.getFormattedLine();
        returnBuffer+= ' ';
        // returnBuffer+= '\t';
    }


    // // top border
    // if(showBorder){
    //     border = std::make_unique<Border>();
    //     size_t borderWidth=0;
    //     if(longMode){
    //         borderWidth = maxFileNameCnt;
    //     }
    //     else{
    //         for(size_t i=0; i<iMax; i++){
    //             const File curFile = filesVector[i];
    //             borderWidth+=curFile.getLineLen()+1;
    //             if(borderWidth>winWidth){
    //                 borderWidth = winWidth-2;
    //                 break;
    //             }

    //         }
    //     }
    //     border->setWidth(borderWidth);
    //     returnBuffer+=border->getTop();
    // }

    // if(showBorder && !longMode) returnBuffer += border->vertical;
    // int curLineLen = 0;
    // char sep = longMode ? '\n' : '\t';
    // bool isNewLine = false;
    // for(size_t i=0; i<iMax; i++){
    //     isNewLine=false;
    //     const File curFile = filesVector[i];
    //     size_t curFileLen = curFile.getLineLen();
    //     if(!longMode && curLineLen+curFileLen>winWidth){
    //         isNewLine=true;
    //         if(showBorder){
    //             returnBuffer+=border->vertical+"\n"+border->vertical;
    //             curLineLen=1;
    //         }
    //         else{
    //             returnBuffer+="\n";
    //             curLineLen=0;
    //         }
    //     }

    //     // left border
    //     if(showBorder && longMode) returnBuffer += border->vertical;
    //     
    //     // file contents
    //     returnBuffer += curFile.getFormattedLine();
    //     // curLineLen+= curFile.getLineLen();
    //     curLineLen += maxFileNameCnt;

    //     // right border
    //     if(showBorder && longMode){

    //         const size_t lineSize = curFile.getLineLen();
    //         const unsigned int spacerLen = (lineSize<border->getWidth()) ? border->getWidth()-lineSize : 0;
    //         std::string rightBorderBuffer;
    //         for(int i=0; i<spacerLen; i++){
    //             rightBorderBuffer+=" ";
    //         }
    //         rightBorderBuffer+=border->vertical;
    //         returnBuffer+=rightBorderBuffer;
    //     }

    //     // separator
    //     if(i!=iMax){
    //         returnBuffer+=sep;
    //         curLineLen++;
    //     }
    // }
    // if(showBorder){
    //     if(!longMode) returnBuffer+=border->vertical+"\n";
    //     returnBuffer+=border->getBottom();
    // }
    return returnBuffer;
}

void FileCollection::newFile(fs::path path){
    File file(path);
    file.setIcon(iconNameMap, iconExtMap);
    FileCollection::addFile(file);
}

void FileCollection::addFile(File file){
    std::string newFileName = file.getCompFileName();

    if(filesVector.size()==0){
        std::cout << "adding file: " << file.getCompFileName() << " to empty vector" << std::endl;
        filesVector.push_back(file);
    }
    else if(filesVector.size()==1){
        // if greater than existing item in vector
        if(newFileName > filesVector[0].getCompFileName()) {
            std::cout << "adding file: " << file.getCompFileName() << " after existing file " << filesVector[0].getCompFileName() << std::endl;
            filesVector.push_back(file);
        }
        else{
            std::cout << "adding file: " << file.getCompFileName() << " before existing file " << filesVector[0].getCompFileName() << std::endl;
            filesVector.insert(filesVector.begin(), file);
        }
    }
    else{
        bool foundPlace = false;
        if(newFileName < filesVector[0].getCompFileName()){
            std::cout << "adding " << file.getCompFileName() << " to start of vector" << std::endl;
            filesVector.insert(filesVector.begin(), file);
        }
        else{
            for(size_t i = 1; i<filesVector.size()-1; i++){
                std::string prevFileName = filesVector[i-1].getCompFileName();
                std::string nextFileName = filesVector[i].getCompFileName();

                if(prevFileName < newFileName && newFileName < nextFileName) {
                    std::cout << "adding file " << newFileName << " after " << prevFileName << std::endl;
                    std::cout << prevFileName << " < " << newFileName << " < " << nextFileName << std::endl;
                    std::cout << "prevFileName == " << prevFileName <<std::endl;
                    std::cout << "nextFileName == " << nextFileName <<std::endl;
                    std::cout << "string " << (prevFileName < newFileName && newFileName < nextFileName) << std::endl;
                    filesVector.insert(filesVector.begin()+i, file);
                    foundPlace = true;
                    break;
                }
            }
            if(!foundPlace){
                std::cout << "couldn't find spot adding file: " << file.getCompFileName() << " to end" << std::endl;
                filesVector.push_back(file);
            }
        }
    }
    std::cout << "   ";
    for(int i =0; i < filesVector.size(); i++){
        std::cout << filesVector[i].getFileName() << ' ';
    }
    std::cout << '\n' << std::endl;
    const size_t lineLen = file.getLineLen();
    if(lineLen>maxFileNameCnt) maxFileNameCnt = lineLen;
    filesCnt++;
}

size_t FileCollection::getCnt(){
    return filesCnt;
}
