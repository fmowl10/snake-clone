#ifndef __BOARD_H__
#define __BOARD_H__
//#include "cursor.h"
#include <exception>
#include <ncurses.h>
#include <vector>

#define WALL short(1)
#define IMMUNE_WALL short(2)
#define SNAKE_HEAD short(3)
#define SNAKE_BODY short(4)

using namespace std;
class Board
{
    vector<vector<int>> board;
    const int size;
    WINDOW *win;
    //cursor user;
    static bool isInitColor;

    bool update();
    bool print();

public:
    Board(int size = 21);
    int loop();
    static void initColor();
};

class BoardMiniumSizeException : public exception
{
public:
    const char *what();
};
#endif