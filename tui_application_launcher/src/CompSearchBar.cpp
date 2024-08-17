#include "CompSearchBar.hpp"
#include <boost/algorithm/string/case_conv.hpp>
#include <ftxui/component/component.hpp>
#include "IconMap.hpp"
#include <iostream>
#include <vector>

namespace ui = ftxui;

SearchBar::SearchBar(){
    // configure input options
    inputOptions_ = ui::InputOption::Spacious();
    inputOptions_.transform = [](ui::InputState state){
        state.element |= ui::borderRounded;
        return state.element;
    };
    inputOptions_.multiline = false;

    input_ = ui::Input(&inputStr_, inputOptions_);


    Add(input_);
}

void SearchBar::setupSearchEvent(std::vector<std::string>& appNames, std::vector<std::string>& menuEntries){
        input_ |= ui::CatchEvent([&](ui::Event event) {
        if(event.is_character() || event == ui::Event::Backspace){
            // std::cout << "char: " << event.character() << std::endl;
            std::vector<std::string> newMenuEntries;
            std::string searchValue = inputStr_;
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
                    std::string displayName = appName;
                    if(iconMap.find(lowerAppName) != iconMap.end()){
                        displayName = iconMap[lowerAppName]+" "+displayName;
                    }
                    else{
                        displayName = "󰘔 " + displayName;
                    }
                    newMenuEntries.push_back(displayName);
                    i++;
                }
                
            }
            menuEntries=newMenuEntries;
        }
        return false;
        // return false;
    });
}

ui::Component& SearchBar::getComponent(){
    return input_;
}

bool SearchBar::Focusable() const {
    return false;
}

ui::Element SearchBar::Render(){
    return input_->Render();
}
