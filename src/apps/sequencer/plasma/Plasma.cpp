// adapted from - https://invisiblewater.github.io/MattGraphicsTutorials/effects/plasma.html

#include "Plasma.h"

#include "Config.h"

#include "os/os.h"

uint8_t plasma_fb[CONFIG_LCD_WIDTH * CONFIG_LCD_HEIGHT];

static float dist(int a, int b, int c, int d) {
    return sqrt((a - c) * (a - c) + (b - d) * (b - d));
}

Plasma::Plasma() {
}

void Plasma::init() {
    _time = 0.f;
}

void Plasma::update(float dt) {
    _time += dt;
}

void Plasma::draw(Canvas &canvas) {
    canvas.setBlendMode(BlendMode::Set);
    canvas.setColor(0);
    canvas.fill();

    int plasma_time = os::ticks() / 50;

    for (int y = 0; y < CONFIG_LCD_HEIGHT; y++) {
        for (int x = 0; x < CONFIG_LCD_WIDTH; x++) {
            float value = sin(dist(x + plasma_time, y, 128, 128) / 8) +
                sin(dist(x, y, 64, 64) / 8) +
                sin(dist(x, y + plasma_time / 7, 192, 64) / 7) +
                sin(dist(x, y, 192, 100) / 8);

            int color = ((4 + (int) value) * 2) % 16;

            int id = y * CONFIG_LCD_WIDTH + x;

            plasma_fb[id] = color;
            // plasma_fb[id + 1] = color * 2;
            // plasma_fb[id + 2] = 15 - color;
            // plasma_fb[id + 3] = 15;

            canvas.setColor(plasma_fb[id]);
            canvas.point(x, y);
        }
    }
}
