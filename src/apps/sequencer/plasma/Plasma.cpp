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

uint8_t plasma_fb[FB_WIDTH * FB_HEIGHT];

int a, b, c, d;

static float dist(int a, int b, int c, int d) {
    return std::sqrt((a - c) * (a - c) + (b - d) * (b - d));
}

Plasma::Plasma() {
}

void Plasma::init() {
    _time = 0.f;

    srand(time(NULL));

    a = rand() % 64 + 64;
    b = rand() % 64 + 64;
    c = rand() % 96 + 96;
    d = rand() % 32;
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
            float v = std::sin(dist(x + (_time * 20), y, a, b) / 8) +
                std::sin(dist(x, y, 64, 64) / 8) +
                std::sin(dist(x, y + (_time * 20) / 7, c, d) / 7) +
                std::sin(dist(x, y, 192, 100) / 8);
            int color = (int) (4 + std::floor(v)) * 2;
            int index = y * FB_WIDTH + x;

            plasma_fb[index] = color % 16;
            // plasma_fb[(index + 1) % (FB_WIDTH * FB_HEIGHT)] = color * 2;
            // plasma_fb[(index + 2) % (FB_WIDTH * FB_HEIGHT)] = 15 - color;
            // plasma_fb[(index + 3) % (FB_WIDTH * FB_HEIGHT)] = 15;

            canvas.setColor(plasma_fb[index]);

#ifdef PLASMA_LORES
            canvas.fillRect(x * FB_SCALE, y * FB_SCALE, FB_SCALE, FB_SCALE);
#else
            canvas.point(x, y);
#endif
        }
    }
}
