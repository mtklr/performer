// https://www.ninjaforce.com/html/special_demoscene_modulae.html

#include "VuLine.h"

#include "Config.h"

// #define MULTI_LINE
// #define VERT_LINE
// #define LINE_POINTS
// #define EDGE_LINES

typedef struct {
    float x, y, dx, dy;
} Bar;

#ifdef MULTI_LINE
const int line_length = 26;
#else
const int line_length = 28;
#endif

#ifdef MULTI_LINE
const int line_offset = 10;
const int x_start = (CONFIG_LCD_WIDTH - (line_length * 7)) / 2 - (line_offset / 2);
#else
const int x_start = (CONFIG_LCD_WIDTH - (line_length * 7)) / 2;
#endif

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
        canvas.line(0, y_max, x_start - line_length, y_max);
        canvas.line(x_end, y_max, CONFIG_LCD_WIDTH - 1, y_max);
#ifdef MULTI_LINE
        int o = line_offset;

        canvas.line(0, y_max + o, x_start - line_length + o, y_max + o);
        canvas.line(x_end + o, y_max + o, CONFIG_LCD_WIDTH - 1 , y_max + o);
#endif
#endif

    for (int i = -1; i < 8; ++i) {
        Bar *bar = i < 0 ? &bar_start : &bars[i];
        Bar *bar_next = i < 7 ? &bars[i+1] : &bar_end;

        bool pulse = (_pulse_state >> (i)) & 1;

#ifdef LINE_POINTS
        canvas.point(bar->x, bar->y);
#endif

        canvas.line(bar->x, bar->y, bar_next->x, bar_next->y); // upper line

#ifdef MULTI_LINE
        int o = line_offset;

#ifdef LINE_POINTS
        canvas.point(bar->x + o, bar->y + o);
#endif

        canvas.line(bar->x, bar->y, bar->x + o, bar->y + o); // connecting segments (w/ first segment)
        canvas.line(bar->x + o, bar->y + o, bar_next->x + o, bar_next->y + o); // lower line

        // bars
        // if (i < 0) { // skip first connecting line
        //     continue;
        // }

        // angled bars
        // for (int j = y_min - line_offset + o; j < y_max + line_offset - bar->y; j++) {
        //     canvas.line(bar->x, bar->y - line_offset + o + j - 5, bar->x + o, bar->y - line_offset + o + j + 5);
        // }

        // straight bars
        // for (int j = y_min - line_offset + o; j < y_max + line_offset - bar->y; j++) {
        //     canvas.line(bar->x, bar->y - line_offset + o + j - 5, bar->x + o, bar->y - line_offset + o + j - 5);
        // }

        // straight lines
        // for (int j = y_min - line_offset + o; j < y_max + line_offset - bar->y; j+=2) {
        //     canvas.line(bar->x, bar->y - line_offset + o + j - 5, bar->x + o, bar->y - line_offset + o + j - 5);
        // }
#endif // MULTI_LINE

#ifdef VERT_LINE
        canvas.vline(bar->x, bar->y, CONFIG_LCD_HEIGHT - bar->y);
#ifdef MULTI_LINE
        canvas.vline(bar->x + o, bar->y + o, CONFIG_LCD_HEIGHT - bar->y);
#endif
#endif

        // end
        if (i > 6) {
#ifdef LINE_POINTS
            canvas.point(bar_next->x, bar_next->y);
#endif

#ifdef MULTI_LINE
            canvas.line(bar_next->x, bar_next->y, bar_next->x + o, bar_next->y + o); // end segment
#ifdef LINE_POINTS
            canvas.point(bar_next->x + o, bar_next->y + o);
#endif
#endif

#ifdef VERT_LINE
            canvas.vline(bar_next->x, bar_next->y, CONFIG_LCD_HEIGHT - bar_next->y);
#ifdef MULTI_LINE
            canvas.vline(bar_next->x + o, bar_next->y + o, CONFIG_LCD_HEIGHT - bar_next->y);
#endif
#endif
        }

        step(bar, pulse);
    }
}
