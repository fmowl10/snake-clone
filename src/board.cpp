/**
 * @file board.cpp
 * @author kim jea ha, kim jin seok
 * @brief 
 * @version 0.1
 * @date 2021-05-29
 * 
 * @copyright Copyright (c) 2021 cocozo fmowl
 * 
 */
#include "board.h"
#include "mission.h"
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <ncurses.h>
#include <random>
#include <sstream>
#include <thread>

using namespace chrono;

/**
 * @brief operator 
 * 
 * @param item 
 * @return Item& 
 */
Item &Item::operator=(const Item &item)
{
    p = item.p;
    itemV = item.itemV;
    tick = item.tick;
    return *this;
}

/**
 * @brief returns exception message
 * 
 * @return const char* 
 */
const char *BoardMiniumSizeException::what()
{
    return "too small board size, minium is 21";
}

/**
 * @brief returns exception message
 * 
 * @return const char* 
 */
const char *InvalidMapException::what()
{
    return "invalid map file";
}

bool Board::isInitColor = false;

/**
 * @brief singletone method
 * 
 */
void Board::initColor()
{
    if (isInitColor)
        return;
    start_color();
    use_default_colors();
    init_color(6, 1000, 1000, 0);
    init_color(1, 1000, 500, 0);
    init_pair(WALL, 8, 8);
    init_pair(IMMUNE_WALL, 0, 0);
    init_pair(SNAKE_HEAD, 6, 6);
    init_pair(SNAKE_BODY, 1, 1);
    init_pair(GROWTH_ITEM, COLOR_GREEN, COLOR_GREEN);
    init_pair(POISON_ITEM, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(GATE, COLOR_BLUE, COLOR_BLUE);
    isInitColor = true;
}

/**
 * @brief Construct a new Board:: Board object
 * 
 * @param size 
 */
Board::Board(string file_name, WINDOW *map, WINDOW *missionBoard, WINDOW *statusBoard) : map(map), missionBoard(missionBoard), statusBoard(statusBoard)
{
    ifstream mapFile(file_name);

    keypad(map, true);
    keypad(missionBoard, true);
    keypad(statusBoard, true);

    wclear(missionBoard);
    wclear(statusBoard);

    wclear(map);

    vector<Point> snakeBody;
    char line[80] = {
        0,
    };
    int MaxBodyLen = 0;
    int NumberGrowth = 0;
    int NumberPoisonItem = 0;
    int NumberGate = 0;

    mapFile >> MaxBodyLen >> NumberGrowth >> NumberPoisonItem >> NumberGate;

    stageClear = Mission(MaxBodyLen, NumberGrowth, NumberPoisonItem, NumberGate);

    stageClear.currentMax = 3;

    if (!mapFile.is_open())
    {
        throw InvalidMapException();
    }

    while (!mapFile.eof())
    {
        mapFile >> line;
        board.push_back({});
        int i = 0;
        while (line[i] != '\0')
        {
            board.back().push_back((short)(line[i++] - '0'));
        }
    }

    size = board[0].size();

    if (size > 21)
    {
        throw BoardMiniumSizeException();
    }

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if ((board[i][j] == SNAKE_HEAD) && (snakeBody.empty()))
            {
                snakeBody.push_back(Point(j, i));
                board[i][j] = NONE_COLOR;
            }

            if (board[i][j] == SNAKE_BODY)
            {
                if (snakeBody.empty())
                {
                    throw exception();
                }
                else
                {
                    snakeBody.push_back(Point(j, i));
                    board[i][j] = NONE_COLOR;
                }
            }
            if (board[i][j] == WALL)
            {
                walls.push_back(Point(j, i));
            }
        }
    }
    user = Snake(snakeBody);
}

/**
 * @brief get input from user
 * 
 * @return int 
 */
int Board::loop()
{
    Board::initColor();
    timeout(50);
    noecho();

    print();
    while (true)
    {
        if (stageClear.isClear())
        {
            return EXIT_SUCCESS;
        }
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
            deadCase = DeadCase::UserGiveup;
            return EXIT_FAILURE;
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

/**
 * @brief event handler
 * 
 * @return true 
 * @return false 
 */
bool Board::update()
{
    user.move();
    if (user.isDead())
    {
        if (user.bodyLength <= 2)
            deadCase = DeadCase::ShortBody;
        else
            deadCase = DeadCase::ColideBody;
        return false;
    }
    Point head = user.body[0];
    if (1 <= board[head.y][head.x] && board[head.y][head.x] <= 2)
    {
        deadCase = DeadCase::ColideWall;
        return false;
    }

    // event snake enter a gate
    if (board[head.y][head.x] == GATE)
    {
        enterGate(head);
    }

    // event 게이트가 존자 하지 않을 경우
    if (gate.gateV == 0)
    {
        if ((rand() % 100) + 1 <= 20)
        {
            createGate(20);
        }
    }
    // event 게이트 존재시
    else
    {
        consumeGateTick();
    }

    //  event item attribute
    for (int i = 0; i < 3; i++)
    {

        if (items[i].itemV == 0)
        {
            if ((rand() % 100) + 1 <= 30)
            {
                createItem(i);
            }
        }

        if (head == items[i].p)
        {
            consumeItem(i);
        }
    }

    consumeItemTick();

    return true;
}

/**
 * @brief print current state
 * 
 */
void Board::print()
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            wattron(map, COLOR_PAIR(board[i][j]));
            wmove(map, i, j * 2);
            waddch(map, '.');
            wmove(map, i, j * 2 + 1);
            waddch(map, '.');
            wattroff(map, COLOR_PAIR(board[i][j]));
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

        wattron(map, COLOR_PAIR(color));

        wmove(map, user.body[i].y, user.body[i].x * 2);
        waddch(map, ' ');

        wmove(map, user.body[i].y, user.body[i].x * 2 + 1);
        waddch(map, ' ');

        wattroff(map, COLOR_PAIR(color));
    }
    box(statusBoard, 0, 0);
    box(missionBoard, 0, 0);

    mvwprintw(statusBoard, 0, 1, "Score Board");
    mvwprintw(statusBoard, 1, 1, "B : %2d/%d", user.bodyLength, stageClear.currentMax);
    mvwprintw(statusBoard, 2, 1, "+ : %2d", stageClear.consumedGrowthItem);
    mvwprintw(statusBoard, 3, 1, "- : %2d", stageClear.consumedPoisonItem);
    mvwprintw(statusBoard, 4, 1, "G : %2d", stageClear.GatePassed);

    mvwprintw(missionBoard, 0, 1, "Mission");
    mvwprintw(missionBoard, 1, 1, "B : %2d (%c)", stageClear.getMaxBodyLength(), stageClear.isPassMaxBodyLength() ? 'v' : ' ');
    mvwprintw(missionBoard, 2, 1, "+ : %2d (%c)", stageClear.getNumberGrothItem(), stageClear.isPassGrowthItem() ? 'v' : ' ');
    mvwprintw(missionBoard, 3, 1, "- : %2d (%c)", stageClear.getNumberPoisonItem(), stageClear.isPassPoisonItem() ? 'v' : ' ');
    mvwprintw(missionBoard, 4, 1, "G : %2d (%c)", stageClear.getNumberGate(), stageClear.isPassNumberGate() ? 'v' : ' ');

    wrefresh(map);
    wrefresh(statusBoard);
    wrefresh(missionBoard);
}

/**
 * @brief returns snake deadcase
 * 
 * @return DeadCase 
 */
DeadCase Board::why()
{
    return deadCase;
}

/**
 * @brief create item
 * 
 * @param num 
 */
void Board::createItem(int num)
{
    int randX = 0;
    int randY = 0;
    while (true)
    {
        randX = (rand() % (size - 2)) + 1;
        randY = (rand() % (size - 2)) + 1;

        if (board[randY][randX] == NONE_COLOR)
            break;
    }
    int itemV = (rand() % 2) + 1;
    switch (itemV)
    {
    case 1:
        board[randY][randX] = GROWTH_ITEM;
        break;
    case 2:
        board[randY][randX] = POISON_ITEM;
    }

    items[num] = Item(Point(randX, randY), itemV, 20);
}

/**
 * @brief consume item
 * 
 * @param num 
 */
void Board::consumeItem(int num)
{
    switch (items[num].itemV)
    {
    case 1:
        user.growthBody();
        stageClear.consumedGrowthItem++;
        if (stageClear.currentMax < user.bodyLength)
        {
            stageClear.currentMax = user.bodyLength;
        }
        break;
    case 2:
        user.decreaseBody();
        stageClear.consumedPoisonItem++;
        break;
    }

    board[items[num].p.y][items[num].p.x] = NONE_COLOR;
    items[num].itemV = 0;
}

/**
 * @brief consume item tick
 * 
 */
void Board::consumeItemTick()
{
    for (int i = 0; i < 3; i++)
    {
        if (items[i].itemV == 0)
            continue;
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
}

/**
 * @brief set snake's way out and direction
 * 
 * @param head 
 */
void Board::enterGate(Point &head)
{
    stageClear.GatePassed++;
    Point exit = gate.checkExit(head, user.bodyLength);
    Point wayout;
    // 벽이 위 가장자리인경우
    if (exit.y == 0)
    {
        user.changeDirect(Direct::S);
        wayout = Point(exit.x, exit.y + 1);
    }
    // 벽이 아래 가장자리인 경우
    else if (exit.y == size - 1)
    {
        user.changeDirect(Direct::N);
        wayout = Point(exit.x, exit.y - 1);
    }
    // 벽이 왼쪽 가장자리인 경우
    else if (exit.x == 0)
    {
        user.changeDirect(Direct::E);
        wayout = Point(exit.x + 1, exit.y);
    }
    // 벽이 오른쪽 가장자리인 경우
    else if (exit.x == size - 1)
    {
        user.changeDirect(Direct::W);
        wayout = Point(exit.x - 1, exit.y);
    }
    //벽이 가장자리가 아닐경우
    else
    {
        // user 의 방향 대로 나올 수 있는 경우
        switch (user.direct)
        {
        case Direct::N:
            if (find(passAble, passAble + 3, board[exit.y - 1][exit.x]))
            {
                wayout = Point(exit.x, exit.y - 1);
            }
            break;
        case Direct::S:
            if (find(passAble, passAble + 3, board[exit.y + 1][exit.x]))
            {
                wayout = Point(exit.x, exit.y + 1);
            }
            break;
        case Direct::E:
            if (find(passAble, passAble + 3, board[exit.y][exit.x + 1]))
            {
                wayout = Point(exit.x + 1, exit.y);
            }
            break;
        case Direct::W:
            if (find(passAble, passAble + 3, board[exit.y][exit.x - 1]))
            {
                wayout = Point(exit.x - 1, exit.y);
            }
            break;
        default:
            break;
        }

        //user의 방향대로 나갈 수 없을 경우, 시계방향으로 회전
        if (wayout == Point(0, 0))
        {
            switch (user.direct)
            {
            case Direct::N:
                if (find(passAble, passAble + 3, board[exit.y][exit.x + 1]))
                {
                    wayout = Point(exit.x + 1, exit.y);
                    user.changeDirect(Direct::E);
                }
                break;
            case Direct::S:
                if (find(passAble, passAble + 3, board[exit.y][exit.x - 1]))
                {
                    wayout = Point(exit.x - 1, exit.y);
                    user.changeDirect(Direct::W);
                }
                break;
            case Direct::E:
                if (find(passAble, passAble + 3, board[exit.y + 1][exit.x]))
                {
                    wayout = Point(exit.x, exit.y + 1);
                    user.changeDirect(Direct::S);
                }
                break;
            case Direct::W:
                if (find(passAble, passAble + 3, board[exit.y - 1][exit.x]))
                {
                    wayout = Point(exit.x, exit.y - 1);
                    user.changeDirect(Direct::N);
                }
                break;

            default:
                break;
            }
        }

        // 역시계 방향
        if (wayout == Point(0, 0))
        {
            switch (user.direct)
            {
            case Direct::N:
                if (find(passAble, passAble + 3, board[exit.y][exit.x - 1]))
                {
                    wayout = Point(exit.x - 1, exit.y);
                    user.changeDirect(Direct::W);
                }
                break;
            case Direct::S:
                if (find(passAble, passAble + 3, board[exit.y][exit.x + 1]))
                {
                    wayout = Point(exit.x + 1, exit.y);
                    user.changeDirect(Direct::E);
                }
                break;
            case Direct::E:
                if (find(passAble, passAble + 3, board[exit.y - 1][exit.x]))
                {
                    wayout = Point(exit.x, exit.y - 1);
                    user.changeDirect(Direct::N);
                }
                break;
            case Direct::W:
                if (find(passAble, passAble + 3, board[exit.y + 1][exit.x]))
                {
                    wayout = Point(exit.x, exit.y + 1);
                    user.changeDirect(Direct::S);
                }
                break;

            default:
                break;
            }
        }
        // 반대 방향
        if (wayout == Point(0, 0))
        {
            switch (user.direct)
            {
            case Direct::N:
                if (find(passAble, passAble + 3, board[exit.y + 1][exit.x]))
                {
                    wayout = Point(exit.x, exit.y + 1);
                    user.changeDirect(Direct::S);
                }
                break;
            case Direct::S:
                if (find(passAble, passAble + 3, board[exit.y - 1][exit.x]))
                {
                    wayout = Point(exit.x, exit.y - 1);
                    user.changeDirect(Direct::N);
                }
                break;
            case Direct::E:
                if (find(passAble, passAble + 3, board[exit.y][exit.x - 1]))
                {
                    wayout = Point(exit.x - 1, exit.y);
                    user.changeDirect(Direct::W);
                }
                break;
            case Direct::W:
                if (find(passAble, passAble + 3, board[exit.y][exit.x + 1]))
                {
                    wayout = Point(exit.x + 1, exit.y);
                    user.changeDirect(Direct::E);
                }
                break;

            default:
                break;
            }
        }
    }

    user.body[0] = wayout;
}

/**
 * @brief create gate
 * 
 * @param tick 
 */
void Board::createGate(int tick)
{
    for (int i = 0; i < 2; i++)
    {
        int randWall = rand() % walls.size();
        gate.gatePoints[i] = walls[randWall];
        board[walls[randWall].y][walls[randWall].x] = GATE;
    }
    gate.gateV = 1;
    gate.tick = tick;
}

/**
 * @brief consume gate's tick 
 * 
 */
void Board::consumeGateTick()
{
    gate.tick--;
    if (gate.tick <= 0)
    {
        gate.gateV = NONE_COLOR;
        for (int i = 0; i < 2; i++)
        {
            board[gate.gatePoints[i].y][gate.gatePoints[i].x] = WALL;
            gate.gatePoints[i].x = 0;
            gate.gatePoints[i].y = 0;
        }
    }
}