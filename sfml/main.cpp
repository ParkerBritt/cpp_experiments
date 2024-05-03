#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <chrono>
#include <unistd.h>
#include <vector>

class cell_grid : public sf::Drawable, public sf::Transformable{
    public:
        int virt_height;
        int virt_width;
        sf::VertexArray m_vertices;
        std::vector<std::vector<float>> grid_array;
        int pixel_width = 100;
        cell_grid(const int window_width, const int window_height){
            virt_width = window_width/pixel_width;
            virt_height = window_height/pixel_width;
            m_vertices.setPrimitiveType(sf::Quads);
            grid_array.resize(virt_width, std::vector<float>(virt_height));


            for(int y=0; y<virt_height; y++){
                for(int x=0; x<virt_width; x++){
                    float brightness = float(x+y)/(virt_width+virt_height)*255;
                    grid_array[x][y] = brightness;
                }
            }
            refresh_verts();

        }
        void refresh_verts(){
            for(int y=0; y<virt_height; y++){
                for(int x=0; x<virt_width; x++){
                    float brightness = grid_array[x][y];
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
        sf::Color get_color(const int x, const int y){
            int index = x*4+(y*virt_height*4);
                return m_vertices[index].color;

        }
        void set_color(const int x, const int y, sf::Color color){
            int index = x*4+(y*virt_height*4);
            for(int i=0; i<4; i++){
                m_vertices[index+i].color = color;
            }

        }
    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
        {
            target.draw(m_vertices, states);
        }
};
int main()
{

    const int window_width = 800;
    const int window_height = 800;
    cell_grid foo(window_height, window_width);

    foo.grid_array[0][0] = 255;
    foo.refresh_verts();
    for(int x = 0; x<foo.grid_array.size(); x++){
        for(int y=0; y<foo.grid_array[0].size(); y++){
            std::cout << foo.grid_array[x][y] << " ";
        } 
        std::cout << std::endl;
    }
    // return 0;
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
        float brightness = i;
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
