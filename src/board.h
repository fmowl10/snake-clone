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
#define GATE short(7)

/**
 * @brief gate class
 * 
 */
class Gate
{
    Point gatePoints[2];
    int tick;
    int gateV;

    public:
        
        Gate(){ gateV = 0; tick = 40; }
        Gate(Point g1, Point g2, int v, int t): gateV(0), tick(t) { gatePoints[0] = g1; gatePoints[1] = g2; }
        
        Point checkExit(Point p, int len)
        {   
            tick += len;
            if(p == gatePoints[0]) return gatePoints[1];
            else return gatePoints[0];
        }

    friend class Board;
};

//item class
class Item{
    Point p;
    int itemV, tick;
    
    public:
        Item(Point point, int v, int t = 20): p(point), itemV(v), tick(t){}
        Item(){p = Point(0, 0); itemV = 0; tick = 20;}

    Item& operator=(const Item &item);

    friend class Board;
};


// deadcase
enum DeadCase
{
    ColideWall,
    ColideBody,
    OppositeWay,
    ShortBody
};

// enum to data
const string DeadMessage[4] = {
    "you hit the wall",
    "you hit the body",
    "you move to your body",
    "your body is too short"};

using namespace std;
class Board
{
    vector<vector<short>> board;
    vector<Point> walls;
    Item items[3];
    int tick[3] = {0, 0, 0};
    Gate gate;
    const int size;
    WINDOW *win;
    Snake user;
    static bool isInitColor;
    DeadCase deadCase;

    bool update();
    void print();
    void createItem(int num);
    void consumeItem(int num);
    void consumeItemTick();
    void enterGate(Point &);
    void createGate(int tick);
    void consumeGateTick();

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