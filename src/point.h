#ifndef __POINT_H__
#define __POINT_H__
class Point
{
    int x;
    int y;

    Point(int dx, int dy) : x(dx), y(dy) {}

    Point &operator=(const Point &p)
    {
        x = p.x;
        y = p.y;
        return *this;
    }
    bool operator==(const Point &p)
    {
        if (x == p.x && y == p.y)
            return true;
        else
            return false;
    }
    friend class Snake;
    friend class Board;
};
#endif