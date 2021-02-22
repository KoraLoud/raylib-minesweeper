#include "button.h"
#include<cstring>

bool button::isMouseOver(int mx, int my)
{
    if(mx >= x && mx <= x+width && my>=y && my<=y+height)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void button::draw()
{
    rectangle::draw();

    char char_array[buttonText.length()+1];
    strcpy(char_array, buttonText.c_str());
    DrawText(char_array, x, y, 16, BLACK);

}