#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <chrono>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include "CellGrid.hpp"

const std::string WINDOW_TITLE = "Cells";
int main()
{

    const int window_width = 800;
    const int window_height = 800;
    CellGrid foo(window_height, window_width);

    std::vector<std::vector<int>> grid_array_default_state = foo.grid_array;
    foo.refresh_verts();

    sf::RenderWindow window(sf::VideoMode(window_width, window_height), WINDOW_TITLE);
    float viewWidth = 400.0f;
    float viewHeight = 400.0f;
    const float SCROLL_SENSITIVITY = 0.06;
    sf::View mainView(sf::FloatRect(0,0 ,viewWidth, viewHeight));
    window.setView(mainView);
 
    float i = 0;
    int selected_x = 0;
    int selected_y = 0;


    sf::Clock deltaClock;
    sf::Time updateInterval = sf::seconds(0.01);
    sf::Time elapsedTimeSinceLastUpdate = sf::Time::Zero;

    bool uncapUpdateSpeed = false;
    bool displayUpdateTime = false;
    sf::Vector2i prevLocalMousePos;
    int isPanDown = false;
    sf::Vector2f prevViewCenter = mainView.getCenter();
    float viewX=prevViewCenter.x;
    float viewY=prevViewCenter.y;
    while (window.isOpen())
    {
        bool updateView = false;
        auto start = std::chrono::high_resolution_clock::now();
        sf::Time dt = deltaClock.restart();
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
            // scroll controls
            else if(event.type == sf::Event::MouseWheelScrolled){
                std::cout << event.mouseWheelScroll.delta << std::endl;
                float scroll_dir = event.mouseWheelScroll.delta;
                viewWidth = viewWidth-(viewWidth*SCROLL_SENSITIVITY*scroll_dir);
                viewWidth = std::clamp(viewWidth, 5.0f, static_cast<float>(window_width*1.2));
                viewHeight = viewHeight-(viewHeight*SCROLL_SENSITIVITY*scroll_dir);
                viewHeight = std::clamp(viewHeight, 5.0f, static_cast<float>(window_height*1.2));
                updateView = true;
            }
        }

        elapsedTimeSinceLastUpdate += dt;
        // pan controls
        if (sf::Mouse::isButtonPressed(sf::Mouse::Middle) || (sf::Mouse::isButtonPressed(sf::Mouse::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))){
            sf::Vector2i localMousePosition = sf::Mouse::getPosition(window);
            if(!isPanDown){
                // initial down
                prevLocalMousePos = localMousePosition;
                prevViewCenter = mainView.getCenter();
                std::cout << "setting prev mouse pos";
                std::cout << "prev screen center: " << prevViewCenter.x << " " << prevViewCenter.y << std::endl;
                // sf::View mainView(sf::FloatRect(viewX,viewY ,600.0f, 600.0f));
                isPanDown= true;
            }
            else{
                // after inial press
                viewX = prevViewCenter.x+(prevLocalMousePos.x-localMousePosition.x);
                viewY = prevViewCenter.y+(prevLocalMousePos.y-localMousePosition.y);
                updateView = true;
                // std::cout << "local pos: " << " " << localMousePosition.x << " " << localMousePosition.y << std::endl;
                // std::cout << "move view to: " << viewX << " " << viewY << std::endl;
            }
        }
        else if(isPanDown){
            isPanDown = false;
        }
        if(updateView){
            std::cout << "updating view" << std::endl;
            mainView = sf::View(sf::Vector2f(viewX,viewY), sf::Vector2f(viewWidth, viewHeight));
            window.setView(mainView);
        }

        if(elapsedTimeSinceLastUpdate >= updateInterval || uncapUpdateSpeed){
            // reset set grid colors to initial color
            for(int x = 0; x<foo.grid_array.size(); x++){
                for(int y=0; y<foo.grid_array[0].size(); y++){
                    foo.grid_array[x][y] = grid_array_default_state[x][y];
                } 
            }
            // set changing cell to full bright
            foo.set_color(selected_x, selected_y, 255);
            foo.refresh_verts();

            // draw frame
            window.clear();
            window.draw(foo);
            window.display();

            // iterate
            selected_x++;
            if(selected_x > foo.virt_width-1){
                selected_x = 0;
                selected_y = (selected_y + 1)%(foo.virt_height);
            }
            elapsedTimeSinceLastUpdate = sf::Time::Zero;
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;
        // std::cout << duration.count() << " milliseconds" << std::endl;
        if(displayUpdateTime){
            std::cout << dt.asMilliseconds() << " milliseconds" << std::endl;
        }
        // usleep(100000);
    }

    return 0;
}
