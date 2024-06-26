# include "raylib.h"

int main()
{
    // window dimensions
    const int winWidth = 1200;
    const int winHeight = 800;

    InitWindow(winWidth, winHeight, "Dapper Dasher");

    //rectangle dimensions
    const int rectWidth = 50;
    const int rectHeight = 80;

    int posY = winHeight - rectHeight;
    int velocity = 0;
    int gravity = 1;

    SetTargetFPS(60);
    while(!WindowShouldClose())
    {
        // start drawing
        BeginDrawing();
        ClearBackground(WHITE);

        // perform ground check
        if(posY >= winHeight - rectHeight)
        {
            //rect is on the ground
            velocity = 0;
        }
        else
        {
            // rect is in the air
            velocity += gravity;
        }

        if(IsKeyPressed(KEY_SPACE))
        {
            velocity -= 10; 
        }

        // update pos
        posY += velocity;

        DrawRectangle(winWidth / 2, posY, rectWidth, rectHeight, BLUE);

        // stop drawing
        EndDrawing();
    }
    CloseWindow();
}