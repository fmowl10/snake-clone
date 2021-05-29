/**
 * @file board.cpp
 * @author kim jea ha, kim jin seok
 * @brief 
 * @version 0.1
 * @date 2021-05-29
 * 
 * @copyright Copyright (c) 2021 cocozo, fmowl
 * 
 */
#include "board.h"
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ncurses.h>
#include <random>
#include <thread>

using namespace chrono;

Item &Item::operator=(const Item &item)
{
    p = item.p;
    itemV = item.itemV;
    tick = item.tick;
    return *this;
}

const char *BoardMiniumSizeException::what()
{
    return "too small board size, minium is 21";
}

bool Board::isInitColor = false;

void Board::initColor()
{
    if (isInitColor)
        return;
    start_color();
    init_color(6, 1000, 1000, 0);
    init_color(2, 0, 1000, 0);
    init_pair(WALL, 8, 8);
    init_pair(IMMUNE_WALL, 0, 0);
    init_pair(SNAKE_HEAD, 6, 6);
    init_pair(SNAKE_BODY, 2, 2);
    init_pair(GROWTH_ITEM, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(POISON_ITEM, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(GATE, COLOR_BLUE, COLOR_BLUE);
}

Board::Board(int size) : size(size), user(Snake(size))
{
    if (size < 20)
        throw BoardMiniumSizeException();
    win = newwin(size, size * 2, 2, 2);
    if (win == nullptr)
        throw exception();
    keypad(win, true);

    for (int i = 0; i < size; i++)
    {
        board.push_back({});
        for (int j = 0; j < size; j++)
        {
            int value = 0;
            if (i == 0 || i + 1 == size)
            {
                value = 1;
            }
            if (j == 0 || j + 1 == size)
            {
                value += 1;
            }
            if(value == 1) walls.push_back(Point(j, i));
            board[i].push_back(value);
            if (i == 0 || i + 1 == size)
            {
                value = 1;
            }
            value = 0;
        }
    }
}

int Board::loop()
{
    Board::initColor();
    timeout(50);
    noecho();

    print();
    while (true)
    {
        int input = getch();
        flushinp();
        Direct d = Direct::NONE;
        switch (input)
        {
        case KEY_UP:
        case 'w':
            d = Direct::N;
            break;
        case KEY_DOWN:
        case 's':
            d = Direct::S;
            break;
        case KEY_LEFT:
        case 'a':
            d = Direct::W;
            break;
        case KEY_RIGHT:
        case 'd':
            d = Direct::E;
            break;
        case 'q':
            return EXIT_SUCCESS;
        }
        if ((d ^ user.direct) == Direct::Opposite)
        {
            deadCase = DeadCase::OppositeWay;
            return EXIT_FAILURE;
        }
        if (d != Direct::NONE)
        {
            user.changeDirect(d);
        }
        if (update())
        {
            print();
        }
        else
        {
            return EXIT_FAILURE;
        }
        flushinp();
        this_thread::sleep_for(milliseconds(200));
    }
}

bool Board::update()
{
    user.move();
    if (user.isDead())
    {
        deadCase = DeadCase::ColideBody;
        return false;
    }
    Point head = user.body[0];
    if (1 <= board[head.y][head.x] && board[head.y][head.x] <= 2)
    {
        deadCase = DeadCase::ColideWall;
        return false;
    }

    if(board[head.y][head.x] == GATE) 
    {
       Point exit = gate.checkExit(head, user.bodyLength);
       // 벽이 위 가장자리인경우
       if(exit.y== 0) 
       {
          user.changeDirect(Direct::S);
          Point wayout = Point(exit.x, exit.y + 1);
          user.body[0] = wayout;
          
       }
       // 벽이 아래 가장자리인 경우
       else if(exit.y == size - 1) 
       {
            user.changeDirect(Direct::N);
            Point wayout = Point(exit.x, exit.y - 1);
            user.body[0] = wayout;
       }
       // 벽이 왼쪽 가장자리인 경우
       else if(exit.x == 0) 
       {
           user.changeDirect(Direct::E);
           Point wayout = Point(exit.x + 1, exit.y);
           user.body[0] = wayout;
       } 
       // 벽이 오른쪽 가장자리인 경우
       else if(exit.x == size - 1) 
       {
           user.changeDirect(Direct::W);
           Point wayout = Point(exit.x - 1, exit.y);
           user.body[0] = wayout;
       }

    }

    if(gate.gateV == 0)
    {
        if ((rand() % 100) + 1 <= 20)
        {   
            for(int i = 0; i < 2; i++)
            {
                int randWall = rand() % walls.size();
                gate.gatePoints[i] = walls[randWall];
                board[walls[randWall].y][walls[randWall].x] = GATE;
            }
            gate.gateV = 1;
            gate.tick = 40;
        }
    }
    else
    {
        gate.tick--;
        if(gate.tick <= 0)
        {
            gate.gateV = NONE_COLOR;
            for(int i = 0; i < 2; i++)
            {
                board[gate.gatePoints[i].y][gate.gatePoints[i].x] = WALL;
                gate.gatePoints[i].x = 0;
                gate.gatePoints[i].y = 0;
            }
        }
    }

    for (int i = 0; i < 3; i++)
    {

        if (items[i].itemV == 0)
        {
            if ((rand() % 100) + 1 <= 30)
            {
                int randX = (rand() % (size - 2)) + 1;
                int randY = (rand() % (size - 2)) + 1;
                int itemV = (rand() % 2) + 1;
                switch (itemV)
                {
                case 1:
                    board[randY][randX] = GROWTH_ITEM;
                    break;
                case 2:
                    board[randY][randX] = POISON_ITEM;
                }

                items[i] = Item(Point(randX, randY), itemV, 20);
            }
        }

        if (head == items[i].p)
        {
            switch (items[i].itemV)
            {
            case 1:
                user.growthBody();
                break;
            case 2:
                user.decreaseBody();
                break;
            }

            board[items[i].p.y][items[i].p.x] = NONE_COLOR;
            items[i].itemV = 0;
        }

        if (items[i].itemV == 0) continue;
        else
        {
            items[i].tick--;
            if (items[i].tick <= 0)
            {
                board[items[i].p.y][items[i].p.x] = NONE_COLOR;
                items[i].itemV = 0;
            }
        }    
    }

    return true;
}

void Board::print()
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            wattron(win, COLOR_PAIR(board[i][j]));
            wmove(win, i, j * 2);
            waddch(win, '.');
            wmove(win, i, j * 2 + 1);
            waddch(win, '.');
            wattroff(win, COLOR_PAIR(board[i][j]));
        }
    }
    for (int i = 0; i < user.bodyLength; i++)
    {
        short color = 0;
        // head
        if (i == 0)
        {
            color = SNAKE_HEAD;
        }
        else
        {
            color = SNAKE_BODY;
        }

        wattron(win, COLOR_PAIR(color));

        wmove(win, user.body[i].y, user.body[i].x * 2);
        waddch(win, ' ');

        wmove(win, user.body[i].y, user.body[i].x * 2 + 1);
        waddch(win, ' ');

        wattroff(win, COLOR_PAIR(color));
    }
    wrefresh(win);
}

DeadCase Board::why()
{
    return deadCase;
}