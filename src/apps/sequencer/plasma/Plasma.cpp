// adapted from - https://invisiblewater.github.io/MattGraphicsTutorials/effects/plasma.html

#include "Plasma.h"

#include "Config.h"

#include "os/os.h"

#include <ctime>

uint8_t *plasma_fb;

static float dist(int a, int b, int c, int d) {
    return sqrt((a - c) * (a - c) + (b - d) * (b - d));
}

Plasma::Plasma() {
}

void Plasma::init() {
    _time = 0.f;

    srand(time(NULL));

    plasma_fb = (uint8_t *) std::calloc(CONFIG_LCD_WIDTH * CONFIG_LCD_HEIGHT, sizeof(uint8_t));

    if (plasma_fb == NULL) {
        // fprintf(stderr,"error: calloc plasma_fb\n");
        return;
    }

}

void Plasma::cleanup() {
    if (plasma_fb) {
        std::free(plasma_fb);
        plasma_fb = NULL;
        // fprintf(stderr, "freed plasma_fb\n");
    }
}

void Plasma::update(float dt) {
    _time += dt;
}

void Plasma::draw(Canvas &canvas) {
    canvas.setBlendMode(BlendMode::Set);
    canvas.setColor(0);
    canvas.fill();

    int plasma_time = floor(os::ticks() / 50);

    for (int y = 0; y < CONFIG_LCD_HEIGHT; y++) {
        for (int x = 0; x < CONFIG_LCD_WIDTH; x++) {
            float value = sin(dist(x + plasma_time, y, 128, 128) / 8) +
                sin(dist(x, y, 64, 64) / 8) +
                sin(dist(x, y + plasma_time / 7, 192, 64) / 7) +
                sin(dist(x, y, 192, 100) / 8);

            int color = (int) ((4 + value) * 2) % 16; // floor()

            int id = y * CONFIG_LCD_WIDTH + x;

            plasma_fb[id] = color;

            canvas.setColor(plasma_fb[id]);
            canvas.point(x, y);
        }
    }
}
