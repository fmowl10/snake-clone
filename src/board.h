#include <exception>
#include <ncurses.h>
#include <vector>

#include "snake.h"

#ifndef __BOARD_H__
#define __BOARD_H__

// ncurese color pair
#define NONE_COLOR short(0)
#define WALL short(1)
#define IMMUNE_WALL short(2)
#define SNAKE_HEAD short(3)
#define SNAKE_BODY short(4)
#define GROWTH_ITEM short(5)
#define POISON_ITEM short(6)

//item class
class Item{
    Point p;
    int itemV;
    
    public:
        Item(Point point, int v): p(point), itemV(v){}
        Item(){ }

    Item& operator=(const Item &item);

    friend class Board;
};


// deadcase
enum DeadCase
{
    ColideWall,
    ColideBody,
    OppositeWay,
};

// enum to data
const string DeadMessage[3] = {
    "you hit the wall",
    "you hit the body",
    "you move to your body",
};

using namespace std;
class Board
{
    vector<vector<short>> board;
    Item items[3];
    const int size;
    WINDOW *win;
    Snake user;
    static bool isInitColor;

    bool update();
    void print();

    DeadCase deadCase;

public:
    Board(int size = 21);
    int loop();
    DeadCase why();
    static void initColor();
};

class BoardMiniumSizeException : public exception
{
public:
    const char *what();
};
#endif