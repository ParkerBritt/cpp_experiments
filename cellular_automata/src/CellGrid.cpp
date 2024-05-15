#include "CellGrid.hpp"

CellGrid::CellGrid(const int window_width, const int window_height, const int pixelWidth){
    virt_width = window_width/pixelWidth;
    virt_height = window_height/pixelWidth;
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

            int virt_x = x*pixelWidth;
            int virt_y = y*pixelWidth;
            m_vertices.append(sf::Vertex(sf::Vector2f(  virt_x,   virt_y), color));
            m_vertices.append(sf::Vertex(sf::Vector2f(  virt_x, virt_y+pixelWidth), color));
            m_vertices.append(sf::Vertex(sf::Vector2f( virt_x+pixelWidth, virt_y+pixelWidth), color));
            m_vertices.append(sf::Vertex(sf::Vector2f( virt_x+pixelWidth, virt_y), color));
        }
    }

}
void CellGrid::refresh_verts(){
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

sf::Color CellGrid::get_color(const int x, const int y){
    int index = x*4+(y*virt_height*4);
        return m_vertices[index].color;

}
void CellGrid::set_color(const int x, const int y, int cell_value){
    grid_array[x][y] = cell_value;
}
void CellGrid::set_vert_color(const int x, const int y, sf::Color color){
    int index = x*4+(y*virt_height*4);
    for(int i=0; i<4; i++){
        m_vertices[index+i].color = color;
    }

}
void CellGrid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_vertices, states);
}

