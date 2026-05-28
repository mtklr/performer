/* from https://fabiensanglard.net/doom_fire_psx/ */

#include "Fire.h"

#include "Config.h"

#include <ctime>

// can use calloc for these and it works but not sure that's ok?
// uint8_t *px; // calloc
static uint8_t px[CONFIG_LCD_WIDTH * CONFIG_LCD_HEIGHT];

Fire::Fire() {
}

void Fire::init() {
    _time = 0.f;

    srand(time(NULL));

    // px = (uint8_t *) std::calloc(CONFIG_LCD_WIDTH * CONFIG_LCD_HEIGHT, sizeof(uint8_t));

    // clear px buf
    for (int i = 0; i < CONFIG_LCD_WIDTH * CONFIG_LCD_HEIGHT; i++) {
        px[i] = 0;
    }

    // set bottom row
    for (int i = 0; i < CONFIG_LCD_WIDTH; i++) {
        // height - 2 offset avoids line at the top of display panel;
        // doesn't appear in simulator (??)
        px[(CONFIG_LCD_HEIGHT - 2) * CONFIG_LCD_WIDTH + i] = 0xf;
    }
}

void Fire::update(float dt) {
    _time += dt;
}

void Fire::draw(Canvas &canvas) {
    canvas.setBlendMode(BlendMode::Set);
    canvas.setColor(0);
    canvas.fill();

    for (int x = 0; x < CONFIG_LCD_WIDTH; x++) {
        for (int y = 0; y < CONFIG_LCD_HEIGHT - 1; y++) { // - 1 for display panel, see below
            int src = y * CONFIG_LCD_WIDTH + x;

            if (px[src] == 0) {
                px[src - CONFIG_LCD_WIDTH] = 0;
            } else {
                int rnd = rand() * 3 & 3;
                int dst = src - rnd + 1;

                // px[src - CONFIG_LCD_WIDTH] = px[src] - 1; // stationary
                // px[src - CONFIG_LCD_WIDTH] = px[src] - (rand() & 1); // movement
                px[dst - CONFIG_LCD_WIDTH] = px[src] - (rnd & 1); // as above with left/right movement
            }

            canvas.setColor(px[src]);
            canvas.point(x, y);
        }
    }
}
