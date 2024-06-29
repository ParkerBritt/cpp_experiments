#include <iostream>

#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/screen.hpp"
#include "ftxui/screen/string.hpp"

int main(void) {
  using namespace ftxui;

  auto helloWorldSection = [&] {
    auto content = vbox({
        hbox({text(L" Hello World ") | color(Color::Blue), text(L"🙂") | bold}) | color(Color::Green),
    });
    return window(text(L" Header "), content);
  };

  auto document =  //
      vbox({
          helloWorldSection()
      });

  // Limit the size of the document to 80 char.
  document = document | size(WIDTH, LESS_THAN, 80);

  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
  Render(screen, document);

  std::cout << screen.ToString() << '\0' << std::endl;

  return EXIT_SUCCESS;
}
