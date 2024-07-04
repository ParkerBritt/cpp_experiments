#include "FileCollection.hpp"
#include "File.hpp"
#include "Utils.hpp"
#include <csignal>
#include <iostream>
#include <stdexcept>
#include <string>

FileCollection::FileCollection(std::shared_ptr<ConfigParser> configParser){
    iconNameMap = configParser->getSectionContents("Icon Name Mapping");
    iconExtMap = configParser->getSectionContents("Extension Mapping");
}

std::string FileCollection::getFormattedFiles(bool longMode, bool showBorder, bool showHiddenFiles){
    auto winSize = getWinSize();
    unsigned short winWidth = std::get<1>(winSize);
    std::string returnBuffer;

    // create a new vector of files
    std::vector<std::reference_wrapper<File>> displayFilesVector;
    for(int i = 0; i<filesVector.size(); i++){
        File& file = filesVector.at(i);
        if(!showHiddenFiles && file.getFileName()[0]=='.'){
            continue;
        }
        displayFilesVector.push_back(file);
    }
    

    size_t totalItems = displayFilesVector.size();
    int colPadding = 2;
    std::vector<size_t> fileLineLengths;
    size_t borderWidth = 0;
    
    // calculate file lengths
    for(size_t i = 0; i < totalItems; i++){
        size_t curFileLineLen = displayFilesVector.at(i).get().getLineLen();
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
                // size_t curLineLen = displayFilesVector[fileIndex].getLineLen()+colPadding;
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
    // remove blank columns
    columns = ceil(static_cast<float>(totalItems)/rows);


    // fetch max length for each column for generating separator length 
    std::vector<size_t> colMaxLengths; 
    for(size_t col = 0; col < columns; col++) {
        size_t colMaxLength = 0;
        for(size_t row = 0; row < rows; row++) {
            size_t fileIndex = row + col * rows;
            if (fileIndex >= totalItems) {
                continue;
            }
            size_t curLineLen = displayFilesVector[fileIndex].get().getLineLen();
            if(curLineLen >colMaxLength) colMaxLength = curLineLen;
        }
        colMaxLengths.push_back(colMaxLength);
        if(colMaxLength>0) borderWidth += colMaxLength+colPadding;
        // std::cout << "column: " << col << " max length: " << colMaxLength << std::endl;
    }

    if(showBorder){
        if(borderWidth+2 > winWidth) borderWidth = winWidth-2;
        border->setWidth(borderWidth);
        returnBuffer += border->getTop();
    }

    // start iterating through files and adding to buffer
    for(size_t row = 0; row < rows; row++) {
        std::string rowBuffer = "";
        size_t rowLen = 0;

        if(showBorder) rowBuffer += border->vertical; // border left side

        rowLen += 1; // count up
        for(size_t col = 0; col < columns; col++) {
            size_t fileIndex = row + col * rows;
            if (fileIndex >= totalItems) {
                continue;
            }
            File curFile = displayFilesVector.at(fileIndex);
            rowBuffer += curFile.getFormattedLine();
            rowLen += curFile.getLineLen(); // count up
            
            // add separator
            if(col<columns-1){
                std::string sep = "";
                size_t sepSize = colMaxLengths[col]-curFile.getLineLen();
                for(size_t i=0;i<sepSize+colPadding; i++){
                    sep+=' ';
                }
                rowBuffer+=sep;
                rowLen += sepSize+colPadding; // count up
            }
        }

        if(showBorder){
            int spacerCnt = borderWidth - rowLen+1;
            for(int i=0; i<spacerCnt; i++){
                rowBuffer+=' ';
            }
            rowBuffer+= border->borderColor.getEscape() + border->vertical; // border right side
        }

        if(row<rows-1){
            rowBuffer += '\n';
        }
        returnBuffer += rowBuffer;
    }

    if(showBorder) returnBuffer += '\n'+border->getBottom(); // border bottom

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
        const File curFile = displayFilesVector.at(fileIndex);

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
