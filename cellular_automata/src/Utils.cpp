#include "Utils.hpp"


    // unsigned int windowWidth = windowSize.x;
    // unsigned int windowHeight = windowSize.y;

sf::Vector2f windowToWorldTransform(sf::Vector2f& windowPos, sf::RenderWindow& window, sf::View& view){
    std::cout << "working" << std::endl;
    
    // size
    sf::Vector2u windowSize = window.getSize(); 
    sf::Vector2f viewCenter = view.getCenter();
    sf::Vector2f viewSize = view.getSize();
    

    float worldX = windowPos.x/(windowSize.x/viewSize.x)+(viewCenter.x-viewSize.x/2);
    float worldY = windowPos.y/(windowSize.y/viewSize.y)+(viewCenter.y-viewSize.y/2);
    sf::Vector2f worldCoord = sf::Vector2f(worldX, worldY);
    return worldCoord;
}
