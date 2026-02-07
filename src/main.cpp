#include "constants.h"
#include "menu.h"
#include "maze.h"
#include <raylib.h>

int main()
{
    using namespace GraphicsConstants;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Delta Maze");
    SetTargetFPS(TARGET_FPS);
    GameState state = GameState::MENU;
    Maze *myMaze = nullptr;

    while (!WindowShouldClose() && state != GameState::QUIT)
    {
        // 1. Evemt Handeling
        if (state == GameState::MENU)
        {
            UpdateMenu(state);
            myMaze = nullptr;
        }

        if (state == GameState::MAZE)
        {
            if (!myMaze)
            {
                myMaze = new Maze(mazeW, mazeH);
                myMaze->generateMaze();
            }
            updateMazeMenu(state);
        }

        // 3. Drawing
        BeginDrawing();

        ClearBackground(DARKGRAY);

        if (state == GameState::MENU)
        {
            DrawMenu();
        }

        if (state == GameState::MAZE)
        {
            if (myMaze)
            {
                myMaze->drawMaze();
            }
            drawMazeMenu();
        }

        // ClearBackground(green);
        // DrawCircle(ballX, ballY, 30, WHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}