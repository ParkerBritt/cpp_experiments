#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <chrono>
#include <unistd.h>
#include <vector>

const std::string WINDOW_TITLE = "";
class CellGrid : public sf::Drawable, public sf::Transformable{
    public:
        int virt_height;
        int virt_width;
        sf::VertexArray m_vertices;
        std::vector<std::vector<int>> grid_array;
        std::vector<std::vector<int>> prev_grid_array;
        int pixel_width = 1;
        CellGrid(const int window_width, const int window_height){
            virt_width = window_width/pixel_width;
            virt_height = window_height/pixel_width;
            m_vertices.setPrimitiveType(sf::Quads);
            grid_array.resize(virt_width, std::vector<int>(virt_height));


            for(int y=0; y<virt_height; y++){
                for(int x=0; x<virt_width; x++){
                    float brightness = float(x+y)/(virt_width+virt_height)*255;
                    grid_array[x][y] = brightness;
                }
            }
            prev_grid_array = grid_array;

            // set initial vert color
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
        void refresh_verts(){
            for(int y=0; y<virt_height; y++){
                for(int x=0; x<virt_width; x++){
                    if(prev_grid_array[x][y]==grid_array[x][y]){
                        continue;
                    }
                    int cell_value = grid_array[x][y];
                    sf::Color color(cell_value, cell_value, cell_value, 255);

                    set_vert_color(x, y, color);
                }
            }
            prev_grid_array = grid_array;
        }

        sf::Color get_color(const int x, const int y){
            int index = x*4+(y*virt_height*4);
                return m_vertices[index].color;

        }
        void set_color(const int x, const int y, int cell_value){
            grid_array[x][y] = cell_value;
        }
        void set_vert_color(const int x, const int y, sf::Color color){
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
    CellGrid foo(window_height, window_width);

    std::vector<std::vector<int>> grid_array_default_state = foo.grid_array;
    foo.refresh_verts();

    sf::RenderWindow window(sf::VideoMode(window_width, window_height), WINDOW_TITLE);
 
    float i = 0;
    int selected_x = 0;
    int selected_y = 0;


    sf::Clock deltaClock;
    sf::Time updateInterval = sf::seconds(0.01);
    sf::Time elapsedTimeSinceLastUpdate = sf::Time::Zero;

    while (window.isOpen())
    {
        auto start = std::chrono::high_resolution_clock::now();
        sf::Time dt = deltaClock.restart();
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
        }

        elapsedTimeSinceLastUpdate += dt;
        if(elapsedTimeSinceLastUpdate >= updateInterval || true){
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
        std::cout << dt.asMilliseconds() << " milliseconds" << std::endl;
        // usleep(100000);
    }

    return 0;
}
