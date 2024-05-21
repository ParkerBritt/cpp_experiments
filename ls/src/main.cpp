#include <iostream>
#include <filesystem>
#include <vector>
namespace fs = std::filesystem;
std::string ansiColor(int r, int g, int b){
    return "\e[38;2;"+std::to_string(r)+";"+std::to_string(g)+";"+std::to_string(g)+"m";
}

int main(){
    std::filesystem::path wd = std::filesystem::current_path();
    bool showHidden = false;
    std::vector<std::string> formattedFiles;
    for (std::filesystem::directory_entry const& dir_entry : std::filesystem::directory_iterator(wd)){
        const std::filesystem::path curPath = dir_entry.path();
        std::string icon;
        std::string fileName = curPath.filename().string();
        if(fileName.substr(0,1)=="."){ continue; }
        if(std::filesystem::is_directory(curPath)){ // is dir
            if(std::filesystem::is_empty(curPath)){
                icon = "";
            }else{
                icon = ansiColor(255,0,0)+""+ansiColor(200,0,0);
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
        formattedFiles.push_back(icon+" "+fileName);
        // std::cout << icon << " " << fileName << sep;
    }
    size_t filesC = formattedFiles.size();
    char sep = filesC > 10 ? '\n' : ' ';
    std::string allFiles;
    for(int i=0; i<filesC; i++){
        allFiles+=formattedFiles[i]+sep;
    }
    std::cout << allFiles;
    return 0;

}
