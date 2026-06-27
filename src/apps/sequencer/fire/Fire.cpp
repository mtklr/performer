/* from https://fabiensanglard.net/doom_fire_psx/ */

#include "Fire.h"

#include "Config.h"

#include <ctime>

// #define FIRE_LORES 1

#ifdef FIRE_LORES
#define FB_SCALE 2
#else
#define FB_SCALE 1
#endif

#define FB_WIDTH CONFIG_LCD_WIDTH / FB_SCALE
#define FB_HEIGHT CONFIG_LCD_HEIGHT / FB_SCALE

static uint8_t fire_fb[FB_WIDTH * FB_HEIGHT];

Fire::Fire() {
}

void Fire::init() {
    _time = 0.f;

    srand(time(NULL));

    // clear fb
    for (int i = 0; i < FB_WIDTH * FB_HEIGHT; i++) {
        fire_fb[i] = 0;
    }

    // set bottom row
    for (int i = 0; i < FB_WIDTH; i++) {
        // height - 2 offset avoids line at the top of display panel;
        // doesn't appear in simulator (??)
        fire_fb[(FB_HEIGHT - 1) * FB_WIDTH + i] = 0xf; // - 2
    }
}

void Fire::update(float dt) {
    _time += dt;
}

void Fire::draw(Canvas &canvas) {
    canvas.setBlendMode(BlendMode::Set);
    canvas.setColor(0);
    canvas.fill();

    for (int x = 0; x < FB_WIDTH; x++) {
        for (int y = 0; y < FB_HEIGHT - 0; y++) { // height - 1 for display panel
            int src = y * FB_WIDTH + x;

            if (fire_fb[src] == 0) {
                fire_fb[src - FB_WIDTH] = 0;
            } else {
                int rnd = rand() * 3 & 3;
                int dst = src - rnd + 1;

                // fire_fb[src - FB_WIDTH] = fire_fb[src] - 1; // stationary
                // fire_fb[src - FB_WIDTH] = fire_fb[src] - (rand() & 1); // movement
                fire_fb[dst - (FB_WIDTH / FB_SCALE)] = fire_fb[src] - (rnd & 1); // as above with left/right movement
            }

            canvas.setColor(fire_fb[src]);

#ifdef FIRE_LORES
            canvas.drawRect(x * FB_SCALE, y * FB_SCALE, 2, 2);
            // canvas.drawRect(x * FB_SCALE, y * FB_SCALE, 3, 3);
            // canvas.point(x * FB_SCALE, y * FB_SCALE);
#else
            canvas.point(x, y);
#endif
        }
    }
}
