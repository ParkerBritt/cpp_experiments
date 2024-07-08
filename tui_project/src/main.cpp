#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

namespace ui = ftxui;
int main(){
     auto screen = ftxui::ScreenInteractive::TerminalOutput();

    std::string inputStr = "";
    ftxui::InputOption inputOptions = ui::InputOption::Spacious();
    inputOptions.transform = [](ui::InputState state){
        state.element |= ui::borderRounded;
        return state.element;
    };

    ftxui::Component input = ui::Input(inputStr, inputOptions);
    screen.Loop(input);
}
