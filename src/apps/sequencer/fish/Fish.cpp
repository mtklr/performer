/* basically sideways snow with some other stuff */
/* tftf - https://soundvision.bandcamp.com/album/mini-mod-ep-2025-remaster */

#include "Fish.h"

#include "Config.h"

#include "os/os.h"

#include <ctime>

#define RANDF(x) ((float)rand()/((float)RAND_MAX/((float)x)))
#define MAX_FISH 10
#define SCALE 1

// 16 x 8
static const uint16_t fish_bitmap[] = {
    0b0000001111111000,
    0b1000010000000100,
    0b1100100000010010,
    0b1010100000000001,
    0b1001100000000001,
    0b1010100000000010,
    0b1100110000000100,
    0b1000001111111000
};

// 8 x 10
uint8_t bubble_frame0[] = {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000
};

uint8_t bubble_frame1[] = {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b01000000
};

uint8_t bubble_frame2[] = {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00100000,
    0b00000000
};

uint8_t bubble_frame3[] = {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b10000000,
    0b00000000,
    0b00100000,
    0b00000000,
    0b01000000
};

uint8_t bubble_frame4[] = {
    0b00000000,
    0b00000000,
    0b00100000,
    0b00000000,
    0b10000000,
    0b00000000,
    0b00100000,
    0b00000000,
    0b01000000,
    0b00000000
};

// orig
uint8_t bubble_frame5[] = {
    0b10000000,
    0b00100000,
    0b00000000,
    0b10000000,
    0b00000000,
    0b00100000,
    0b00000000,
    0b01000000,
    0b00000000,
    0b00000000
};

uint8_t bubble_frame6[] = {
    0b00100000,
    0b00000000,
    0b10000000,
    0b00100000,
    0b00000000,
    0b01000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000
};

uint8_t bubble_frame7[] = {
    0b00000000,
    0b10000000,
    0b00100000,
    0b00000000,
    0b01000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000
};

uint8_t bubble_frame8[] = {
    0b10000000,
    0b00100000,
    0b00000000,
    0b01000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000
};

uint8_t bubble_frame9[] = {
    0b00000000,
    0b00100000,
    0b00000000,
    0b01000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000
};

uint8_t bubble_frame10[] = {
    0b00100000,
    0b00000000,
    0b01000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000
};

uint8_t bubble_frame11[] = {
    0b00000000,
    0b01000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000
};

uint8_t bubble_frame12[] = {
    0b01000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000
};

uint8_t *bubble_frames[13] = {
    bubble_frame0,
    bubble_frame1,
    bubble_frame2,
    bubble_frame3,
    bubble_frame4,
    bubble_frame5,
    bubble_frame6,
    bubble_frame7,
    bubble_frame8,
    bubble_frame9,
    bubble_frame10,
    bubble_frame11,
    bubble_frame12
};

typedef struct {
    int size;
    uint8_t color;
    float x;
    float y;
    float speed;
    float phase;
    float freq;
    float wobble;
    bool dir;
    int draw_bubbles;
    // int bindex;
    // uint8_t *bp;
} Fishy;

static const float Pi = 3.1415926536f;
static const uint8_t fish_colors[7] = { 0x3, 0x5, 0x7, 0x9, 0xb, 0xd, 0xf };

int num_fish;
int bx;
int by;
int bindex;
uint8_t *bp;

Fishy fishies[MAX_FISH];

Fish::Fish() {
}

void Fish::init() {
    _time = 0.f;

    _delay = os::ticks();

    srand(time(NULL));

    for (int i = 0; i < MAX_FISH; i++) {
        fishies[i].size = 1;
        fishies[i].color = fish_colors[rand() % 7];
        fishies[i].x = RANDF(256);
        fishies[i].y = RANDF(64);
        fishies[i].speed = 0.6f + RANDF(1.2f);
        fishies[i].phase = RANDF(2.0f * Pi);
        fishies[i].freq = RANDF(0.2f);
        fishies[i].wobble = 0.5f + RANDF(2.5f);
        fishies[i].dir = rand() % 2;
        fishies[i].draw_bubbles = rand() % 2;
        // fishies[i].bindex = rand() % 12;
        // fishies[i].bp = bubble_frames[rand() % 12];
    }

    num_fish = rand() % (MAX_FISH / 2) + (MAX_FISH / 2) + 1;

    bp = bubble_frames[0];
}

void Fish::update(float dt) {
    _time += dt;
}

void Fish::draw(Canvas &canvas) {
    canvas.setBlendMode(BlendMode::Set);
    canvas.setColor(0);
    canvas.fill();

    for (int i = 0; i < num_fish; i++) {
        Fishy *fish = &fishies[i];

        if (fish->dir == 0 && fish->x > CONFIG_LCD_WIDTH - 1) {
            fish->x = -16 * SCALE;
            fish->y = RANDF(54) + 10;

            if (rand() % 2 == 0) {
                fish->dir = !fish->dir;
            }
        }

        if (fish->dir == 1 && fish->x < 0) {
            fish->x = (CONFIG_LCD_WIDTH - 1) + 16 * SCALE;

            if (rand() % 2 == 0) {
                fish->dir = !fish->dir;
            }
        }

        int x = (int) floorf(fish->x);
        int y = (int) floorf(fish->y + fish->wobble * sinf(fish->phase));

        canvas.setColor(fish->color);

        // draw fish
        for (int row = 0; row < 8; row++) {
            const uint32_t row_data = fish_bitmap[row];

            if (fish->dir == 0) { // move right
                for (int col = 0; col < 16; col++) {
                    if (row_data & (1 << (15 - col))) {
                        canvas.fillRect(x + col * SCALE, y + row * SCALE, SCALE, SCALE);
                    }
                }
            } else { // move left
                for (int col = 15; col >= 0; col--) {
                    if (row_data & (1 << (15 - col))) {
                        canvas.fillRect(x - col * SCALE, y + row * SCALE, SCALE, SCALE);
                    }
                }
            }
        }

        // bubbles
        if (fish->dir == 0) {
            bx = x + 15 * SCALE;
        } else {
            bx = x - 15 * SCALE;
        }

        by = y - 6 * SCALE;

        // delay, cycle bubble bubble_frames
        if (os::ticks() > _delay + os::time::ms(500)) {
            _delay = os::ticks();

            bindex %= 13;
            bp = bubble_frames[bindex++];
            fish->draw_bubbles = rand() % 2;

            // see rant below
            // fish->bindex = fish->bindex % 13;
            // fish->bp = bubble_frames[fish->bindex++];
            // bp = fish->bp;
        }

// TODO the problem: bubble anim is identical for each fish.  i'd like the anim
// to start at a random frame and loop for each fish, so each one looks like
// it's "breathing" on its own.
//
// i tried adding a pointer to anim frames (bp) and index (bindex) INSIDE the
// fish struct, thinking that if i initialized each fishie[i].bindex with a
// random number in the range of bubble frames (13), each fish in this for loop
// would start its bubble animation on a different frame, because fish->bp
// would point to bubble_frames[fish->bindex], and fish->bindex would be
// different for each as it was initialized with a random num in Fish::init()
// above.
//
// HOWEVER...
//
// when i tried this, one fish would have a looping bubble anim, and the rest
// would be stuck on one frame of the anim.
// 
// i don't understand why this is - what's the difference between a pointer and
// index "outside" the fish struct, and the same inside the fish struct, which
// (to my mind) would be "independent" and have their own different animation
// loops...  arhghrghghgghhghgrhrg....
//
// so for now each fish has an identical anim loop.
// i kept fish->bindex (index to anim frame) inside the struct because it seems
// to work...
//
// while i'm complaing, firing them off at different times would also be nice.

    // only draw bubbles sometimes, this helps look better but still doesn't
    // address above issue
    if (fish->draw_bubbles == 1) {
        // draw bubbles
        for (int row = 0; row < 10; row++) {
            const uint8_t row_data = bp[row];
            // const uint8_t row_data = fish->bp[row];

            if (fish->dir == 0 && bx < CONFIG_LCD_WIDTH - 1) { // move right
                for (int col = 0; col < 8; col++) {
                    if (row_data & (1 << (7 - col))) {
                        canvas.fillRect(bx + col * SCALE, by + row * SCALE, SCALE, SCALE);
                    }
                }
            } else if (fish->dir == 1 && bx > 0) { // move left
                for (int col = 0; col < 8; col++) {
                    if (row_data & (1 << (7 - col))) {
                        canvas.fillRect(bx - col * SCALE, by + row * SCALE, SCALE, SCALE);
                    }
                }
            }
        }
    }
        if (fish->dir == 0) {
            fish->x += fish->speed; // move right
        } else {
            fish->x -= fish->speed; // move left
        }

        fish->phase += fish->freq;
    }
}
