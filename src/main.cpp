#include "board.h"
#include <iostream>
#include <ncurses.h>

using namespace std;

int main(int argc, char const *argv[])
{
    initscr();
    cbreak();
    refresh();
    try
    {
        Board now = Board();
        int result = now.loop();
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
