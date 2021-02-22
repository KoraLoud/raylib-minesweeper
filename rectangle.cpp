#include "rectangle.h"
#include<cstring>

rectangle::rectangle()
{
    x = 0;
    y = 0;
    height = 0;
    width = 0;
    touchingMines = 0;
    flagged = false;

}
rectangle::rectangle(int _x, int _y, int _height, int _width)
{
    x = _x;
    y = _y;
    height = _height;
    width = _width;
    touchingMines = 0;
    flagged = false;
}

rectangle::rectangle(int _x, int _y, int _height, int _width, Color _color)
{
    x = _x;
    y = _y;
    height = _height;
    width = _width;
    color = _color;
    touchingMines = 0;
    flagged = false;
}

void rectangle::draw()
{
    DrawRectangle(x, y, width, height, color);
    if(touchingMines > 0)
    {
        std::string text = std::to_string(touchingMines);
        char char_array[text.length() + 1];
        strcpy(char_array, text.c_str());
        DrawText(char_array, x+(width/2), y+(height/2), 12, BLACK); 
    }
}
