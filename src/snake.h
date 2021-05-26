#include "direct.h"
#include "point.h"
#include <iostream>
#include <ncurses.h>
#include <vector>

using namespace std;

class Snake
{
private:
    int bodyLength;
    vector<Point> body;
    Direct direct;
    bool isColide = false;

public:
    Snake()
    {
        bodyLength = 3;
        body.push_back(Point(0, 0));
    }

    void move();

    void changeDirect(Direct d) { direct = d; }
    bool isDead() { return isColide; }

    friend class Board;
};