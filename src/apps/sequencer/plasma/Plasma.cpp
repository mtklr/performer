// adapted from - https://invisiblewater.github.io/MattGraphicsTutorials/effects/plasma.html

#include "Plasma.h"

#include "Config.h"

#include "os/os.h"

#include <ctime>

uint8_t buffer[CONFIG_LCD_WIDTH * CONFIG_LCD_HEIGHT];

static float dist(int a, int b, int c, int d) {
    return sqrt((a - c) * (a - c) + (b - d) * (b - d));
}

Plasma::Plasma() {
}

void Plasma::init() {
    _time = 0.f;

    srand(time(NULL));
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

            buffer[id] = color;
            buffer[id + 1] = color * 2;
            buffer[id + 2] = 15 - color;
            buffer[id + 3] = 15;

            canvas.setColor(buffer[id]);
            canvas.point(x, y);
        }
    }
}
