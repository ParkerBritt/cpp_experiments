// ftxui
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

// boost
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/case_conv.hpp>


#include <fstream>
#include <string>
#include <unordered_set>
#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <stdexcept>

namespace ui = ftxui;
namespace bfs = boost::filesystem;

std::vector<bfs::path> getDesktopFiles(std::string dataDirsRaw){
    // split env var into vector
    std::vector<std::string> dataDirsSplit;
    boost::split(dataDirsSplit, dataDirsRaw, boost::is_any_of(":"));

    // filter to only unique data dirs
    std::vector<std::string> uniqueDataDirs;
    std::unordered_set<std::string> seenDataDirs;
    for(auto dataDir : dataDirsSplit){
        if(seenDataDirs.find(dataDir) == seenDataDirs.end()){
            // std::cout << "seenDataDirs: " << dataDir << std::endl;
            seenDataDirs.insert(dataDir);
            uniqueDataDirs.push_back(dataDir);
        }
    }

    // filter to only unique desktop files
    std::vector<bfs::path> desktopFilePaths;
    std::unordered_set<std::string> seenDesktopFiles;
    for(int i=0; i<uniqueDataDirs.size(); i++){
        // std::cout << "path: " << uniqueDataDirs[i] << std::endl;

        bfs::path path = bfs::path(uniqueDataDirs[i])/"applications";
        if(!bfs::exists(path)) continue;

        // std::cout << "successs" << std::endl;
        for (bfs::directory_entry& dirEntry : bfs::directory_iterator(path)){
            std::string desktopFilePath = dirEntry.path().filename().string();
            if(seenDesktopFiles.find(desktopFilePath) != seenDesktopFiles.end()) continue;
            seenDesktopFiles.insert(desktopFilePath);
            desktopFilePaths.push_back(dirEntry.path());
        }
    }

    return desktopFilePaths;
}

int main(){
    auto screen = ftxui::ScreenInteractive::TerminalOutput();

    std::string inputStr = "";
    ftxui::InputOption inputOptions = ui::InputOption::Spacious();
    inputOptions.transform = [](ui::InputState state){
        state.element |= ui::borderRounded;
        return state.element;
    };
    inputOptions.multiline = false;

    ftxui::Component input = ui::Input(&inputStr, inputOptions);

    // check XDG_DATA_DIRS env var is set
    const char* envTemp = std::getenv("XDG_DATA_DIRS");
    if(!envTemp) throw std::runtime_error("XDG_DATA_DIRS not set");

    std::vector<bfs::path> desktopFilePaths = getDesktopFiles(std::string(envTemp));


    std::vector<std::string> appNames;
    for(auto desktopFilePath : desktopFilePaths){

        std::string desktopFilePathStr = desktopFilePath.string();
        std::cout << "foo:" << desktopFilePathStr << std::endl;
        std::ifstream desktopFileStream(desktopFilePathStr);

        if(!desktopFileStream.is_open()){
            std::cerr << "failed to open file: " << desktopFilePathStr << std::endl; // add file path
        }

        std::string curLine;

        while (getline(desktopFileStream, curLine)){
            if(curLine.substr(0, 5)=="Name="){
                std::string appName = curLine.substr(5);
                std::cout << "\tappname: " << appName << std::endl;
                appNames.push_back(appName);
                break;
            }
        }

        desktopFileStream.close();
    }
    std::sort(appNames.begin(), appNames.end());

    std::vector<std::string> menuEntries;

    for(int i=0; i<3; i++){
        menuEntries.push_back(appNames[i]);
    }


    // create menu
    int selectedEntry = 0;
    ui::MenuOption menuOption;
    auto menu = ui::Menu(&menuEntries, &selectedEntry, menuOption);

    input |= ui::CatchEvent([&](ui::Event event) {
        if(event.is_character() || event == ui::Event::Backspace){
            // std::cout << "char: " << event.character() << std::endl;
            std::vector<std::string> newMenuEntries;
            std::string searchValue = inputStr;
            if(event.is_character()) searchValue += event.character();
            else if(event == ui::Event::Backspace) searchValue = searchValue.substr(0, searchValue.size()-1);

            // menuEntries.empty();
            size_t i = 0;
            for(auto appName : appNames){
                if(i>10) break;

                std::string lowerAppName = appName;
                boost::algorithm::to_lower(lowerAppName);
                
                size_t foundPos = lowerAppName.find(searchValue);
                if(foundPos != std::string::npos){
                    // appName +=  " " + std::to_string(foundPos) + " " + appName.substr(foundPos, searchValue.size());
                    newMenuEntries.push_back(appName);
                    i++;
                }
                
            }
            menuEntries=newMenuEntries;
        }
        return false;
        // return false;
    });

    ui::Component mainLayout = ui::Container::Vertical({
        input,
        menu,
    });

    auto renderer = ui::Renderer(mainLayout, [&] {
        return ui::vbox({
            input->Render(),
            ui::text("input string: \"" + inputStr + "\""),
        }) | ui::borderRounded;
    });


    screen.Loop(mainLayout);
}
