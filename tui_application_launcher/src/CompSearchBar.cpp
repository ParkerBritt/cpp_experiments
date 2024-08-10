#include "CompSearchBar.hpp"
#include <ftxui/component/component.hpp>
#include "iostream"

namespace ui = ftxui;

SearchBar::SearchBar(std::string& inputStr){
    // configure input options
    inputOptions_ = ui::InputOption::Spacious();
    inputOptions_.transform = [](ui::InputState state){
        state.element |= ui::borderRounded;
        return state.element;
    };
    inputOptions_.multiline = false;

    input_ = ui::Input(&inputStr, inputOptions_);

    Add(input_);
}

ui::Component& SearchBar::getComponent(){
    return input_;
}

bool SearchBar::Focusable() const {
    return isFocusable;
}

ui::Element SearchBar::Render(){
    return input_->Render();
}
