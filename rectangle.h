#pragma once

extern "C"
{
#include "raylib.h"
}


enum Space
{
    Empty,
    Mine,
    Searched
};

class rectangle
{
public:
    int x;
    int y;
    int width;
    int height;
    Color color;
    Space spaceInfo;
    bool flagged;

    int vectorX;
    int vectorY;

    int touchingMines;

    rectangle();
    rectangle(int, int, int, int);
    rectangle(int, int, int, int, Color);

    void draw();
};