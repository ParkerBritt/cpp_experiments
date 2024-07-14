// ftxui
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

// boost
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/filesystem.hpp>


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
    ui::Component mainLayout = ui::Container::Vertical({
        input,
    });

    auto renderer = ui::Renderer(mainLayout, [&] {
        return ui::vbox({
            input->Render(),
            ui::text("input string: \"" + inputStr + "\""),
        }) | ui::borderRounded;
    });

    // check XDG_DATA_DIRS env var is set
    const char* envTemp = std::getenv("XDG_DATA_DIRS");
    if(!envTemp) throw std::runtime_error("XDG_DATA_DIRS not set");

    std::vector<bfs::path> desktopFilePaths = getDesktopFiles(std::string(envTemp));

    for(auto desktopFilePath : desktopFilePaths){
        // std::cout << "foo:" << desktopFilePath.string() << std::endl;
    }




    screen.Loop(renderer);
}
