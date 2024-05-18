#include "DrawLine.hpp"
#include <iostream>

void plotLine(CellGrid& grid, unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1){
    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int error = dx + dy;
    
    while(true){
        grid.setValue(x0, y0, 1);
        if(x0 == x1 && y0 == y1){
            break;
        }
        int e2 = 2 * error;
        if(e2 >= dy){
            if(x0 == x1){
                break;
            }
            error = error + dy;
            x0 = x0 + sx;
        }
        if(e2 <= dx){
            if(y0 == y1){
                break;
            }
            error = error + dx;
            y0 = y0 + sy;
        }
    }
}
