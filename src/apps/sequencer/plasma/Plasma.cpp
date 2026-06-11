// adapted from - https://invisiblewater.github.io/MattGraphicsTutorials/effects/plasma.html

#include "Plasma.h"

#include "Config.h"

#include <ctime>

// #define PLASMA_LORES 1

#ifdef PLASMA_LORES
#define FB_SCALE 2
#else
#define FB_SCALE 1
#endif

#define FB_WIDTH CONFIG_LCD_WIDTH / FB_SCALE
#define FB_HEIGHT CONFIG_LCD_HEIGHT / FB_SCALE

static uint8_t plasma_fb[FB_WIDTH * FB_HEIGHT];

int a, b, c, d;

static float dist(int a, int b, int c, int d) {
    return sqrt((a - c) * (a - c) + (b - d) * (b - d));
}

Plasma::Plasma() {
}

void Plasma::init() {
    _time = 0.f;

    srand(time(NULL));

    a = rand() % 64 + 64;
    b = rand() % 64 + 64;
    c = rand() % 96 + 96;
    d = rand() % 32 % 32;
}

void Plasma::update(float dt) {
    _time += dt;
}

void Plasma::draw(Canvas &canvas) {
    canvas.setBlendMode(BlendMode::Set);
    canvas.setColor(0);
    canvas.fill();

    for (int y = 0; y < FB_HEIGHT; y++) {
        for (int x = 0; x < FB_WIDTH; x++) {
            float value = sin(dist(x + (_time * 20), y, a, b) / 8) +
                sin(dist(x, y, 64, 64) / 8) +
                sin(dist(x, y + (_time * 20) / 7, c, d) / 7) +
                sin(dist(x, y, 192, 100) / 8);

            int color = ((4 + (int) floor(value)) * 2) % 16;

            int id = y * FB_WIDTH + x;

            plasma_fb[id] = color;
            // plasma_fb[id + 1] = color * 2;
            // plasma_fb[id + 2] = 15 - color;
            // plasma_fb[id + 3] = 15;

            canvas.setColor(plasma_fb[id]);
#ifdef PLASMA_LORES
            canvas.fillRect(x * FB_SCALE, y * FB_SCALE, FB_SCALE, FB_SCALE);
#else
            canvas.point(x, y);
#endif
        }
    }
}
