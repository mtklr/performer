// adapted from - https://invisiblewater.github.io/MattGraphicsTutorials/effects/plasma.html
// and - https://seancode.com/demofx/

#include "Plasma.h"

#include "Config.h"

#include <ctime>

// #define MOIRE 1
// #define PLASMA_LORES 1

#ifdef PLASMA_LORES
#define FB_SCALE 2
#else
#define FB_SCALE 1
#endif

#define FB_WIDTH CONFIG_LCD_WIDTH / FB_SCALE
#define FB_HEIGHT CONFIG_LCD_HEIGHT / FB_SCALE

#ifndef MOIRE
int a, b, c, d;

static float dist(int a, int b, int c, int d) {
    return std::sqrt((a - c) * (a - c) + (b - d) * (b - d));
}
#endif

Plasma::Plasma() {
}

void Plasma::init() {
    _time = 0.f;

    srand(time(NULL));

#ifndef MOIRE
    a = rand() % 64 + 64;
    b = rand() % 64 + 64;
    c = rand() % 96 + 96;
    d = rand() % 32;
#endif
}

void Plasma::update(float dt) {
    _time += dt;
}

void Plasma::draw(Canvas &canvas) {
    canvas.setBlendMode(BlendMode::Set);
    canvas.setColor(0);
    canvas.fill();

#ifdef MOIRE
    float cx1 = std::sin(_time / 2) * FB_WIDTH / 3 + FB_WIDTH / 2;
    float cy1 = std::sin(_time / 4) * FB_HEIGHT / 3 + FB_HEIGHT / 2;
    float cx2 = std::cos(_time / 3) * FB_WIDTH / 3 + FB_WIDTH / 2;
    float cy2 = std::cos(_time) * FB_HEIGHT / 3 + FB_HEIGHT / 2;

    for (int y = 0; y < FB_HEIGHT; y++) {
        float dy = (y - cy1) * (y - cy1);
        float dy2 = (y - cy2) * (y - cy2);

        for (int x = 0; x < FB_WIDTH; x++) {
            float dx = (x - cx1) * (x - cx1);
            float dx2 = (x - cx2) * (x - cx2);
            uint8_t color = ((((int) sqrt(dx + dy) ^ (int) sqrt(dx2 + dy2)) >> 3) & 1) * 15;

            canvas.setColor(color % 16);
#ifdef PLASMA_LORES
            canvas.fillRect(x * FB_SCALE, y * FB_SCALE, FB_SCALE, FB_SCALE);
#else
            canvas.point(x, y);
#endif
        }
    }
#else
    for (int y = 0; y < FB_HEIGHT; y++) {
        for (int x = 0; x < FB_WIDTH; x++) {
            float v = std::sin(dist(x + (_time * 20), y, a, b) / 8) +
                std::sin(dist(x, y, 64, 64) / 8) +
                std::sin(dist(x, y + (_time * 20) / 7, c, d) / 7) +
                std::sin(dist(x, y, 192, 100) / 8);
            uint8_t color = (int) (4 + std::floor(v)) * 2;

            canvas.setColor(color % 16);
#ifdef PLASMA_LORES
            canvas.fillRect(x * FB_SCALE, y * FB_SCALE, FB_SCALE, FB_SCALE);
#else
            canvas.point(x, y);
#endif
        }
    }
#endif // MOIRE
}
