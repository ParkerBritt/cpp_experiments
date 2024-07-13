// ftxui
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

// boost
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/filesystem.hpp>


#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <stdexcept>

namespace ui = ftxui;
namespace bfs = boost::filesystem;

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
    const char* envTemp = std::getenv("XDG_DATA_DIRS");
    if(envTemp){
        // split env var into vector
        std::string dataDirsRaw = envTemp;
        std::vector<std::string> dataDirsSplit;
        boost::split(dataDirsSplit, dataDirsRaw, boost::is_any_of(":"));

        // filter vector to unique values
        std::sort(dataDirsSplit.begin(), dataDirsSplit.end());
        std::vector<std::string>::iterator lastUniquePos = std::unique(dataDirsSplit.begin(), dataDirsSplit.end());
        dataDirsSplit.erase(lastUniquePos, dataDirsSplit.end());

        for(int i=0; i<dataDirsSplit.size(); i++){
            std::cout << "path: " << dataDirsSplit[i] << std::endl;
            bfs::path path = bfs::path(dataDirsSplit[i])/"applications";
            if(bfs::exists(path)){
                std::cout << "successs" << std::endl;
                for (bfs::directory_entry& dirEntry : bfs::directory_iterator(path)){
                    std::cout<< "\tinside: " << dirEntry.path() << std::endl;
                }
            }
            else std::cout << "failed" << std::endl;
        }
    }
    else{
        throw std::runtime_error("XDG_DATA_DIRS not set");
    }

    screen.Loop(renderer);
}
