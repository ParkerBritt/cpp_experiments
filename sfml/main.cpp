#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <chrono>

int main()
{

    sf::VertexArray m_vertices(sf::Quads);
    int window_width = 800;
    int window_height = 800;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Test Window");
    int pixel_width = 10;
    int virt_width = window_width/pixel_width;
    int virt_height = window_height/pixel_width;

    for(int x=0; x<virt_width; x++){
        for(int y=0; y<virt_height; y++){
            float brightness = float(x+y)/(virt_width+virt_height)*255;
            std::cout << brightness << std::endl;
            sf::Color color(brightness, brightness, brightness, 255);

            int virt_x = x*pixel_width;
            int virt_y = y*pixel_width;
            m_vertices.append(sf::Vertex(sf::Vector2f(  virt_x,   virt_y), color));
            m_vertices.append(sf::Vertex(sf::Vector2f(  virt_x, virt_y+pixel_width), color));
            m_vertices.append(sf::Vertex(sf::Vector2f( virt_x+pixel_width, virt_y+pixel_width), color));
            m_vertices.append(sf::Vertex(sf::Vector2f( virt_x+pixel_width, virt_y), color));
        }
    }
 
    while (window.isOpen())
    {
        auto start = std::chrono::high_resolution_clock::now();
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(m_vertices);
        window.display();

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;
        std::cout << duration.count() << " milliseconds." << std::endl;
    }

    return 0;
}
