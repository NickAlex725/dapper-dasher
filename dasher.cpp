# include "raylib.h"

struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnGround(AnimData data, int windowHeight)
{
     return data.pos.y >= windowHeight - data.rec.height;
}

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
    // update running time
    data.runningTime += deltaTime;
    if(data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0;
        // update animation frame
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if(data.frame > maxFrame)
        {
            data.frame = 0;
        }
    }
    return data;
}

int main()
{
    // window dimensions
    int windowDimension[2];
    windowDimension[0] = 512;
    windowDimension[1] = 380;

    InitWindow(windowDimension[0], windowDimension[1], "Dapper Dasher");

    // nebula vars
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

    const int sizeOfNebulae{6};
    const int nebSpacing = 300;
    AnimData nebulae[sizeOfNebulae]{};

    for (int i = 0; i < sizeOfNebulae; i++)
    {
        nebulae[i].rec.x = 0.0;
        nebulae[i].rec.y = 0.0;
        nebulae[i].rec.width = nebula.width/8;
        nebulae[i].rec.height = nebula.height/8;
        nebulae[i].pos.x = windowDimension[0] + (i * nebSpacing);
        nebulae[i].pos.y = windowDimension[1] - nebula.height/8;
        nebulae[i].frame = 0;
        nebulae[i].runningTime = 0.0;
        nebulae[i].updateTime = 1.0/16.0;
    }

    float finishLine{nebulae[sizeOfNebulae - 1].pos.x};

    // nebula x velocity (pixels/seconds)
    int nebVel{-200};

    // scaryf vars
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    AnimData scarfyData;
    scarfyData.rec.width = scarfy.width/6;
    scarfyData.rec.height = scarfy.height;
    scarfyData.rec.x = 0;
    scarfyData.rec.y = 0;
    scarfyData.pos = {windowDimension[0]/2 - scarfyData.rec.width/2, windowDimension[1] - scarfyData.rec.height};
    scarfyData.frame = 0;
    scarfyData.updateTime = 1.0/12.0;
    scarfyData.runningTime = 0.0;

    int velocity{0};
    int gravity{1000}; // pixels/second
    bool isInAir{false};
    const int jumpVel{-600}; //  pixels/second

    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bgX{};
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float mgX{};
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float fgX{};

    bool collision{};

    SetTargetFPS(60);
    while(!WindowShouldClose())
    {
        const float dT{GetFrameTime()};
        // start drawing
        BeginDrawing();
        ClearBackground(WHITE);

        // scroll the background
        bgX -= 20 * dT;
        if(bgX <= -background.width * 2)
        {
            bgX = 0.0;
        }
        // scroll the midground
        mgX -= 40 * dT;
        if(mgX <= -midground.width * 2)
        {
            mgX = 0.0;
        }
        // scroll the foreground
        fgX -= 80 * dT;
        if(fgX <= -foreground.width * 2)
        {
            fgX = 0.0;
        }

        //draw background
        Vector2 bg1Pos{bgX, 0.0};
        DrawTextureEx(background, bg1Pos, 0.0, 2.0, WHITE);
        Vector2 bg2Pos{bgX + background.width * 2, 0.0};
        DrawTextureEx(background, bg2Pos, 0.0, 2.0, WHITE);

        // draw the midground
        Vector2 mg1Pos{mgX, 0.0};
        DrawTextureEx(midground, mg1Pos, 0.0, 2.0, WHITE);
        Vector2 mg2Pos{mgX + midground.width * 2, 0.0};
        DrawTextureEx(midground, mg2Pos, 0.0, 2.0, WHITE);

        // draw the foreground
        Vector2 fg1Pos{fgX, 0.0};
        DrawTextureEx(foreground, fg1Pos, 0.0, 2.0, WHITE);
        Vector2 fg2Pos{fgX + foreground.width * 2, 0.0};
        DrawTextureEx(foreground, fg2Pos, 0.0, 2.0, WHITE);

        // perform ground check
        if(isOnGround(scarfyData, windowDimension[1]))
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

        // update nebs pos
        for(int i = 0; i < sizeOfNebulae; i++)
        {   
            nebulae[i].pos.x += nebVel * dT;
        }

        // update finishline pos
        finishLine += nebVel * dT;

        // update scarfy position
        scarfyData.pos.y += velocity * dT;

        // updating scarfy animation frame
        if(!isInAir)
        {
            scarfyData = updateAnimData(scarfyData, dT, 5);
        }

        // updating nebula animation frames
        for (int i = 0; i < sizeOfNebulae; i++)
        {
            nebulae[i] = updateAnimData(nebulae[i], dT, 7);
        }

        for (AnimData nebula : nebulae)
        {
            float pad{50};
            Rectangle nebRec{
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rec.width - (2 * pad),
                nebula.rec.height - (2 * pad)
            };
            Rectangle scarfyRec{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height
            };
            if(CheckCollisionRecs(nebRec, scarfyRec))
            {
                collision = true;
            }
        }

        if(collision)
        {
            // lose game
            DrawText("Game Over!", windowDimension[0] / 4, windowDimension[1] / 2, 40, RED);
        }
        else if(scarfyData.pos.x >= finishLine)
        {
            // winner!
            DrawText("You Win!", windowDimension[0] / 4, windowDimension[1] / 2, 40, GREEN);
        }
        else
        {
            // draw nebulas
            for (int i = 0; i < sizeOfNebulae; i++)
            {
                DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
            }

            // draw scarfy
            DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
        }

        // stop drawing
        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();
}