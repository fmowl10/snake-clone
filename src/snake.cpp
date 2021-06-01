/**
 * @file snake.cpp
 * @author Kim Jae Ha Kim Jin Seok
 * @brief 
 * @version 0.1
 * @date 2021-05-30
 * 
 * @copyright Copyright (c) 2021 cocozo fmowl
 * 
 */
#include "snake.h"
#include <iostream>

/**
 * @brief move snake body by direct
 * 
 */
void Snake::move()
{

    Point temp = body.front();
    previousTail = body.back();
    if (direct == N)
    {
        temp.y--;
    }
    else if (direct == S)
    {
        temp.y++;
    }
    else if (direct == W)
    {
        temp.x--;
    }
    else if (direct == E)
    {
        temp.x++;
    }
    for (int i = bodyLength - 1; i >= 1; i--)
    {
        body.at(i) = body.at(i - 1);
        if (body.at(i) == temp)
        {
            isColide = true;
            return;
        }
    }

    body.front() = temp;
}

/**
 * @brief add snake body from tail
 * 
 */
void Snake::growthBody()
{
    body.push_back(previousTail);
    bodyLength++;
}

/**
 * @brief remove snake body from tail
 * 
 */
void Snake::decreaseBody()
{
    body.pop_back();
    bodyLength--;
}

/**
 * @brief check snake condition is dead
 * 
 * @return true 
 * @return false 
 */
bool Snake::isDead()
{

    if (isColide)
        return true;
    if (bodyLength == 2)
        return true;
    return false;
}