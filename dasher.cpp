# include "raylib.h"

int main()
{
    // window dimensions
    const int winWidth{512};
    const int winHeight{380};

    InitWindow(winWidth, winHeight, "Dapper Dasher");

    // nebula vars
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    Rectangle nebRec{0.0, 0.0, nebula.width/8, nebula.height/8};
    Vector2 nebPos{winWidth, winHeight - nebRec.height};

    // nebula animations frame
    int nebFrame;
    const float nebUpdateTime{1.0/12.0};
    float nebRunningTime;

    // nebula x velocity (pixels/seconds)
    int nebVel{-200};

    // scaryf vars
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    Rectangle scarfyRec{0.0, 0.0, scarfy.width/6, scarfy.height};
    Vector2 scarfyPos{winWidth/2 - scarfyRec.width/2, winHeight - scarfyRec.height};

    //animation frame
    int frame;
    const float updateTime{1.0/12.0};
    float runningTime;

    int velocity{0};
    int gravity{1000}; // pixels/second
    bool isInAir{false};
    const int jumpVel{-600}; //  pixels/second

    SetTargetFPS(60);
    while(!WindowShouldClose())
    {
        const float dT{GetFrameTime()};
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
            velocity += gravity * dT;;
            isInAir = true;
        }

        // jump check
        if(IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity += jumpVel; 
        }

        // update nebula position
        nebPos.x += nebVel * dT;

        // update scarfy position
        scarfyPos.y += velocity * dT;

        // updating scarfy animation frame
        if(!isInAir)
        {
            runningTime += dT;
            if(runningTime >= updateTime)
            {
                runningTime = 0.0;
                // update animation frame
                scarfyRec.x = frame * scarfyRec.width;
                frame++;
                if(frame > 5)
                {
                    frame = 0;
                }
            }
        }

        // updating nebula animation frames
        nebRunningTime += dT;
        if(nebRunningTime >= nebUpdateTime)
        {
            nebRunningTime = 0.0;
            // update the frame
            nebRec.x = nebFrame * nebRec.width;
            nebFrame++;
            if(nebFrame > 7)
            {
                nebFrame = 0;
            }
        }

        // draw nebula
        DrawTextureRec(nebula, nebRec, nebPos, WHITE);

        // draw scarfy
        DrawTextureRec(scarfy, scarfyRec, scarfyPos, WHITE);

        // stop drawing
        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    CloseWindow();
}