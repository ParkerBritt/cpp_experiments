#include <iostream>
#include <filesystem>

int main(){
    std::filesystem::path wd = std::filesystem::current_path();
    for (std::filesystem::directory_entry const& dir_entry : std::filesystem::directory_iterator(wd)){
        const std::filesystem::path curPath = dir_entry.path();
        std::string icon;
        if(std::filesystem::is_directory(curPath)){ // is dir
            if(std::filesystem::is_empty(curPath)){
                icon = "";
            }else{
                icon = "";
            }
        }
        else{ // is not dir
            icon = "";
        }
        std::cout << icon << " " << curPath.filename().string() << " ";
    }
    return 0;

}
