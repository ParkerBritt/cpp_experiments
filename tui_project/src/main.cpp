#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include <cstdlib>

namespace ui = ftxui;
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
            ui::text("world" + inputStr),
        }) | ui::borderRounded;
    });
    if(const char* dataDirs = std::getenv("XDG_DATA_DIRS")){
        std::cout << "val: " << std::getenv("XDG_DATA_DIRS") << std::endl;;
    }

    screen.Loop(renderer);
}
