extern "C"
{
    #include "raylib.h"
}

#include<iostream>
#include<vector>
#include<deque>
#include<stdlib.h>
#include<time.h>

#include "rectangle.cpp"
#include "button.cpp"

enum gameStates
{
    Playing,
    Loss,
    Win
};

int getTileNumber(int x, int y, std::vector<std::vector<rectangle>>& board)
{
    int ret = 0;
    for(int i=-1;i<2;i++)
    {
        for(int j=-1;j<2;j++)
        {
            int cX = x+j;
            int cY = y+i;
            if(cX >= 0 && cX <= board.size()-1 && cY >= 0 && cY <= board[0].size()-1)
            {
                if(board[cX][cY].spaceInfo == Space::Mine)
                {
                    ret++;
                }
            }
        }
    }
    board[x][y].touchingMines = ret;  
    return ret;
}

void searchSquare(int x, int y, std::vector<std::vector<rectangle>>& board)
{
    std::deque<rectangle> queue;
    queue.push_back(board[x][y]);
    while(!queue.empty())
    {
        rectangle &currentNode = queue.front();
        queue.pop_front();
        int cNodeX = currentNode.vectorX;
        int cNodeY = currentNode.vectorY;
        if(currentNode.spaceInfo == Space::Empty)
        {
            board[cNodeX][cNodeY].spaceInfo = Space::Searched;
            board[cNodeX][cNodeY].flagged = false;
            if(getTileNumber(cNodeX, cNodeY, board) == 0)
            {
                for(int i=-1;i<2;i++)
                {
                    for(int j=-1;j<2;j++)
                    {
                        int cX = cNodeX+j;
                        int cY = cNodeY+i;
                        if(cX >= 0 && cX <= board.size()-1 && cY >= 0 && cY <= board[0].size()-1)
                        {
                            if(board[cX][cY].spaceInfo == Space::Empty)
                            {
                                queue.push_back(board[cX][cY]);
                            }
                        }
                    }
                }
            }
        }
    }
}

void generateMines(int x, int y, std::vector<std::vector<rectangle>>& board, int maxMines, int& mines)
{
    while(mines < maxMines)
    {
        int randX = rand() % board.size();
        int randY = rand() % board[0].size();
        if(board[randX][randY].spaceInfo == Space::Empty)
        {
            if(abs(randX - x) > 1 || abs(randY - y) > 1)
            {
                board[randX][randY].spaceInfo = Space::Mine;
                mines++;
            }
        }
    }
}

    const int screenWidth = 800;
    const int screenHeight = 600;

    int boardHeight = 8;
    int boardWidth = 8;
    int mineMax = 10;
    int mines;
    bool minesGenerated;
    float borderSize = 0.98f;

    gameStates gameState;
    //vars that keep track of board placements and drawing
    std::vector<std::vector<rectangle>> board(boardWidth, std::vector<rectangle>(boardHeight));

    int PlayHeight = screenHeight*0.05;
    int PlayWidth = screenWidth*0.05;

    int PlayHeightScale = (screenHeight*0.9)/boardHeight;
    int PlayWidthScale = (screenWidth*0.9)/boardWidth;


void reset()
{
    mines = 0;
    minesGenerated = false;
    srand(time(NULL));

    for(int x=0;x<boardWidth;x++)
    {
        for (int y=0;y<boardHeight;y++)
        {
            board[x][y].x = PlayWidth+(x*PlayWidthScale);
            board[x][y].y = PlayHeight+(y*PlayHeightScale);
            board[x][y].height = PlayHeightScale*borderSize;
            board[x][y].width = PlayWidthScale*borderSize;
            board[x][y].spaceInfo = Space::Empty;
            board[x][y].vectorX = x;
            board[x][y].vectorY = y;
            board[x][y].touchingMines = 0;
            board[x][y].flagged = false;
        }
    }

    gameState = gameStates::Playing;
}


int main()
{

    InitWindow(screenWidth, screenHeight, "minesweeper");

    SetTargetFPS(30);

    reset();

    int resetButtonWidth = 50;
    button resetButton = button(screenWidth*0.5-resetButtonWidth, screenHeight*0.75, 20, resetButtonWidth);
    resetButton.buttonText = "restart?";

    while (!WindowShouldClose())
    {

        if(gameState == gameStates::Playing)
        {
            BeginDrawing();
            ClearBackground(PURPLE);

            for(int i = 0;i<boardWidth;i++)
            {
                for(int j=0;j<boardHeight;j++)
                {
                    //rectangleBoard[i][j].color = YELLOW;
                    /*if(board[i][j].spaceInfo == Space::Mine)
                    {
                        board[i][j].color = RED;
                    }*/
                    /*else*/ if(board[i][j].spaceInfo == Space::Searched)
                    {
                        board[i][j].color = (Color){144,238,144,255};
                    }
                    else
                    {
                        board[i][j].color = GREEN;
                    }

                    if(board[i][j].flagged)
                    {
                        board[i][j].color = GRAY;
                    }
                }
            }

            int mousePosX = GetMouseX();
            int mousePosY = GetMouseY();

            mousePosX -= PlayWidth;
            mousePosY -= PlayHeight;
            if(mousePosX >=0 && mousePosY >=0)
            {
                mousePosX /= PlayWidthScale;
                mousePosY /= PlayHeightScale;
                //std::cout << mousePosX << std::endl;

                if(mousePosX >= 0 && mousePosX < boardWidth &&
                    mousePosY >= 0 && mousePosY < boardHeight)
                {
                    board[mousePosX][mousePosY].color = (Color){255,255,102,255};
                }

                if(IsMouseButtonReleased(MOUSE_RIGHT_BUTTON))
                {
                    if(mousePosX >= 0 && mousePosX < boardWidth &&
                    mousePosY >= 0 && mousePosY < boardHeight &&
                    board[mousePosX][mousePosY].spaceInfo != Space::Searched)
                    {
                        board[mousePosX][mousePosY].flagged = !board[mousePosX][mousePosY].flagged;
                    }
                }

                if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
                {
                    if(mousePosX >= 0 && mousePosX < boardWidth &&
                    mousePosY >= 0 && mousePosY < boardHeight && 
                    !board[mousePosX][mousePosY].flagged)
                    {
                        if(!minesGenerated)
                        {
                        generateMines(mousePosX, mousePosY, board, mineMax, mines);
                        minesGenerated = true;
                        searchSquare(mousePosX, mousePosY, board);
                        }
                        else
                        {
                            if(board[mousePosX][mousePosY].spaceInfo == Space::Mine)
                            {
                                gameState = gameStates::Loss;
                            }
                            searchSquare(mousePosX, mousePosY, board);

                            int spacesToSearch = 0;
                            for(int i=0;i<boardHeight;i++)
                            {
                                for(int j=0;j<boardWidth;j++)
                                {
                                    if(board[i][j].spaceInfo == Space::Empty)
                                    {
                                        spacesToSearch++;
                                    }
                                }
                            }
                            if(spacesToSearch == 0)
                            {
                                gameState = gameStates::Win;
                            }
                        }   
                    }
                    
                }
            }
            

            for(int i = 0;i<boardWidth;i++)
            {
                for(int j=0;j<boardHeight;j++)
                {
                    board[i][j].draw();
                }
            }
        EndDrawing();

        } else if(gameState == gameStates::Loss)
        {
            BeginDrawing();

            for(int i = 0;i<boardWidth;i++)
            {
                for(int j=0;j<boardHeight;j++)
                {
                    if(board[i][j].spaceInfo == Space::Mine)
                    {
                        board[i][j].color = RED;
                    }
                    board[i][j].draw();
                }
            }

            int loseFontSize = 64;
            std::string text = "you lose bitch";
            char char_array[text.length() + 1];
            strcpy(char_array, text.c_str());
            int offset = MeasureText(char_array, loseFontSize);
            DrawText(char_array, screenWidth/2-offset/2, screenHeight/2, loseFontSize, BLACK); 

            resetButton.draw();
            Vector2 mousePos = GetMousePosition();
            if(resetButton.isMouseOver(mousePos.x, mousePos.y) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            {
                reset();
            }

            EndDrawing();
        } else if(gameState == gameStates::Win)
        {
            BeginDrawing();
            int winFontSize = 64;
            std::string text = "you win :(";
            char char_array[text.length() + 1];
            strcpy(char_array, text.c_str());
            int offset = MeasureText(char_array, winFontSize);
            DrawText(char_array, (screenWidth/2)-offset/2, screenHeight/2, winFontSize, BLACK);

            resetButton.draw();
            Vector2 mousePos = GetMousePosition();
            if(resetButton.isMouseOver(mousePos.x, mousePos.y) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            {
                reset();
            }

            EndDrawing();
        }
        
    }

    CloseWindow();

}