#include "snake.h"
#include <iostream>

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


void Snake::growthBody()
{
    body.push_back(previousTail);
    bodyLength++;
}

void Snake::decreaseBody()
{
    body.pop_back();
    bodyLength--;
}

bool Snake::isDead(){

    if(isColide) return true;
    if(bodyLength == 2) return true;
    return false;
}