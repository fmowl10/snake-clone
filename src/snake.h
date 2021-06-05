/**
 * @file snake.h
 * @author kim jae ha 
 * @brief snake class 
 * @version 0.1
 * @date 2021-05-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "direct.h"
#include "point.h"
#include <iostream>
#include <ncurses.h>
#include <vector>

using namespace std;

#ifndef __SNAKE_H__
#define __SNAKE_H__

/**
 * @brief Snake class
 * 
 */
class Snake
{
private:
    int bodyLength;
    vector<Point> body;
    Direct direct;
    bool isColide = false;
    Point previousTail;

public:
    /**
    * @brief Construct a new Snake object
    * 
    * @param size 
    */
    Snake(){ }
    Snake(vector<Point>& sBody)
    {
        body = sBody;
        bodyLength = sBody.size();
        direct = Direct::W;
    }

    void move();

    void changeDirect(Direct d) { direct = d; }
    bool isDead();

    void growthBody();
    void decreaseBody();

    friend class Board;
};
#endif