#include <iostream>
#include <filesystem>
#include <vector>
#include <unordered_map>

namespace fs = std::filesystem;

using termColor = int[3];

std::string ansiColor(int r, int g, int b){
    return "\e[38;2;"+std::to_string(r)+";"+std::to_string(g)+";"+std::to_string(b)+"m";
}

std::string ansiColor(const termColor& color){
    return ansiColor(color[0], color[1], color[2]);
}

std::string ansiReset();


std::string getIcon(const std::unordered_map<std::string, std::string> iconNameMap,
        const std::unordered_map<std::string, std::string> extMap,
        const fs::path& curPath){
    std::string icon;
    std::string fileName = curPath.filename().string();
    if(iconNameMap.count(fileName) > 0){
        icon = iconNameMap.at(fileName);
        return icon;
    }
    if(std::filesystem::is_directory(curPath)){ // is dir
        if(std::filesystem::is_empty(curPath)){
            icon = "";
        }else{
            icon = "";
        }
        return icon;
    }
    std::string fileExt = curPath.extension().string();
    // check if file extension exists
    if(fileExt.length() > 0){
        fileExt = fileExt.substr(1);
        // check if match found in map
        if(extMap.count(fileExt) > 0){
            icon = extMap.at(fileExt);
            return icon;
        }
    }
    icon = "";
    return icon;
}

int main(int argc, char* argv[]){
    // def colors
    const termColor red = {255,0,0};

    bool long_flag = false;

    int optind = 1;
    for(optind; optind<argc; optind++){
        const std::string opt = argv[optind];
        if(opt[0]!='-'){ // break if no flag detected
            // std::cout << "break " << argc << opt << std::endl;
            // std::cout << opt.substr(1,0) << std::endl;
            break;
        }
        else if(opt[0]=='-' && opt[1]!='-'){
            for(int i=1; i<opt.size(); i++){
                switch(opt[i]){
                    case 'l':
                        std::cout << "long mode" << std::endl;
                        long_flag = true;
                        break;
                    case 'h':
                        std::cout << "help" << std::endl;
                }
            }
        }
        else if(opt == "--columns"){
            int column_cnt = atoi(argv[optind+1]); 
            std::cout << "COLUMNS: " << column_cnt << std::endl;
            optind++;
            continue;
        }
        else if(opt == "--rows"){
            int row_cnt = atoi(argv[optind+1]);
            std::cout << "ROWS: " << row_cnt << std::endl;
            optind++;
            continue;
        }
        else{
            std::cout << ansiColor(255,0,0) << "Unkown option: " << opt << std::endl;
            return 0;
        }
    }

    std::string file_path;
    if (optind < argc) {
        // check if file exists
        file_path = argv[optind];
        if(!fs::exists(file_path)){
            std::cout << ansiColor(255,0,0) << '"' << file_path << '"' << ": No such file or directory" << std::endl;
            return 0;
        }
    }
    // std::cout << "file path:" << file_path << std::endl;
    // icon name map
    std::unordered_map<std::string, std::string> iconNameMap;
    iconNameMap["CMakeLists.txt"] = "";
    iconNameMap["Desktop"] = "";
    iconNameMap["Pictures"] = "󰉏";
    iconNameMap["Videos"] = "";
    iconNameMap["Games"] = "󰮂";
    iconNameMap["bin"] = "";

    // file extension map
    std::unordered_map<std::string, std::string> extMap;
    extMap["txt"] = "";
    extMap["sh"] = "";

    std::filesystem::path wd;
    if(file_path.empty()){
        wd = fs::current_path();
    }else{
        wd = fs::path(file_path);
    }

    bool showHidden = false;
    std::vector<std::string> formattedFiles;
    for (std::filesystem::directory_entry const& dir_entry : std::filesystem::directory_iterator(wd)){
        const std::filesystem::path curPath = dir_entry.path();
        std::string fileName = curPath.filename().string();
        std::string icon = getIcon(iconNameMap, extMap, curPath);
        if(fileName.substr(0,1)=="."){ continue; }
        formattedFiles.push_back(icon+" "+fileName);
        // std::cout << icon << " " << fileName << sep;
    }
    size_t filesC = formattedFiles.size();
    int long_mode = filesC > 10 || long_flag;
    char sep = long_mode ? '\n' : ' ';
    std::string allFiles;
    for(int i=0; i<filesC; i++){
        allFiles+=formattedFiles[i]+sep;
    }
    std::cout << allFiles;
    return 0;

}