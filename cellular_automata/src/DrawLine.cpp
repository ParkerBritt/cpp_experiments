#include "DrawLine.hpp"
#include <iostream>

void plotLine(CellGrid& grid, unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1){
    int dx = x1 - x0;
    int dy = y1 - y0;
    int D = 2*dy - dx;
    unsigned int y = y0;

    for(int x=x0; x<x1; x++){
        grid.setValue(x, y, 1);
        if(D > 0){
            y = y + 1;
            D = D - 2*dx;
        }
        D = D + 2*dy;
    }
}
