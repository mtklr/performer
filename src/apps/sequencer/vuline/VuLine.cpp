// https://www.ninjaforce.com/html/special_demoscene_modulae.html

#include "VuLine.h"

#include "Config.h"

// #define EDGE_LINES

typedef struct {
    float x, y, dx, dy;
} Bar;

const int line_length = 28;
const int line_width = 1;
const int x_start = (CONFIG_LCD_WIDTH - (line_length * 7)) / 2;

#ifdef EDGE_LINES
const int x_end = x_start + (line_length * 8);
#endif

const int y_min = 10;
const int y_max = 48;
const float dy = 3.2f;

static Bar bars[8];
static Bar bar_start, bar_end;

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

VuLine::VuLine() {
}

void VuLine::init() {
    _time = 0.f;
    _pulse_state = 0;

    for (int i = 0; i < 8; i++) {
        bars[i].x = x_start + (line_length * i);
        bars[i].y = y_max;
        bars[i].dx = 0;
        bars[i].dy = 0;
    }

    bar_start.x = x_start - line_length;
    bar_start.y = y_max;
    bar_start.dx = 0;
    bar_start.dy = 0;

    bar_end.x = bars[7].x + line_length;
    bar_end.y = y_max;
    bar_end.dx = 0;
    bar_end.dy = 0;
}

void VuLine::update(float dt, uint8_t gates) {
    _time += dt;
    _pulse_state = gates;
}

void VuLine::draw(Canvas &canvas) {
    canvas.setBlendMode(BlendMode::Set);
    canvas.setColor(0);
    canvas.fill();

    canvas.setBlendMode(BlendMode::Add);
    canvas.setColor(0xa);

#ifdef EDGE_LINES
    for (int w = 0; w < line_width; w++) {
        canvas.line(0, y_max + w, x_start - line_length, y_max + w);
        canvas.line(x_end, y_max + w, CONFIG_LCD_WIDTH - 1, y_max + w);
    }
#endif

    for (int i = -1; i < 8; ++i) {
        Bar *bar = i < 0 ? &bar_start : &bars[i];
        Bar *bar_next = i < 7 ? &bars[i+1] : &bar_end;

        bool pulse = (_pulse_state >> (i)) & 1;

        for (int w = 0; w < line_width; w++) {
            canvas.line(bar->x, bar->y + w, bar_next->x, bar_next->y + w);
        }

        step(bar, pulse);
    }
}
