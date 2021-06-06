/**
 * @file board.h
 * @author Kim Jae Ha Kim Jin Seok
 * @brief 
 * @version 0.1
 * @date 2021-05-30
 * 
 * @copyright Copyright (c) 2021 cocozo fmowl
 * 
 */

#include <exception>
#include <ncurses.h>
#include <vector>

#include "mission.h"
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

const short passAble[3] = {NONE_COLOR, GROWTH_ITEM, POISON_ITEM};

/**
 * @brief Gate class
 * 
 */
class Gate
{
    Point gatePoints[2];
    int tick;
    int gateV;

public:
    /**
    * @brief Construct a new Gate object
    * 
    */
    Gate()
    {
        gateV = 0;
        tick = 10;
    }
    /**
     * @brief Construct a new Gate object
     * 
     * @param g1 
     * @param g2 
     * @param v 
     * @param t 
     */
    Gate(Point g1, Point g2, int v, int t) : gateV(0), tick(t)
    {
        gatePoints[0] = g1;
        gatePoints[1] = g2;
    }

    /**
     * @brief check is exit
     * 
     * @param p 
     * @param len 
     * @return Point 
     */
    Point checkExit(Point p, int len)
    {
        tick += len;
        if (p == gatePoints[0])
            return gatePoints[1];
        else
            return gatePoints[0];
    }

    friend class Board;
};

/**
 * @brief Item class
 * 
 */
class Item
{
    Point p;
    int itemV, tick;

public:
    /**
     * @brief Construct a new Item object
     * 
     * @param point 
     * @param v 
     * @param t 
     */
    Item(Point point, int v, int t = 20) : p(point), itemV(v), tick(t) {}
    Item()
    {
        p = Point(0, 0);
        itemV = 0;
        tick = 20;
    }

    Item &operator=(const Item &item);

    friend class Board;
};

// deadcase
enum DeadCase
{
    ColideWall,
    ColideBody,
    OppositeWay,
    ShortBody,
    UserGiveup,
};

// enum to data
const string DeadMessage[] = {
    "you hit the wall",
    "you hit the body",
    "you move to your body",
    "your body is too short",
    "you give up",
};

using namespace std;
class Board
{
    vector<vector<short>> board;
    vector<Point> walls;
    Item items[3];
    int size;
    Gate gate;
    WINDOW *map;
    WINDOW *missionBoard;
    WINDOW *statusBoard;
    Snake user;
    static bool isInitColor;
    DeadCase deadCase;

    Mission stageClear;

    bool update();
    void print();
    void createItem(int num);
    void consumeItem(int num);
    void consumeItemTick();
    void enterGate(Point &);
    void createGate(int tick);
    void consumeGateTick();

public:
    Board(string file_name, WINDOW *map, WINDOW *missionBoard, WINDOW *statusBoard);
    int loop();
    DeadCase why();
    static void initColor();
};

class BoardMiniumSizeException : public exception
{
public:
    const char *what();
};

class InvalidMapException : public exception
{
public:
    const char *what();
};

#endif