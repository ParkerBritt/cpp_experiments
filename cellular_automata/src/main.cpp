#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include "CellGrid.hpp"

// constants and configuration
const std::string WINDOW_TITLE = "Cells";
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const float SCROLL_SENSITIVITY = 0.06;

// non constants
float viewWidth = 400.0f;
float viewHeight = 400.0f;
float viewX;
float viewY;

// declare functions
void initWindow(sf::RenderWindow& window, sf::View& view);
void updateView(sf::RenderWindow& window, sf::View& view);

int main()
{

    // init cell grid
    CellGrid grid(WINDOW_HEIGHT, WINDOW_WIDTH);

    std::vector<std::vector<int>> gridArrayDefaultState = grid.grid_array;
    grid.refresh_verts();

    // init render window 
    sf::RenderWindow window;
    sf::View mainView;
    initWindow(window, mainView);

    window.setView(mainView);
 
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
    viewX=prevViewCenter.x;
    viewY=prevViewCenter.y;

    while (window.isOpen())
    {
        // bool updateView = false;
        sf::Time dt = deltaClock.restart();
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
            // scroll controls
            else if(event.type == sf::Event::MouseWheelScrolled){
                float scroll_dir = event.mouseWheelScroll.delta;
                viewWidth = viewWidth-(viewWidth*SCROLL_SENSITIVITY*scroll_dir);
                viewWidth = std::clamp(viewWidth, 5.0f, static_cast<float>(WINDOW_WIDTH*1.2));
                viewHeight = viewHeight-(viewHeight*SCROLL_SENSITIVITY*scroll_dir);
                viewHeight = std::clamp(viewHeight, 5.0f, static_cast<float>(WINDOW_HEIGHT*1.2));
                // updateView = true;
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
                // sf::View mainView(sf::FloatRect(viewX,viewY ,600.0f, 600.0f));
                isPanDown= true;
            }
            else{
                // after inial press
                viewX = prevViewCenter.x+(prevLocalMousePos.x-localMousePosition.x);
                viewY = prevViewCenter.y+(prevLocalMousePos.y-localMousePosition.y);
                // updateView = true;
                // std::cout << "local pos: " << " " << localMousePosition.x << " " << localMousePosition.y << std::endl;
                // std::cout << "move view to: " << viewX << " " << viewY << std::endl;
            }
        }
        else if(isPanDown){
            isPanDown = false;
        }
        updateView(window, mainView);

        if(elapsedTimeSinceLastUpdate >= updateInterval || uncapUpdateSpeed){
            // reset set grid colors to initial color
            for(long unsigned x = 0; x<grid.grid_array.size(); x++){
                for(long unsigned y=0; y<grid.grid_array[0].size(); y++){
                    grid.grid_array[x][y] = gridArrayDefaultState[x][y];
                } 
            }
            // set changing cell to full bright
            grid.set_color(selected_x, selected_y, 255);
            grid.refresh_verts();

            // draw frame
            window.clear();
            window.draw(grid);
            window.display();

            // iterate
            selected_x++;
            if(selected_x > grid.virt_width-1){
                selected_x = 0;
                selected_y = (selected_y + 1)%(grid.virt_height);
            }
            elapsedTimeSinceLastUpdate = sf::Time::Zero;
        }

        if(displayUpdateTime){
            std::cout << dt.asMilliseconds() << " milliseconds" << std::endl;
        }
        // usleep(100000);
    }

    return 0;
}

void updateView(sf::RenderWindow& window, sf::View& view){
    // std::cout << "updating view" << std::endl;
    // mainView = sf::View(sf::Vector2f(viewX,viewY), sf::Vector2f(viewWidth, viewHeight));
    view.setCenter(viewX, viewY);
    view.setSize(viewWidth, viewHeight);
    window.setView(view);
}

void initWindow(sf::RenderWindow& window, sf::View& view){
    window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);
    view.setSize(viewWidth, viewHeight);
    view.setCenter(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
    window.setView(view);
}
