#include "snake.h"

void Snake::move()
{

    Point temp = body.front();

    if (direct == N)
    {
        temp.y++;
    }
    else if (direct == S)
    {
        temp.y--;
    }
    else if (direct == W)
    {
        temp.x++;
    }
    else if (direct == E)
    {
        temp.x--;
    }

    for (int i = 1; i < bodyLength; i++)
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
