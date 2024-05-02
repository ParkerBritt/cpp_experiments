#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <chrono>

int main()
{
    int window_width = 800;
    int window_height = 800;
    int pixel_scale = 100;
    int virtual_width = std::floor(float(window_width)/pixel_scale)*pixel_scale;
    int virtual_height = std::floor(float(window_height)/pixel_scale)*pixel_scale;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Testing Window");
    sf::CircleShape my_shape(100.f);
    my_shape.setFillColor(sf::Color::Blue);
    sf::Image my_image;
    sf::Sprite my_sprite;
    my_image.create(window_width,window_height, sf::Color::Blue);
    for(int x=0; x<window_width; x++){
        int virtual_x = std::floor(float(x)/pixel_scale)*pixel_scale;
        for(int y=0; y<window_height; y++){
            int virtual_y = std::floor(float(y)/pixel_scale)*pixel_scale;
            int brightness = (float(virtual_x+virtual_y)/(virtual_width+virtual_height))*255;
            my_image.setPixel(x, y, sf::Color(brightness,brightness,brightness,255));
        }
    }
    // std::cout << "virtual_width: " << virtual_width << std::endl;
    sf::Texture my_texture;
    my_texture.loadFromImage(my_image);
    my_sprite.setTexture(my_texture);
    // my_sprite.setColor(sf::Color::Blue);
    float time = 0;
    float speed = 0.0001;
    float scale = 100;

    while (window.isOpen())
    {
        auto timer_start = std::chrono::high_resolution_clock::now();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        float rad = (std::sin(time)+1)/2*scale;
        my_shape.setRadius(rad);
        window.draw(my_shape);
        // window.draw(my_sprite);
        window.display();
        time+= speed;

        auto timer_end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = timer_end - timer_start; 
        std::cout << duration.count() << " milliseconds." << std::endl;
    }

    return 0;
}

