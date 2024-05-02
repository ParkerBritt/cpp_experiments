#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

int main()
{
    int x_width = 800;
    int y_width = 800;
    sf::RenderWindow window(sf::VideoMode(x_width, y_width), "Testing Window");
    sf::CircleShape my_shape(100.f);
    my_shape.setFillColor(sf::Color::Blue);
    sf::Image my_image;
    sf::Sprite my_sprite;
    my_image.create(x_width,y_width, sf::Color::Blue);
    for(int x=0; x<x_width; x++){
        for(int y=0; y<y_width; y++){
            int brightness = float(x)/x_width*255;
            std::cout << "brightness: " << brightness << std::endl;
            my_image.setPixel(x, y, sf::Color(brightness,brightness,brightness,255));
        }
    }
    sf::Texture my_texture;
    my_texture.loadFromImage(my_image);
    my_sprite.setTexture(my_texture);
    // my_sprite.setColor(sf::Color::Blue);
    float time = 0;
    float speed = 0.0001;
    float scale = 100;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        float rad = (std::sin(time)+1)/2*scale;
        my_shape.setRadius(rad);
        // window.draw(my_shape);
        window.draw(my_sprite);
        window.display();
        time+= speed;
    }

    return 0;
}

