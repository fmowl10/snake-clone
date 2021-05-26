#include "board.h"
#include <ncurses.h>

using namespace std;

int main(int argc, char const *argv[])
{
    initscr();
    cbreak();
    refresh();
    Board now = Board();
    int result = now.loop();
    endwin();
    return result;
}
