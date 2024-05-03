#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <chrono>

class cell_grid : public sf::Drawable, public sf::Transformable{
    public:
        cell_grid(const int window_width, const int window_height){
            int pixel_width = 100;
            int virt_width = window_width/pixel_width;
            int virt_height = window_height/pixel_width;
            m_vertices.setPrimitiveType(sf::Quads);

            for(int x=0; x<virt_width; x++){
                for(int y=0; y<virt_height; y++){
                    float brightness = float(x+y)/(virt_width+virt_height)*255;
                    // brightness = 255;
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
        }
    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
        {
            target.draw(m_vertices, states);
        }
        sf::VertexArray m_vertices;
};
int main()
{

    const int window_width = 800;
    const int window_height = 800;
    cell_grid foo(window_height, window_width);
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Test Window");
 
    float i = 0;
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
        // float brightness = i;
        // for(int i = 0; i<m_vertices.getVertexCount(); i++){
        //     m_vertices[i].color = sf::Color(brightness, brightness, brightness, 255);
        //     
        // }
        // window.draw(m_vertices);
        window.draw(foo);
        window.display();

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;
        std::cout << duration.count() << " milliseconds." << std::endl;
        i+=0.001;
        if(i >= 255){
            i = 0;
        }
    }

    return 0;
}
