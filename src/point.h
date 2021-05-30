/**
 * @file point.h
 * @author Kim Jae Ha Kim Jin Seok
 * @brief 
 * @version 0.1
 * @date 2021-05-30
 * 
 * @copyright Copyright (c) 2021 cocozo fmowl
 * 
 */
#ifndef __POINT_H__
#define __POINT_H__
/**
 * @brief Point class
 * 
 */
class Point
{
    int x;
    int y;

public:
    /**
 * @brief Construct a new Point object
 * 
 * @param dx 
 * @param dy 
 */
    Point(int dx = 0, int dy = 0) : x(dx), y(dy) {}

    /**
 * @brief = operator
 * 
 * @param p 
 * @return Point& 
 */
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