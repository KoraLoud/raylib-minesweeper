#pragma once
#include "rectangle.h"
#include<string>

class button : public rectangle 
{
    public:

    std::string buttonText;

    button() : rectangle() {}
    button(int x, int y, int h, int w) : rectangle(x,y,h,w) {};
    button(int x, int y, int h, int w, Color c) : rectangle(x, y, h, w, c) {};

    bool isMouseOver(int, int);

    void draw();
};