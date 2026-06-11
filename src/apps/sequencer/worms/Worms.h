/***************************************************************************
*
*   Copyright(c) Jeff V. Merkey 1997-2019.  All rights reserved.
*
*   Portions adapted from xscreensaver loadsnake program is
*   portions Copyright (c) 2007-2011 Cosimo Streppone <cosimo@cpan.org>
*
*   Licensed under the MIT/X License
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy
*   of this software and associated documentation files (the "Software"), to
*   deal in the Software without restriction, including without limitation
*   the rights to use, copy, modify, merge, publish, distribute, sublicense,
*   and/or sell copies of the Software, and to permit persons to whom the
*   Software is furnished to do so, subject to the following conditions:
*
*   The above copyright notice and this permission notice shall be included
*   in all copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
*   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
*   DEALINGS IN THE SOFTWARE.
**************************************************************************/

#pragma once

#include "core/gfx/Canvas.h"

#define COLS (CONFIG_LCD_WIDTH / 2)
#define ROWS (CONFIG_LCD_HEIGHT / 2)

#define WORM_MIN_LEN    4
#define WORM_MAX_LEN   18
// #define WORM_TAIL_LEN   3
#define MAX_WORMS      64
#define WORM_SEG_HEIGHT 4
#define WORM_SEG_WIDTH  4

// adjust worm total length based on screen area size.  smaller
// displays have shorter worms.  the current logic adjusts for
// worm speedup based on the current max worm length.  Here we
// check a min and max screen area for worm length expansion.

/* #define AREA_BASE_LEN   (WORM_MAX_LEN / 2) */
/* #define AREA_MINROWS    32 */
/* #define AREA_MINCOLS    128 */
/* #define AREA_MIN        (AREA_MINROWS * AREA_MINCOLS) */
/* #define AREA_MAXROWS    64 */
/* #define AREA_MAXCOLS    256 */
/* #define AREA_MAX        (AREA_MAXROWS * AREA_MAXCOLS) */
/* #define AREA            ((COLS * ROWS) < (AREA_MINROWS * AREA_MINCOLS) \ */
/*                         ? (AREA_MINROWS * AREA_MINCOLS) : (COLS * ROWS)) */
/* #define AREA_DIVISOR    ((AREA_MAX - AREA_MIN) / AREA_BASE_LEN) */
/* #define AREA_EXT_LEN    (((AREA - (AREA_MIN)) / (AREA_DIVISOR)) > \ */
/*                         AREA_BASE_LEN ? AREA_BASE_LEN : \ */
/*                         ((AREA - (AREA_MIN)) / (AREA_DIVISOR))) */

/* #define MAX_LOADAVG   100 */
/* #define MAX_NANOSEC   100000000 */
/* #define MIN_NANOSEC   10000000 */
/* #define MAX_MICROSEC  100000 */
/* #define MIN_MICROSEC  10000 */

typedef struct _WORM {
    int cpu;
    int count;
    int limit;
    int x[WORM_MAX_LEN];
    int y[WORM_MAX_LEN];
    int x_prev[WORM_MAX_LEN];
    int y_prev[WORM_MAX_LEN];
    int length;
    int length_prev;
    int direction;
    int runlength;
    // int windowlength;
} WORM;

typedef struct _STATE {
    int cpus;
    int delay;
    int divisor;
    int rows;
    int cols;
    /* unsigned long long usr[MAX_WORMS]; */
    /* unsigned long long sys[MAX_WORMS]; */
    /* unsigned long long nice[MAX_WORMS]; */
    /* unsigned long long idle[MAX_WORMS]; */
    /* unsigned long long io[MAX_WORMS]; */
    /* unsigned long long irq[MAX_WORMS]; */
    /* unsigned long long sirq[MAX_WORMS]; */
    /* unsigned long long steal[MAX_WORMS]; */
    /* unsigned long long guest[MAX_WORMS]; */
    /* unsigned long long guest_nice[MAX_WORMS]; */
    WORM worms[8];
} STATE;

class Worms {
public:
    Worms();

    void init();

    void update(float dt);
    void draw(Canvas &canvas);
    void worm_put_rect(Canvas &canvas, int c, int y, int x, uint8_t color);
    void clear_worm(Canvas &canvas, STATE *st, WORM *s);
    void draw_worm(Canvas &canvas, STATE *st, WORM *s);
    void run_worms(Canvas &canvas);

private:
    float _time;

    // uint32_t _delay;
};
