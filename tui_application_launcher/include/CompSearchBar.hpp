#ifndef COMP_SEARCH_BAR_HPP
#define COMP_SEARCH_BAR_HPP

#include <ftxui/component/component.hpp>

namespace ui = ftxui;

class SearchBar : public ui::ComponentBase {
public:
    // constructors
    SearchBar(std::string& inputStr);
    virtual ~SearchBar() = default;

    // overrides
    bool Focusable() const override;
    ui::Element Render() override;

private:
    ui::Component input_;
    ui::InputOption inputOptions_;

};


#endif
