#include "raylib.h"

#define SCREEN_WIDTH (GetScreenWidth())
#define SCREEN_HEIGHT (GetScreenHeight())

#define WINDOW_TITLE "Reaction Time Test"
#define NO_TIME_SET 1000000.0
#define HOUSE_RECORD "172.0 set by Ella"

int handleKeyPress(int key)
{
    if (key == KEY_F11)
    {
        ToggleFullscreen();

        return KEY_NULL;
    }

    else if (key == KEY_LEFT_ALT || key == KEY_TAB || key == KEY_LEFT_SUPER)
    {
        if (IsWindowFullscreen())
        {
            ToggleFullscreen();
        }

        return KEY_NULL;
    }

    return key;
}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    ToggleFullscreen();
    SetTargetFPS(0); // 0 to go as fast as possible

    Texture2D texture = LoadTexture(ASSETS_PATH"test.png"); // Check README.md for how this works
    bool hasPressed = true;
    bool hasPressSignalAppeared = false;
    bool isSignalTimerWaiting = false;
    double lastPressTime = 0;
    double timeUntilEvent = 0;
    double timeStartEvent = 0;
    double bestTime = NO_TIME_SET;

    while (!WindowShouldClose())
    {
        if (handleKeyPress(GetKeyPressed()) != 0)
        {
            double currentTime = GetTime();

            if (currentTime - lastPressTime < 1)
            {
                continue;
            }

            lastPressTime = currentTime;
            hasPressed = !hasPressed;

            if (!hasPressed)
            {
                timeUntilEvent = GetRandomValue(3000000, 9000000) / 1000000.0;
                timeStartEvent = lastPressTime;
                isSignalTimerWaiting = true;
            }

            // To clear out any keys still in the keys pressed queue
            // if you press multiple keys at once.
            while (handleKeyPress(GetKeyPressed()) != 0) {}
        }

        hasPressSignalAppeared = GetTime() - timeStartEvent >= timeUntilEvent && isSignalTimerWaiting;

        BeginDrawing();

        ClearBackground(RAYWHITE);
        const int texture_x = SCREEN_WIDTH / 2 - texture.width / 2;
        const int texture_y = SCREEN_HEIGHT / 2 - texture.height / 2;

        if (!hasPressed)
        {
            if (hasPressSignalAppeared)
            {
                DrawTexture(texture, texture_x, texture_y, WHITE);
            }
        }
        else
        {
            double bbtime = (lastPressTime - (timeStartEvent + timeUntilEvent)) * 1000.0;
            const char* text = TextFormat("Your reaction time is %f msec", bbtime);

            if (!hasPressSignalAppeared)
            {
                text = "Too early! Relax :)";
                isSignalTimerWaiting = false;
            }

            if (lastPressTime == 0)
            {
                text = "Press any button to start and react as fast as possible!";
            }

            const Vector2 text_size = MeasureTextEx(GetFontDefault(), text, 20, 1);
            DrawText(text, SCREEN_WIDTH / 2 - text_size.x / 2, texture_y + texture.height + text_size.y + 10, 20, BLACK);

            if (bbtime > 0 && bbtime < bestTime)
            {
                bestTime = bbtime;
            }
        }

        const char* text1 = TextFormat("Your best time is %f msec", bestTime);
        const Vector2 textSize1 = MeasureTextEx(GetFontDefault(), text1, 20, 1);
        
        if (bestTime > 0 && bestTime != NO_TIME_SET)
        {
            DrawText(text1, SCREEN_WIDTH / 2 - textSize1.x / 2, texture_y - textSize1.y + 10, 20, BLACK);
        }

        const char* text2 = TextFormat("The house record is a "HOUSE_RECORD);
        const Vector2 textSize2 = MeasureTextEx(GetFontDefault(), text2, 20, 1);
        DrawText(text2, SCREEN_WIDTH / 2 - textSize2.x / 2, texture_y - textSize2.y - textSize1.y, 20, GRAY);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
