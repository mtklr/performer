/* from https://fabiensanglard.net/doom_fire_psx/ */

#include "Fire.h"

#include "Config.h"

#include <ctime>

uint8_t *fire_fb;

Fire::Fire() {
}

void Fire::init() {
    _time = 0.f;

    srand(time(NULL));

    fire_fb = (uint8_t *) std::calloc(CONFIG_LCD_WIDTH * CONFIG_LCD_HEIGHT, sizeof(uint8_t));

    if (fire_fb == NULL) {
        // fprintf(stderr,"error: calloc fire_fb\n");
        return;
    }

    // clear fire_fb buf
    for (int i = 0; i < CONFIG_LCD_WIDTH * CONFIG_LCD_HEIGHT; i++) {
        fire_fb[i] = 0;
    }

    // set bottom row
    for (int i = 0; i < CONFIG_LCD_WIDTH; i++) {
        // height - 2 offset avoids line at the top of display panel;
        // doesn't appear in simulator (??)
        fire_fb[(CONFIG_LCD_HEIGHT - 2) * CONFIG_LCD_WIDTH + i] = 0xf;
    }
}

void Fire::cleanup() {
    if (fire_fb) {
        std::free(fire_fb);
        fire_fb = NULL;
        // fprintf(stderr, "freed fire_fb\n");
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

            if (fire_fb[src] == 0) {
                fire_fb[src - CONFIG_LCD_WIDTH] = 0;
            } else {
                int rnd = rand() * 3 & 3;
                int dst = src - rnd + 1;

                // fire_fb[src - CONFIG_LCD_WIDTH] = fire_fb[src] - 1; // stationary
                // fire_fb[src - CONFIG_LCD_WIDTH] = fire_fb[src] - (rand() & 1); // movement
                fire_fb[dst - CONFIG_LCD_WIDTH] = fire_fb[src] - (rnd & 1); // as above with left/right movement
            }

            canvas.setColor(fire_fb[src]);
            canvas.point(x, y);
        }
    }
}
