#ifndef CELLGRID_HPP
#define CELLGRID_HPP

#include <SFML/Graphics.hpp>
#include <vector>

class CellGrid : public sf::Drawable, public sf::Transformable{
    public:
        // global vars
        int virt_height;
        int virt_width;
        sf::VertexArray m_vertices;
        std::vector<std::vector<int>> grid_array;
        std::vector<std::vector<int>> prev_grid_array;

        // constructor
        CellGrid(const int window_width, const int window_height, const int pixelWidth);

        // methods
        void refresh_verts();
        sf::Color get_color(const int x, const int y);
        void set_color(const int x, const int y, int cell_value);
        void set_vert_color(const int x, const int y, sf::Color color);

        virtual ~CellGrid() {}

    private:
        // draw function
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
#endif
