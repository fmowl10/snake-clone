/**
 * @file direct.h
 * @author Kim Jae Ha Kim Jin Seok
 * @brief
 * @version 0.1
 * @date 2021-05-30
 *
 * @copyright Copyright (c) 2021 cocozo fmowl
 *
 */
#include <iostream>
#include <ncurses.h>

#ifndef __DIRECT_H__
#define __DIRECT_H__
/**
 * @brief direction enum
 *
 */
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