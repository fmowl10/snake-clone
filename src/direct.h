#include <iostream>
#include <ncurses.h>

#ifndef __DIRECT_H__
#define __DIRECT_H__
enum Direct
{
    NONE = 0b0000,
    N = 0b0101,
    S = 0b1010,
    W = 0b1100,
    E = 0b0011,
    Opposite = 0b1111,
};
// XOR
#endif