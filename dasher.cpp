# include "raylib.h"

int main()
{
    // window dimensions
    const int winWidth = 512;
    const int winHeight = 380;

    InitWindow(winWidth, winHeight, "Dapper Dasher");

    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    Rectangle scarfyRec;
    scarfyRec.width = scarfy.width/6;
    scarfyRec.height = scarfy.height;
    scarfyRec.x = 0;
    scarfyRec.y = 0;
    Vector2 scarfyPos;
    scarfyPos.x = winWidth/2 - scarfyRec.width/2;
    scarfyPos.y = winHeight - scarfyRec.height;

    int velocity = 0;
    int gravity = 1;
    bool isInAir = false;
    const int jumpVel = -22;

    SetTargetFPS(60);
    while(!WindowShouldClose())
    {
        // start drawing
        BeginDrawing();
        ClearBackground(WHITE);

        // perform ground check
        if(scarfyPos.y >= winHeight - scarfyRec.height)
        {
            //rect is on the ground
            velocity = 0;
            isInAir = false;
        }
        else
        {
            // rect is in the air
            velocity += gravity;
            isInAir = true;
        }

        if(IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity += jumpVel; 
        }

        // update pos
        scarfyPos.y += velocity;

        DrawTextureRec(scarfy, scarfyRec, scarfyPos, WHITE);

        // stop drawing
        EndDrawing();
    }
    UnloadTexture(scarfy);
    CloseWindow();
}