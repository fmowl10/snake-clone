#include "direct.h"
#include "point.h"
#include <iostream>
#include <ncurses.h>
#include <vector>

using namespace std;

#ifndef __SNAKE_H__
#define __SNAKE_H__
class Snake
{
private:
    int bodyLength;
    vector<Point> body;
    Direct direct;
    bool isColide = false;

public:
    Snake(int size)
    {
        bodyLength = 3;
        for(int i = 0; i < 3; i++){
            body.push_back(Point((size/2)+i, (size/2)));
        }
    }

    void move();

    void changeDirect(Direct d) { direct = d; }
    bool isDead() { return isColide; }

    friend class Board;
};
#endif