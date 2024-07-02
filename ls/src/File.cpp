#include "File.hpp"
#include "AnsiUtils.hpp"
#include "Utils.hpp"
#include <csignal>
#include <iostream>
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
        if(std::filesystem::is_empty(path)){
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

// ---- start class FileCollection ----
FileCollection::FileCollection(std::shared_ptr<ConfigParser> configParser){
    iconNameMap = configParser->getSectionContents("Icon Name Mapping");
    iconExtMap = configParser->getSectionContents("Extension Mapping");
}

std::string FileCollection::getFormattedFiles(bool longMode, bool showBorder){
    auto winSize = getWinSize();
    unsigned short winWidth = std::get<1>(winSize);
    std::string returnBuffer;
    size_t totalItems = filesVector.size();
    int colPadding = 2;
    std::vector<size_t> fileLineLengths;
    size_t borderWidth = 0;
    
    // calculate file lengths
    for(size_t i = 0; i < totalItems; i++){
        size_t curFileLineLen = filesVector.at(i).getLineLen();
        fileLineLengths.push_back(curFileLineLen);
    }
    
    // calculate rows and collumns
    size_t columns = winWidth;
    size_t rows;
    bool overFlow = true;

    size_t debugIter = 0;
    while(overFlow){
        overFlow = false;
        rows = ceil(totalItems/static_cast<float>(columns));

        // figure out max length for each column
        std::vector<size_t> colMaxLengths; 
        for(size_t col = 0; col < columns; col++) {
            size_t colMaxLength = 0;
            for(size_t row = 0; row < rows; row++) {
                size_t fileIndex = row + col * rows;
                if (fileIndex >= totalItems) {
                    continue;
                }
                // size_t curLineLen = filesVector[fileIndex].getLineLen()+colPadding;
                size_t curLineLen = fileLineLengths.at(fileIndex)+colPadding;
                if(curLineLen >colMaxLength) colMaxLength = curLineLen;
            }
            // std::cout << "\tcolumn len: " << colMaxLength << std::endl;
            colMaxLengths.push_back(colMaxLength);
        }

        size_t rowLen = 0;
        size_t rowItems = 0;
        for(size_t col = 0; col < columns; col++){
            

            size_t colLineLen = colMaxLengths.at(col);
            overFlow = rowLen+colLineLen > winWidth;
            if(overFlow) break;

            rowLen += colLineLen;
            rowItems++;
        }

        if(!overFlow) break;
        columns=rowItems;

        debugIter++;
    }
    if(columns == 0 && rows == 0){
        columns = 1;
        rows = totalItems;
    }


    // fetch max length for each column for generating separator length 
    std::vector<size_t> colMaxLengths; 
    for(size_t col = 0; col < columns; col++) {
        size_t colMaxLength = 0;
        for(size_t row = 0; row < rows; row++) {
            size_t fileIndex = row + col * rows;
            if (fileIndex >= totalItems) {
                continue;
            }
            size_t curLineLen = filesVector[fileIndex].getLineLen();
            if(curLineLen >colMaxLength) colMaxLength = curLineLen;
        }
        colMaxLengths.push_back(colMaxLength);
        if(colMaxLength>0) borderWidth += colMaxLength+colPadding;
        // std::cout << "column: " << col << " max length: " << colMaxLength << std::endl;
    }

    if(borderWidth+2 > winWidth) borderWidth = winWidth-2;
    border->setWidth(borderWidth);
    returnBuffer += border->getTop();

    // start iterating through files and adding to buffer
    for(size_t row = 0; row < rows; row++) {
        for(size_t col = 0; col < columns; col++) {
            size_t fileIndex = row + col * rows;
            if (fileIndex >= totalItems) {
                continue;
            }
            File curFile = filesVector.at(fileIndex);
            returnBuffer += curFile.getFormattedLine();
            
            // add separator
            if(col<columns-1){
                std::string sep = "";
                size_t sepSize = colMaxLengths[col]-curFile.getLineLen();
                for(size_t i=0;i<sepSize+colPadding; i++){
                    sep+=' ';
                }
                returnBuffer+=sep;
            }
        }
        if(row<rows-1) returnBuffer += '\n';
    }
    return returnBuffer;


    size_t curLineCnt = 0;
    std::cout << std::endl;
    for(size_t i=0; i<totalItems; i++){
        if(curLineCnt>=columns){
            returnBuffer+='\n';
            curLineCnt = 0;
        }
        size_t col = i % columns;
        size_t row = i / columns;
        size_t fileIndex = row + col * rows;


        std::cout << "file index " << i << " = " << fileIndex << std::endl;
        std::cout << i/static_cast<float>(totalItems-1)*100 << "% through" << std::endl;;
        if(fileIndex >= totalItems){
            throw std::out_of_range("fileVector out of range at index: " + std::to_string(i) + " value: " + std::to_string(fileIndex) + " max value: " + std::to_string(totalItems));
        }
        const File curFile = filesVector.at(fileIndex);

        returnBuffer += curFile.getFormattedLine();
        returnBuffer+= ' ';
        curLineCnt++;
    }
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
        filesVector.push_back(file);
    }
    else if(filesVector.size()==1){
        // if greater than existing item in vector
        if(newFileName > filesVector[0].getCompFileName()) {
            filesVector.push_back(file);
        }
        else{
            filesVector.insert(filesVector.begin(), file);
        }
    }
    else{
        bool foundPlace = false;
        if(newFileName < filesVector[0].getCompFileName()){
            filesVector.insert(filesVector.begin(), file);
        }
        else{
            for(size_t i = 1; i<filesVector.size(); i++){
                std::string prevFileName = filesVector[i-1].getCompFileName();
                std::string nextFileName = filesVector[i].getCompFileName();

                if(prevFileName < newFileName && newFileName < nextFileName) {
                    filesVector.insert(filesVector.begin()+i, file);
                    foundPlace = true;
                    break;
                }
            }
            if(!foundPlace){
                //std::cout << "couldn't find spot adding file: " << file.getCompFileName() << " to end" << std::endl;
                filesVector.push_back(file);
            }
        }
    }
    const size_t lineLen = file.getLineLen();
    if(lineLen>maxFileNameCnt) maxFileNameCnt = lineLen;
    filesCnt++;
}

size_t FileCollection::getCnt(){
    return filesCnt;
}
