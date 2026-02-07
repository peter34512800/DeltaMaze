#ifndef MAZE_H
#define MAZE_H

#include "menu.h"
#include <cstddef>
#include <vector>
#include <utility>
#include <unordered_set>
#include <random>

using namespace std;

enum class CellType
{
    WALL,
    ROOM,
    PATH
};

struct pairHash
{
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2> &p) const
    {
        return hash<T1>()(p.first) ^ hash<T2>()(p.second);
    }
};

class Maze
{
public:
    // width = cols, height = rows
    Maze(int width, int height);
    void generateMaze();

    void drawMaze();

    int width() const { return cols_; }
    int height() const { return rows_; }

private:
    int cols_;
    int rows_;

    vector<vector<CellType>> grid_;

    pair<int, int> pickRandomPairFromSet(const unordered_set<pair<int, int>, pairHash> &r, mt19937 &rng);
};

void drawMazeMenu();
void updateMazeMenu(GameState &state);

#endif