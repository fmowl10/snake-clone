#include "board.h"
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
    init_pair(SNAKE_BODY, 6, 6);
    init_pair(SNAKE_BODY, 2, 2);
}

Board::Board(int size) : size(size)
{
    if (size < 20)
        throw BoardMiniumSizeException();
    win = newwin(size, size * 2, 2, 2);
    if (win == nullptr)
        throw exception();
    wborder(win, '*', '*', '*', '*', '*', '*', '*', '*');
    keypad(win, true);

    for (int i = 0; i < size; i++)
    {
        board.push_back({});
        for (int j = 0; j < size; j++)
        {
            board[i].push_back(0);
        }
    }
}

int Board::loop()
{
    Board::initColor();
    timeout(500);
    noecho();
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
            y--;
            wattron(win, COLOR_PAIR(SNAKE_BODY));
            wmove(win, y, x);
            waddch(win, ' ');
            wmove(win, y, x + 1);
            waddch(win, ' ');
            wattroff(win, COLOR_PAIR(SNAKE_BODY));
            break;
        case KEY_DOWN:
        case 's':
            y++;
            wattron(win, COLOR_PAIR(SNAKE_BODY));
            wmove(win, y, x);
            waddch(win, ' ');
            wmove(win, y, x + 1);
            waddch(win, ' ');
            wattroff(win, COLOR_PAIR(SNAKE_BODY));
            break;
        case KEY_LEFT:
        case 'a':
            x--;
            x--;
            wattron(win, COLOR_PAIR(SNAKE_BODY));
            wmove(win, y, x);
            waddch(win, ' ');
            wmove(win, y, x + 1);
            waddch(win, ' ');
            wattroff(win, COLOR_PAIR(SNAKE_BODY));
            break;
        case KEY_RIGHT:
        case 'd':
            x++;
            x++;
            wattron(win, COLOR_PAIR(SNAKE_BODY));
            wmove(win, y, x);
            waddch(win, ' ');
            wmove(win, y, x + 1);
            waddch(win, ' ');
            wattroff(win, COLOR_PAIR(SNAKE_BODY));
            break;

        case 'q':
            return EXIT_SUCCESS;
        }
        wrefresh(win);
    }
}