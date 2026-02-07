#include <raylib.h>
#include "constants.h"
#include "maze.h"
#include "menu.h"

#include <vector>
#include <random>
#include <unordered_set>
#include <algorithm>
#include <iostream>

using namespace std;

Maze::Maze(int width, int height) : cols_(width), rows_(height), grid_(height, vector<CellType>(width, CellType::WALL)) {}

// NOTE: draw grid starting from 200, 100
// NOTE: maze generation using Wilson's algorithm
void Maze::generateMaze()
{
    if (cols_ < 5 || rows_ < 5)
        return;

    // randomizor
    random_device rd;
    mt19937 rng(rd());

    unordered_set<pair<int, int>, pairHash> rooms;
    unordered_set<pair<int, int>, pairHash> visited;

    // Set rooms
    for (int i = 1; i < rows_; i += 2)
    {
        for (int j = 1; j < cols_; j += 2)
        {
            grid_[i][j] = CellType::ROOM;
            pair<int, int> r = {i, j};
            rooms.insert(r);
        }
    }

    grid_[1][1] = CellType::PATH;
    rooms.erase({1, 1});
    visited.insert({1, 1});

    // random walk
    while (!rooms.empty())
    {
        pair<int, int> current = pickRandomPairFromSet(rooms, rng);

        vector<pair<int, int>> walk;
        unordered_map<pair<int, int>, int, pairHash> posIndex;

        walk.push_back(current);
        posIndex[current] = 0;

        while (visited.find(current) == visited.end())
        {
            vector<pair<int, int>> neighborRooms;
            int y = current.first;
            int x = current.second;

            if (y - 2 >= 1)
                neighborRooms.push_back({y - 2, x});
            if (y + 2 < rows_)
                neighborRooms.push_back({y + 2, x});
            if (x - 2 >= 1)
                neighborRooms.push_back({y, x - 2});
            if (x + 2 < cols_)
                neighborRooms.push_back({y, x + 2});

            uniform_int_distribution<int> dist(0, neighborRooms.size() - 1);
            pair<int, int> next = neighborRooms[dist(rng)];

            if (posIndex.find(next) != posIndex.end())
            {
                // remove loop
                int idx = posIndex[next];
                walk.resize(idx + 1);
                for (auto it = posIndex.begin(); it != posIndex.end();)
                {
                    if (it->second > idx)
                    {
                        it = posIndex.erase(it);
                    }
                    else
                    {
                        ++it;
                    }
                }
            }
            else
            {
                walk.push_back(next);
                posIndex[next] = walk.size() - 1;
            }

            current = next;
        }

        // commit path
        for (size_t i = 0; i < walk.size(); ++i)
        {
            pair<int, int> room = walk[i];
            grid_[room.first][room.second] = CellType::PATH;
            visited.insert(room);
            rooms.erase(room);

            if (i > 0)
            {
                int wy = (walk[i - 1].first + room.first) / 2;
                int wx = (walk[i - 1].second + room.second) / 2;
                grid_[wy][wx] = CellType::PATH;
            }
        }
    }

    // Set starting and ending path after generation
    grid_[0][1] = CellType::PATH;
    grid_[rows_ - 1][cols_ - 2] = CellType::PATH;
}

pair<int, int> Maze::pickRandomPairFromSet(const unordered_set<pair<int, int>, pairHash> &r, mt19937 &rng)
{
    uniform_int_distribution<int> dist(0, r.size() - 1);

    int k = dist(rng);
    auto it = r.begin();
    advance(it, k);

    return *it;
}

void Maze::drawMaze()
{
    // 680 x 680 size
    cout << "Drawing Maze\n";
    pair<int, int> mazeBoundStart = {60, 60};
    // pair<int, int> mazeBoundEnd = {980, 780};
    // int displayWidth = mazeBoundEnd.first - mazeBoundStart.first;
    // int displayHeight = mazeBoundEnd.second - mazeBoundStart.second;
    pair<int, int> cellSize = {20, 20};
    //  grid_(height, vector<CellType>(width, CellType::WALL)
    for (int i = 0; i < rows_; ++i)
    {
        for (int j = 0; j < cols_; ++j)
        {
            if (grid_[i][j] == CellType::PATH)
            {
                DrawRectangle(cellSize.first * j + mazeBoundStart.first, cellSize.second * i + mazeBoundStart.second, cellSize.first, cellSize.second, WHITE);
            }
            else
            {
                DrawRectangle(cellSize.first * j + mazeBoundStart.first, cellSize.second * i + mazeBoundStart.second, cellSize.first, cellSize.second, BLACK);
            }
        }
    }
}

// NOTE: Maze Menu stuff
Rectangle backButton = {0, 0, 120, 50};
bool mouseOnBackBtn = false;

void drawMazeMenu()
{
    DrawRectangleRec(backButton, BLACK);
    DrawText("Back", 5, 5, 40, WHITE);
}

void updateMazeMenu(GameState &state)
{
    mouseOnBackBtn = false;
    if (CheckCollisionPointRec(GetMousePosition(), backButton))
        mouseOnBackBtn = true;

    if (mouseOnBackBtn)
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            state = GameState::MENU;
        }
    }
}
