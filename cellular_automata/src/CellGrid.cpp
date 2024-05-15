#include "CellGrid.hpp"

CellGrid::CellGrid(const int window_width, const int window_height, const unsigned int pixelWidth){
    virt_width = window_width/pixelWidth;
    virt_height = window_height/pixelWidth;
    m_vertices.setPrimitiveType(sf::Quads);
    grid_array.resize(virt_width, std::vector<int>(virt_height));


    // set initial grid values
    for(int y=0; y<virt_height; y++){
        for(int x=0; x<virt_width; x++){
            // float brightness = float(x+y)/(virt_width+virt_height)*255;
            float brightness = 0;
            grid_array[x][y] = brightness;
        }
    }
    prev_grid_array = grid_array;

    // init verts
    for(int y=0; y<virt_height; y++){
        for(int x=0; x<virt_width; x++){
            float cellValue = grid_array[x][y];
            sf::Color color = mapColor(cellValue);

            int virt_x = x*pixelWidth;
            int virt_y = y*pixelWidth;
            m_vertices.append(sf::Vertex(sf::Vector2f(  virt_x,   virt_y), color));
            m_vertices.append(sf::Vertex(sf::Vector2f(  virt_x, virt_y+pixelWidth), color));
            m_vertices.append(sf::Vertex(sf::Vector2f( virt_x+pixelWidth, virt_y+pixelWidth), color));
            m_vertices.append(sf::Vertex(sf::Vector2f( virt_x+pixelWidth, virt_y), color));
        }
    }
    // CellGrid::refresh_verts();

}
sf::Color CellGrid::mapColor(int cellValue){
    int rgb[3];
    switch(cellValue){
        case 0:
            rgb[0]=15;
            rgb[1]=15;
            rgb[2]=15;
            break;
        case 1:
            rgb[0]=255;
            rgb[1]=255;
            rgb[2]=255;
            break;
        default:
            std::cout<< "undefined mapping value: " << cellValue << std::endl;
            rgb[0]=255;
            rgb[1]=0;
            rgb[2]=0;
            break;
    }
    sf::Color color(rgb[0], rgb[1], rgb[2], 255);
    return color;
}
void CellGrid::refresh_verts(){
    for(int y=0; y<virt_height; y++){
        for(int x=0; x<virt_width; x++){
            if(prev_grid_array[x][y]==grid_array[x][y]){
                continue;
            }
            int cellValue = grid_array[x][y];

            set_vert_color(x, y, mapColor(cellValue));
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

