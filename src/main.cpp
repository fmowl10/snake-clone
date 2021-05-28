#include "board.h"
#include <cstring>
#include <iostream>
#include <ncurses.h>
#define MESSAGECOUNT 3

using namespace std;

int main(int argc, char const *argv[])
{
    initscr();
    curs_set(0);
    mousemask(ALL_MOUSE_EVENTS, NULL);
    keypad(stdscr, true);
    cbreak();
    noecho();
    refresh();
    int row = getmaxy(stdscr);
    int col = getmaxx(stdscr);
    string welcome_message[MESSAGECOUNT] = {
        "welcome this is snake game",
        "click this text to start or press s",
        "click this text to exit or press q",
    };
    for (int i = 0; i < MESSAGECOUNT; i++)
    {
        move(row / 2 + i, col / 2 - welcome_message[i].length() / 2);
        addstr(welcome_message[i].c_str());
    }
    while (true)
    {
        int ch = getch();
        switch (ch)
        {
        case 's':
            goto Main;
            break;
        case 'q':
            return EXIT_SUCCESS;
        case KEY_MOUSE:
            MEVENT event;
            if (getmouse(&event) == OK && event.bstate == BUTTON1_CLICKED)
            {
                if (event.y == row / 2 + 2)
                {
                    if (event.x >= col / 2 - (int)welcome_message[2].length() / 2 && event.x <= col / 2 + (int)welcome_message[2].length())
                    {
                        return EXIT_FAILURE;
                    }
                }
                else if (event.y == row / 2 + 1)
                {
                    if (col / 2 - (int)welcome_message[1].length() / 2 <= event.x && event.x <= col / 2 + (int)welcome_message[1].length())
                    {
                        goto Main;
                    }
                }
            }
            break;
        }
    }
Main:
    try
    {
        clear();
        refresh();
        Board now = Board();
        int result = now.loop();
        clear();
        if (result == EXIT_FAILURE)
        {
            switch (now.why())
            {
            case DeadCase::ColideBody:
                move(row / 2, col / 2 - DeadMessage[DeadCase::ColideBody].length() / 2);
                addstr(DeadMessage[DeadCase::ColideBody].c_str());
                break;
            case DeadCase::ColideWall:
                move(row / 2, col / 2 - DeadMessage[DeadCase::ColideWall].length() / 2);
                addstr(DeadMessage[DeadCase::ColideWall].c_str());
                break;
            case DeadCase::OppositeWay:
                move(row / 2, col / 2 - DeadMessage[DeadCase::OppositeWay].length() / 2);
                addstr(DeadMessage[DeadCase::OppositeWay].c_str());
                break;
            default:
                break;
            }
            timeout(-1);
            move(row / 2 + 1, col / 2);
            addstr("quit to press any button");
            refresh();
            timeout(-1);
            getch();
        }
        endwin();
        return result;
    }
    catch (BoardMiniumSizeException &e)
    {
        endwin();
        cout << e.what() << endl;
        return EXIT_FAILURE;
    }
}
