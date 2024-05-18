#ifndef CELLGRID_HPP
#define CELLGRID_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

class CellGrid : public sf::Drawable, public sf::Transformable{
    public:
        // global vars
        int virt_height;
        int virt_width;
        int& width = virt_width;
        int& height = virt_height;
        mutable bool doVertRefresh = true;
        mutable sf::VertexArray m_vertices;
        std::vector<std::vector<int>> grid_array;
        mutable std::vector<std::vector<int>> prev_grid_array;

        // constructor
        CellGrid(const int window_width, const int window_height, const unsigned int pixelWidth);

        // methods
        void refresh_verts();
        int getValue(const int x, const int y) const;
        void setValue(const int x, const int y, int cell_value);
        void set_vert_color(const int x, const int y, sf::Color color) const;

        virtual ~CellGrid() {}

        sf::Color mapColor(int cellValue) const;

    private:
        // draw function
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
#endif
