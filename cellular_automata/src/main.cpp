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
const sf::Color BG_COLOR(18,18,18);

// non constants
float viewWidth = 400.0f;
float viewHeight = 400.0f;
float viewX;
float viewY;

struct ViewState{
    bool isPanDown = false;
    sf::Vector2i prevLocalMousePos;
    sf::Vector2f prevViewCenter;
    sf::View mainView;
};

struct GameState{
    unsigned int selected_x;
    unsigned int selected_y;
    std::vector<std::vector<int>> gridArrayDefaultState;
};

// declare functions
void initWindow(sf::RenderWindow& window, sf::View& view);
void updateView(sf::RenderWindow& window, sf::View& view);
void handleEvents(sf::RenderWindow& window, ViewState& viewState);
void updateGameState(GameState& gameState, CellGrid& cellGrid);
void renderFrame(sf::RenderWindow& window, std::vector<sf::Drawable*> renderBuffer);


int main()
{


    // init view
    ViewState viewState;
    GameState gameState;
    sf::View& mainView = viewState.mainView;
    viewState.prevViewCenter = mainView.getCenter();

    // init render window 
    sf::RenderWindow window;
    initWindow(window, mainView);

    // init cell grid
    CellGrid grid(WINDOW_HEIGHT, WINDOW_WIDTH);

    // set initial gameState
    gameState.gridArrayDefaultState = grid.grid_array;

    grid.refresh_verts();


    window.setView(mainView);
 

    // render buffer
    std::vector<sf::Drawable*> renderBuffer;
    renderBuffer.push_back(&grid);


    sf::Clock deltaClock;
    sf::Time updateInterval = sf::seconds(0.01);
    sf::Time elapsedTimeSinceLastUpdate = sf::Time::Zero;

    bool uncapUpdateSpeed = false;
    bool displayUpdateTime = false;
    viewX=WINDOW_WIDTH/2;
    viewY=WINDOW_HEIGHT/2;

    while (window.isOpen())
    {
        // bool updateView = false;
        sf::Time dt = deltaClock.restart();
        elapsedTimeSinceLastUpdate += dt;

        // handle events
        handleEvents(window, viewState);

        if(elapsedTimeSinceLastUpdate >= updateInterval || uncapUpdateSpeed){
            updateGameState(gameState, grid);
            renderFrame(window, renderBuffer);
            elapsedTimeSinceLastUpdate = sf::Time::Zero;
        }

        if(displayUpdateTime){
            std::cout << dt.asMilliseconds() << " milliseconds" << std::endl;
        }
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

void handleEvents(sf::RenderWindow& window, ViewState& viewState){
        sf::Event event;
        bool& isPanDown = viewState.isPanDown;
        sf::Vector2i& prevLocalMousePos = viewState.prevLocalMousePos;
        sf::Vector2f& prevViewCenter = viewState.prevViewCenter;
        sf::View& mainView = viewState.mainView;
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
            }
        }

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
            }
        }
        else if(isPanDown){
            isPanDown = false;
        }
        updateView(window, mainView);
}

void updateGameState(GameState& gameState, CellGrid& cellGrid){
    cellGrid.grid_array = gameState.gridArrayDefaultState;
    unsigned int& selected_x = gameState.selected_x;
    unsigned int& selected_y = gameState.selected_y;
    cellGrid.set_color(selected_x, selected_y, 255);
    cellGrid.refresh_verts();

    // iterate
    selected_x++;
    if(selected_x > cellGrid.virt_width-1){
        selected_x = 0;
        selected_y = (selected_y + 1)%(cellGrid.virt_height);
    }
}

void renderFrame(sf::RenderWindow& window, std::vector<sf::Drawable*> renderBuffer){
        window.clear(BG_COLOR);
        for(long unsigned i=0; i<renderBuffer.size(); i++){
            window.draw(*renderBuffer[i]);
            // std::cout << "Drawing object: " << typeid(*renderBuffer[i]).name() << std::endl;
        }
        window.display();
}
