#include <string>
#include <raylib.h>
#include "menu.h"
#include "constants.h"

using namespace std;
using namespace MenuConstants;

// width, height, x, y
Rectangle wTextBox = {GraphicsConstants::SCREEN_WIDTH / 2 - 225 / 2, 180, 225, 50};
Rectangle hTextBox = {GraphicsConstants::SCREEN_WIDTH / 2 - 225 / 2, 300, 225, 50};
Rectangle playButton = {GraphicsConstants::SCREEN_WIDTH / 2 - 225 / 2, 400, 200, 40};
Rectangle quitButton = {GraphicsConstants::SCREEN_WIDTH / 2 - 225 / 2, 460, 200, 40};

bool mouseOnWText = false;
bool mouseOnHText = false;

bool mouseOnPlayBtn = false;
bool mouseOnQuitBtn = false;

int wTextCount = 0;
int hTextCount = 0;

char mazeWidth[SIZE_TEXT_MAX_CHARS + 1] = "\0";
char mazeHeight[SIZE_TEXT_MAX_CHARS + 1] = "\0";

int mazeW = 0;
int mazeH = 0;

bool oversizeWarning = false;

int framesCounter = 0;

void UpdateMenu(GameState &state)
{
    ClearBackground(BLACK);
    // NOTE: Text boxes
    mouseOnWText = false;
    mouseOnHText = false;

    if (CheckCollisionPointRec(GetMousePosition(), wTextBox))
        mouseOnWText = true;
    if (CheckCollisionPointRec(GetMousePosition(), hTextBox))
        mouseOnHText = true;

    if (mouseOnHText || mouseOnWText)
    {
        SetMouseCursor(MOUSE_CURSOR_IBEAM); // change curser skin
        int key = GetCharPressed();         // get input

        while (key > 0)
        {
            if ((key >= 48) && (key <= 57))
            {
                if (mouseOnWText && (wTextCount < SIZE_TEXT_MAX_CHARS))
                {
                    mazeWidth[wTextCount] = (char)key;
                    mazeWidth[wTextCount + 1] = '\0';
                    mazeW = stoi(mazeWidth);
                    wTextCount++;
                }

                if (mouseOnHText && (hTextCount < SIZE_TEXT_MAX_CHARS))
                {
                    mazeHeight[hTextCount] = (char)key;
                    mazeHeight[hTextCount + 1] = '\0';
                    mazeH = stoi(mazeHeight);
                    hTextCount++;
                }
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE))
        {
            if (mouseOnWText)
            {
                wTextCount--;
                if (wTextCount < 0)
                    wTextCount = 0;
                mazeWidth[wTextCount] = '\0';
            }

            if (mouseOnHText)
            {
                hTextCount--;
                if (hTextCount < 0)
                    hTextCount = 0;
                mazeHeight[hTextCount] = '\0';
            }
        }
    }
    else
    {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }

    if (mouseOnHText || mouseOnWText)
    {
        framesCounter++;
    }
    else
    {
        framesCounter = 0;
    }

    // NOTE: Buttons
    mouseOnPlayBtn = false;
    mouseOnQuitBtn = false;

    if (CheckCollisionPointRec(GetMousePosition(), playButton))
        mouseOnPlayBtn = true;
    if (CheckCollisionPointRec(GetMousePosition(), quitButton))
        mouseOnQuitBtn = true;

    if (mouseOnPlayBtn)
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {

            if (mazeW > MazeConstants::MAX_WIDTH || mazeW < 5 || mazeH > MazeConstants::MAX_HEIGHT || mazeH < 5 || mazeH % 2 == 0 || mazeW % 2 == 0)
            {
                oversizeWarning = true;
            }
            else
            {
                state = GameState::MAZE;
                oversizeWarning = false;
            }
        }
    }

    if (oversizeWarning)
    {
        DrawText("Height and width must be no larger then 60/35\n or smaller then 5 or even.", GraphicsConstants::SCREEN_WIDTH / 2 - 225 / 2, 500, 20, RED);
    }

    if (mouseOnQuitBtn)
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            state = GameState::QUIT;
        }
    }
}

void DrawMenu()
{
    // NOTE: Draw text boxes
    DrawText("Maze Width:", GraphicsConstants::SCREEN_WIDTH / 2 - 225 / 2, 140, 20, WHITE);
    DrawRectangleRec(wTextBox, LIGHTGRAY);
    DrawText("Maze Height:", GraphicsConstants::SCREEN_WIDTH / 2 - 225 / 2, 260, 20, WHITE);
    DrawRectangleRec(hTextBox, LIGHTGRAY);

    if (mouseOnWText)
    {
        DrawRectangleLines((int)wTextBox.x, (int)wTextBox.y, (int)wTextBox.width, (int)wTextBox.height, RED);
    }
    else
    {
        DrawRectangleLines((int)wTextBox.x, (int)wTextBox.y, (int)wTextBox.width, (int)wTextBox.height, GRAY);
    }

    if (mouseOnHText)
    {
        DrawRectangleLines((int)hTextBox.x, (int)hTextBox.y, (int)hTextBox.width, (int)hTextBox.height, RED);
    }
    else
    {
        DrawRectangleLines((int)hTextBox.x, (int)hTextBox.y, (int)hTextBox.width, (int)hTextBox.height, GRAY);
    }

    // NOTE: draw textbox text
    DrawText(mazeWidth, (int)wTextBox.x + 5, (int)wTextBox.y + 8, 40, MAROON);
    DrawText(mazeHeight, (int)hTextBox.x + 5, (int)hTextBox.y + 8, 40, MAROON);

    if (mouseOnWText)
    {
        if (wTextCount < SIZE_TEXT_MAX_CHARS)
        {
            if (((framesCounter / 20) % 2) == 0)
                DrawText("_", (int)wTextBox.x + 8 + MeasureText(mazeWidth, 40), (int)wTextBox.y + 12, 40, MAROON);
        }
    }

    if (mouseOnHText)
    {
        if (hTextCount < SIZE_TEXT_MAX_CHARS)
        {
            if (((framesCounter / 20) % 2) == 0)
                DrawText("_", (int)hTextBox.x + 8 + MeasureText(mazeHeight, 40), (int)hTextBox.y + 12, 40, MAROON);
        }
    }

    // NOTE: Draw buttons
    DrawRectangleRec(playButton, LIME);
    DrawText("Generate", GraphicsConstants::SCREEN_WIDTH / 2 - 225 / 2 + 5, 405, 40, BLACK);
    DrawRectangleRec(quitButton, RED);
    DrawText("Quit", GraphicsConstants::SCREEN_WIDTH / 2 - 225 / 2 + 5, 465, 40, BLACK);
}