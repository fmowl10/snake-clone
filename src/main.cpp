#include "board.h"
#include <cstring>
#include <iostream>
#include <ncurses.h>
#include <vector>

#define MESSAGECOUNT 3

#define STAGECOUNT 5
using namespace std;

const string StageFile[STAGECOUNT] = {
    "stage-1.map",
    "stage-2.map",
    "stage-3.map",
    "stage-4.map",
    "stage-5.map",
};

const char *stageClearMessage = "you clear stage %d press any button to start next stage";
const char *gameClearMessage = "you clear the game press any button to exit the game";
const char *stageMessage = "Stage %d";
const char *KONAMICOMMANDMESSAGE = "KONAMI???";
const char *chooseStageMessage = "you can go any stage input stage number ";

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

    vector<int> KONAMICOMMAND = {KEY_UP, KEY_UP, KEY_DOWN, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_LEFT, KEY_RIGHT, int('b'), int('a')};

    int i = 0;
    int KONAMIINDEX = 0;
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
        if (KONAMICOMMAND[KONAMIINDEX] == ch)
        {
            KONAMIINDEX++;
            mvprintw(row / 2 + 4, col / 2 - KONAMICOMMAND.size() / 2 + KONAMIINDEX, "0");
        }
        else
        {
            move(row / 2 + 4, 0);
            clrtoeol();
            KONAMIINDEX = 0;
        }

        if (KONAMIINDEX == KONAMICOMMAND.size())
        {
            clear();
            mvprintw(row / 2, col / 2 - strlen(KONAMICOMMANDMESSAGE) / 2, KONAMICOMMANDMESSAGE);
            mvprintw(row / 2 + 1, col / 2 - strlen(chooseStageMessage) / 2, chooseStageMessage);
            refresh();
            while (true)
            {
                int stage = getch() - '0';
                if (1 <= stage && stage <= STAGECOUNT)
                {
                    i = stage - 1;
                    break;
                }
            }
            clear();
            refresh();
            goto Main;
        }
    }
Main:
    WINDOW *map = newwin(25, 50, 2, 2);
    WINDOW *statusBoard = newwin(6, 15, 2, 50);
    WINDOW *missionBoard = newwin(6, 15, 9, 50);
    try
    {
        for (; i < STAGECOUNT; i++)
        {
            Board now = Board(StageFile[i], map, missionBoard, statusBoard);
            refresh();
            clear();
            mvprintw(1, 2, stageMessage, i + 1);
            int result = now.loop();
            clear();
            refresh();

            if (result == EXIT_FAILURE)
            {
                delwin(statusBoard);
                delwin(statusBoard);
                delwin(map);

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
                case DeadCase::ShortBody:
                    move(row / 2, col / 2 - DeadMessage[DeadCase::ShortBody].length() / 2);
                    addstr(DeadMessage[DeadCase::ShortBody].c_str());
                    break;
                case DeadCase::UserGiveup:
                    move(row / 2, col / 2 - DeadMessage[DeadCase::UserGiveup].length() / 2);
                    addstr(DeadMessage[DeadCase::UserGiveup].c_str());
                    break;
                default:
                    break;
                }
                timeout(-1);
                move(row / 2 + 1, col / 2 - 12);
                clrtoeol();
                addstr("quit to press any button");
                refresh();
                timeout(-1);
                getch();
                endwin();
                return EXIT_FAILURE;
            }
            if (i == STAGECOUNT - 1)
                break;
            flushinp();
            move(row / 2, col / 2 - strlen(stageClearMessage) / 2);
            printw(stageClearMessage, i + 1);
            refresh();
            timeout(-1);
            getch();
        }
    }
    catch (BoardMiniumSizeException &e)
    {
        endwin();
        cout << e.what() << endl;
        return EXIT_FAILURE;
    }
    catch (InvalidMapException &e)
    {
        endwin();
        cout << e.what() << endl;
        return EXIT_FAILURE;
    }
    mvprintw(row / 2, col / 2 - strlen(gameClearMessage) / 2, gameClearMessage);
    refresh();
    timeout(-1);
    getch();
    endwin();
    return EXIT_SUCCESS;
}
