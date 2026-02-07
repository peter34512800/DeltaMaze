#ifndef MENU_H
#define MENU_H

#include "raylib.h"

enum class GameState
{
    MENU,
    MAZE,
    QUIT
};

#pragma once
extern int mazeW;
extern int mazeH;

void UpdateMenu(GameState &state);
void DrawMenu();

#endif