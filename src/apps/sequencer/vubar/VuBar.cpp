#include "VuBar.h"

#include "Config.h"

typedef struct {
    float x, y, dx, dy;
} Bar;

const int bar_width = 2;
const int bar_pad = 16;
const int bar_space_used = (bar_width * 8) + (bar_pad * 7);
const int x_start = (CONFIG_LCD_WIDTH - bar_space_used) / 2;

const int y_min = 10;
const int y_max = 48;
const float dy = 3.2f;

static Bar bars[8];

static void step(Bar *bar, bool pulse) {
    bar->y += bar->dy;
    bar->dy = dy;

    if (pulse) {
        bar->dy *= -1;
        bar->dy *= 4;
    } else {
        bar->dy -= 2;
    }

    if (bar->y < y_min) {
        bar->y = y_min;
    }

    if (bar->y > y_max) {
        bar->y = y_max;
    }
}

VuBar::VuBar() {
}

void VuBar::init() {
    _time = 0.f;
    _pulse_state = 0;

    for (int i = 0; i < 8; i++) {
        bars[i].x = x_start + (bar_width + bar_pad) * i;
        bars[i].y = y_max;
        bars[i].dx = 0;
        bars[i].dy = 0;
    }
}

void VuBar::update(float dt, uint8_t gates) {
    _time += dt;
    _pulse_state = gates;
}

void VuBar::draw(Canvas &canvas) {
    canvas.setBlendMode(BlendMode::Set);
    canvas.setColor(0);
    canvas.fill();

    canvas.setBlendMode(BlendMode::Add);
    canvas.setColor(0xa);

    for (int i = 0; i < 8; ++i) {
        Bar *bar = &bars[i];
        bool pulse = (_pulse_state >> (i)) & 1;

        for (int w = 0; w < bar_width; w++) {
            canvas.line(bar->x + w, y_max, bar->x + w, bar->y);
        }

        step(bar, pulse);
    }
}
