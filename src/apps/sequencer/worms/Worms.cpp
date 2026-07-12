// adapted from - https://github.com/jeffmerkey/netware-screensaver-linux.git

/* Copyright(c) Jeff V. Merkey 1997-2019.  All rights reserved.

Portions adapted from xscreensaver loadsnake program is
portions Copyright (c) 2007-2011 Cosimo Streppone <cosimo@cpan.org>

Licensed under the MIT/X License

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to
deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#include "Worms.h"

#include "Config.h"

#include <ctime>

static const uint8_t worm_colors[7] = { 0x3, 0x5, 0x7, 0x9, 0xa, 0xc, 0xf };

// int worm_max_length = WORM_MAX_LEN;

STATE state;

static void move_worm(STATE *st, WORM *s, bool pulse) {
    int n = 0, dir = 0;
    int x = 0, y = 0;

    // worm head position
    x = s->x[0];
    y = s->y[0];

    // and direction
    dir = s->direction;

    // 0=up, 2=right, 4=down, 6=left
    switch(dir) {
        case 0: y++;      break;    // up
        case 1: y++; x++; break;    // up right
        case 2:      x++; break; // x += 2; break; // right
        case 3: y--; x++; break;    // down right
        case 4: y--;      break;    // down
        case 5: y--; x--; break;    // down left
        case 6:      x--; break; // x-= 2; break; // left
        case 7: y++; x--; break;    // up left
    }

    // Check bounds and change direction
    if (x < 0 && (dir >= 5 && dir <= 7)) {
        x = 1;
        dir -= 4;
    } else if (y < 0 && (dir >= 3 && dir <= 5)) { // y is max y down, dir is down-right
        y = 1;
        dir -= 4;
    } else if (x + (WORM_SEG_WIDTH / 2) >= st->cols && (dir >= 1 && dir <= 3)) {
        x = st->cols - 2;
        dir += 4; // NOTE: worm getting stuck in lower-right corner
    } else if (y + (WORM_SEG_HEIGHT / 2) >= st->rows && (dir == 7 || dir == 0 || dir == 1)) {
        y = st->rows - 2;
        dir += 4; // NOTE: worm getting stuck in lower-right corner
    } else if (s->runlength == 0) {
        int rnd;

        rnd = rand() % 128;

        if (rnd > 90) {
            dir += 2;
        } else if (rnd == 1) {
            dir++;
        } else if (rnd == 2) {
            dir--;
        }

        // set this to the current worm length
        s->runlength = s->length;
    } else {
        int rnd;

        s->runlength--;

        rnd = rand() % 128;

        if (pulse) {
            rnd = rand() % 2 + 1;
        }

        if (rnd == 1) {
            dir++;
        } else if (rnd == 2) {
            dir--;
        }
    }

    if (dir < 0) {
        dir = -dir;
    }

    dir = dir % 8;

    s->direction = dir;

    // Copy x,y coords in "tail" positions
    for(n = s->length - 1; n > 0; n--) {
        s->x[n] = s->x[n-1];
        s->y[n] = s->y[n-1];
    }

    // New head position
    s->x[0] = x;
    s->y[0] = y;
}

// static void grow_worm(STATE *st, WORM *s) {
//     // int newlen = get_cpu_load(st, s->cpu);
//     int newlen = rand() % (WORM_MAX_LEN - (WORM_MIN_LEN - 1)) + WORM_MIN_LEN;
//     int len = s->length;

//     // fprintf(stderr, "grow: cpu %d len %d newlen %d\n", s->cpu, len, newlen);

//     if (newlen > len) {
//         int x, y;

//         x = s->x[len - 1];
//         y = s->y[len - 1];

//         switch(s->direction) {
//             case 0: y--;      break;
//             case 1: y--; x--; break;
//             case 2:      x--; break; //  -= 2; break;
//             case 3: y++; x--; break;
//             case 4: y++;      break;
//             case 5: y++; x++; break;
//             case 6:      x++; break; //  += 2; break;
//             case 7: y--; x++; break;
//         }

//         len++;

//         if (len >= worm_max_length) {
//             len = worm_max_length - 1;
//         }

//         s->x[len] = x;
//         s->y[len] = y;
//     } else if (newlen < len) {
//         len--;

//         if (len < WORM_MIN_LEN) {
//             len = WORM_MIN_LEN;
//         }

//         s->x[len + 1] = 0;
//         s->y[len + 1] = 0;
//     }

//     s->length = len;
//     // return(len);
// }

// static void save_worm(WORM *s) {
//     int n;

//     // save last worm position and coordinates
//     // for clearing later
//     for (n = s->length - 1; n >= 0; n--) {
//         s->x_prev[n] = s->x[n];
//         s->y_prev[n] = s->y[n];
//     }

//     s->length_prev = s->length;
// }

void Worms::worm_put_rect(Canvas &canvas, int c, int y, int x, uint8_t color) {
    // if (x >= CONFIG_LCD_WIDTH)
    //    return;
    // if (y >= CONFIG_LCD_HEIGHT)
    //    return;

    if (c == 0) {
        canvas.setColor(color); // head
        canvas.fillRect(x, y, 3, 3);
    } else if (c > 0 && c < 3) {
        canvas.setColor(color - 1); // middle
        canvas.fillRect(x, y, 3, 3);
    } else {
        canvas.setColor(color - 2); // tail
        canvas.drawRect(x, y, 3, 3);
    }
}

// void Worms::clear_worm(Canvas &canvas, STATE *st, WORM *s) {
//     int n;

//     for (n = s->length_prev - 1; n >= 0; n--) {
//         // worm_put_rect(canvas, 0, s->y_prev[n], s->x_prev[n] + 1, 0);
//         worm_put_rect(canvas, 0, s->y_prev[n] * 2, s->x_prev[n] * 2, 0);
//     }
// }

void Worms::draw_worm(Canvas &canvas, STATE *st, WORM *s) {
    int n, div, mod, c;

    // get character interval and draw worm; it is
    // assumed that the minimum worm length is 4
    div = s->length / 4;
    mod = s->length % 4;

    for (n = s->length - 1; n >= 0 && div; n--) {
        c = n < (div + 1) * mod ? n / (div + 1) : (n - mod) / div;
        // worm_put_rect(canvas, c, s->y[n], s->x[n] + 1, worm_colors[s->cpu % 7]);
        worm_put_rect(canvas, c, s->y[n] * 2, s->x[n] * 2, worm_colors[s->cpu % 6 + 1]);

        // fprintf(stderr, "cpu %d x[n] = %d y[n] = %d n = %d\n", s->cpu, s->x[n], s->y[n], n);
    }

    // fprintf(stderr, "\n");
}

void Worms::run_worms(Canvas &canvas) { //, STATE *st)
    // float range, increment;
    int n;

    STATE *st = &state;
    // reset columns and lines in case the screen was resized
    // worm_max_length = AREA_BASE_LEN + AREA_EXT_LEN;

    // if (worm_max_length > WORM_MAX_LEN) {
    //     worm_max_length = WORM_MAX_LEN;
    // }

    st->cols = COLS;
    st->rows = ROWS;

    for (n = 0; n < st->cpus; n++) {
        WORM *s = (WORM *) &st->worms[n];
        bool pulse = (_pulse_state >> (n)) & 1;

        // if (++s->count >= s->limit) {
            // s->count = 0;
            // grow_worm(st, s);
            move_worm(st, s, pulse);
            // clear_worm(canvas, st, s);
            // s->limit = 4 - (s->length / (worm_max_length / 4));

            // fprintf(stderr, "length %d limit %d\n", s->length, s->limit);
        // }

        // save_worm(s);

        // update all worms even those sleeping to
        // maintain worm overwrite stacking order
        // when one worm overwrites another during
        // display
        draw_worm(canvas, st, s);
        // refresh();
    }

    // decrease base wait time if system load increases
    // range is 0-100 load average before reaching
    // minimum st->delay wait time
    // n = 50; // TODO: tempo? //get_system_load();
// #ifdef NANOSLEEP
//     range = MAX_NANOSEC - MIN_NANOSEC;
//     increment = range / MAX_LOADAVG;
//     st->delay = MAX_NANOSEC - (n * increment);
//     if (st->delay < MIN_NANOSEC)
//        st->delay = MIN_NANOSEC;
//     st->delay /= st->divisor;
// #else
    // range = MAX_MICROSEC - MIN_MICROSEC;
    // increment = range / MAX_LOADAVG;
    // st->delay = MAX_MICROSEC - (n * increment);

    // if (st->delay < MIN_MICROSEC) {
    //     st->delay = MIN_MICROSEC;
    // }

    // st->delay /= st->divisor;
// #endif
    // fprintf(stderr, "delay %d load(n) = %d\n", st->delay, n);
    // return st->delay;
}

Worms::Worms() {
}

void Worms::init() {
    _time = 0.f;
    _pulse_state = 0;

    // _delay = os::ticks();

    srand(time(NULL));

    int n, i; //, ret, prio = 0;
    int cpus = rand() % CONFIG_CHANNEL_COUNT + 1; // TODO: tracks
    // int speedup = 1;

    STATE *st = &state;

    // std::memset(st, 0, sizeof(STATE));
    st->cpus = cpus; //get_processors();

    // if (!st->cpus)
    //    exit(1);

    // set nice value to highest priority
    // prio = getpriority(PRIO_PROCESS, 0);
    // setpriority(PRIO_PROCESS, 0, -20);

    if (cpus > st->cpus) {
        st->cpus = cpus;
    }

    if (st->cpus > MAX_WORMS) {
        st->cpus = MAX_WORMS;
    }

    // if (speedup > 0) {
    //     st->divisor = speedup;
    // } else {
    //     st->divisor = 1;
    // }

    // ret = init_ncurses();
    // if (ret < 0)
    //    return 1;

    // fprintf(stderr, "cols: %d lines: %d base: %d len: %d area: %d"
    //         " max: %d min: %d adj: %d divisor: %d\n",
    //         COLS, ROWS, AREA_BASE_LEN, AREA_EXT_LEN, AREA,
    //         AREA_MAX, AREA_MIN, (AREA) - (AREA_MIN), AREA_DIVISOR);

    // worm_max_length = AREA_BASE_LEN + AREA_EXT_LEN;

    // if (worm_max_length > WORM_MAX_LEN) {
    //     worm_max_length = WORM_MAX_LEN;
    // }

    st->cols = COLS;
    st->rows = ROWS;

// #ifdef NANOSLEEP
//     st->delay = MAX_NANOSEC / st->divisor;
// #else
//     st->delay = MAX_MICROSEC / st->divisor;
// #endif

    for (n = 0; n < st->cpus; n++) {
        WORM *s = (WORM *)&st->worms[n];

        s->cpu  = n;
        s->x[0] = rand() % (COLS - 1);
        s->y[0] = rand() % ROWS;

        for (i = 1; i < WORM_MAX_LEN; i++) {
           s->x[i] = s->x[0];
           s->y[i] = s->y[0];
        }

        s->direction = ((rand() % 9) >> 1) << 1;
        // s->length = WORM_MIN_LEN;
        // s->runlength = WORM_MIN_LEN;
        s->length = rand() % (WORM_MAX_LEN - (WORM_MIN_LEN - 1)) + WORM_MIN_LEN; // random range of min_len to max_len
        s->runlength = s->length;

        // fprintf(stderr, "worm %d starting at %d,%d dir %d length %d\n",
        //         s->cpu, s->x[0], s->y[0], s->direction, s->length);
    }

    // why not ncurses getch()??
    // while (!worm_kbhit())
    // {
// #ifdef NANOSLEEP
       // struct timespec ts = { 0, st->delay };
// #endif
// #ifdef NANOSLEEP
       // nanosleep(&ts, NULL);
// #else
       // usleep(st->delay);
// #endif
    // }

    // if (st->worms)
    //    free(st->worms);

    // clear_ncurses();
    // setpriority(PRIO_PROCESS, 0, prio);
    // return 0;
}

void Worms::update(float dt, uint8_t gates) {
    _time += dt;
    _pulse_state = gates;
}

void Worms::draw(Canvas &canvas) {
    canvas.setBlendMode(BlendMode::Set);
    canvas.setColor(0);
    canvas.fill();

    run_worms(canvas);
}
