#include "board.h"
#include <algorithm>
#include <cstdlib>
#include <ncurses.h>

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
    init_pair(WALL, 8, 8);
    init_pair(IMMUNE_WALL, 0, 0);
    init_pair(SNAKE_HEAD, 6, 6);
    init_pair(SNAKE_BODY, 2, 2);
}

Board::Board(int size) : size(size)
{
    user = Snake();
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
    timeout(500);
    noecho();

    print();
    int x = size / 2, y = size / 2;
    wmove(win, y, x);
    wrefresh(win);
    while (true)
    {
        int input = getch();
        switch (input)
        {
        case KEY_UP:
        case 'w':
            user.changeDirect(Direct::N);
            break;
        case KEY_DOWN:
        case 's':
            user.changeDirect(Direct::S);
            break;
        case KEY_LEFT:
        case 'a':
            user.changeDirect(Direct::W);
            break;
        case KEY_RIGHT:
        case 'd':
            user.changeDirect(Direct::E);
            break;
        case 'q':
            return EXIT_SUCCESS;
        }
        wrefresh(win);
    }
}

void Board::print()
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            wattron(win, COLOR_PAIR(board[i][j]));
            wmove(win, i, j * 2);
            waddch(win, ' ');
            wmove(win, i, j * 2 + 1);
            waddch(win, ' ');
            wattroff(win, COLOR_PAIR(board[i][j]));
        }
    }
    for (size_t i = 0; i < user.bodyLength; i++)
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
}