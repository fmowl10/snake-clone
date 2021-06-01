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
#include <ncurses.h>
#include <random>
#include <thread>
#include <fstream>
#include <sstream>

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
Board::Board(string file_name, WINDOW* map, WINDOW *mission_board, WINDOW *status_board) :map(map), mission_board(mission_board), status_board(status_board)
{   
    ifstream mapFile(file_name);
    keypad(map, true);
    keypad(mission_board, true);
    keypad(status_board, true);

    vector<Point> snakeBody;
    char line[80] = {0,};
    mapFile >> stageClear.MaxBodyLength >> stageClear.NumberGrowthItem >> stageClear.NumberPoisonItem >> stageClear.NumberGate;
    if(!mapFile.is_open()){ throw exception(); }


    while(!mapFile.eof())
    {
        mapFile>>line;
        board.push_back({});
        int i = 0;
        while(line[i] != '\0')
        {
            board.back().push_back((short)(line[i++] - '0'));
        }
    }

    size = board[0].size();

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if((board[i][j] == SNAKE_HEAD) && (snakeBody.empty()))
            {
                snakeBody.push_back(Point(j, i));
                board[i][j] = NONE_COLOR;
            }

            if(board[i][j] == SNAKE_BODY)
            {   
                if(snakeBody.empty())
                {
                    throw exception();
                }
                else
                {
                    snakeBody.push_back(Point(j, i));
                    board[i][j] = NONE_COLOR;
                }
            }          
            if(board[i][j] == WALL) {
                walls.push_back(Point(j,i));
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
        if(stageClear.isClear()) {
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
        stageClear.NumberGate--;
        if(stageClear.NumberGate == 0)
            stageClear.isPassGate = true;
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
            switch (items[i].itemV) {
                case 2:
                    stageClear.NumberPoisonItem--;
                    if(stageClear.NumberPoisonItem == 0) 
                    {
                        stageClear.isPassPoison = true;
                    }
                    break;
                case 1:
                    stageClear.NumberGrowthItem--;
                    if(stageClear.NumberGrowthItem == 0)
                    {
                        stageClear.isPassGrowth = true;
                    }
                    break;
            }
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
    wrefresh(map);
    wmove(status_board, 0,0);
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

        // lambda 식에 맞는 wall이 존재하면 못나감
        auto i = find_if(walls.begin(), walls.end(), [randX, randY](Point &x)
                         { return x == Point(randX, randY); });
        if (i == walls.end())
        {
            break;
        }
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
        break;
    case 2:
        user.decreaseBody();
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
            if (board[exit.y - 1][exit.x] != WALL)
            {
                wayout = Point(exit.x, exit.y - 1);
            }
            break;
        case Direct::S:
            if (board[exit.y + 1][exit.x] != WALL)
            {
                wayout = Point(exit.x, exit.y + 1);
            }
            break;
        case Direct::E:
            if (board[exit.y][exit.x + 1] != WALL)
            {
                wayout = Point(exit.x + 1, exit.y);
            }
            break;
        case Direct::W:
            if (board[exit.y][exit.x - 1] != WALL)
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
                if (board[exit.y][exit.x + 1] != WALL)
                {
                    wayout = Point(exit.x + 1, exit.y);
                    user.changeDirect(Direct::E);
                }
                break;
            case Direct::S:
                if (board[exit.y][exit.x - 1] != WALL)
                {
                    wayout = Point(exit.x - 1, exit.y);
                    user.changeDirect(Direct::W);
                }
                break;
            case Direct::E:
                if (board[exit.y + 1][exit.x] != WALL)
                {
                    wayout = Point(exit.x, exit.y + 1);
                    user.changeDirect(Direct::S);
                }
                break;
            case Direct::W:
                if (board[exit.y - 1][exit.x] != WALL)
                {
                    wayout = Point(exit.x, exit.y - 1);
                    user.changeDirect(Direct::N);
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