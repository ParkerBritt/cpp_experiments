#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

size_t countDirSize(fs::path dirPath){
    size_t counter = 0;
    for (auto const& dir_entry : std::filesystem::directory_iterator(dirPath)){
        counter++;
    }
    return counter;
}

int main(){
    std::filesystem::path wd = std::filesystem::current_path();
    size_t dirSize = countDirSize(wd);
    char sep = dirSize > 10 ? '\n' : ' ';
    std::cout << dirSize << std::endl;
    bool showHidden = false;
    for (std::filesystem::directory_entry const& dir_entry : std::filesystem::directory_iterator(wd)){
        const std::filesystem::path curPath = dir_entry.path();
        std::string icon;
        std::string fileName = curPath.filename().string();
        if(fileName.substr(0,1)=="."){ continue; }
        if(std::filesystem::is_directory(curPath)){ // is dir
            if(std::filesystem::is_empty(curPath)){
                icon = "";
            }else{
                icon = "";
            }
        }
        else{ // is not dir
            if(fileName == "CMakeLists.txt"){
                icon = "";
            }
            else{
                icon = "";
            }
        }
        std::cout << icon << " " << fileName << sep;
    }
    return 0;

}
