/* adapted from - https://github.com/godfreybrew/Ball-Simulation-in-C.git */
/* inspiration - http://freetoolsassociation.com/detail_snapshot_Noise_Tracker_noisetracker_3.html */

#include "Bounce.h"

#include "Config.h"

#include <ctime>

#define GRAVITY 0.4
#define DAMPENING 0.7
#define MAX_CIRCLES 20
#define CIRCLE_RADIUS 4

const int note_colors[7] = { 0x3, 0x5, 0x7, 0x9, 0xa, 0xc, 0xf };
int num_circles;

typedef struct {
    float x, y, radius, dx, dy;
    uint8_t color;
    bool flag;
} Circle;

static Circle circles[MAX_CIRCLES];

static void step(Circle *circle, float dymax) {
    circle->x += circle->dx;
    circle->y += circle->dy;
    circle->dy += GRAVITY;

    // collision with top
    if (circle->y < 0) {
        circle->y = 0;
        circle->dy *= -1; // * DAMPENING;
    }

    // collision with bottom
    if (circle->y + circle->radius > CONFIG_LCD_HEIGHT) {
        circle->y = CONFIG_LCD_HEIGHT - circle->radius;
        circle->dy *= -1; // * DAMPENING;
    }

    //collision with right
    if (circle->x + circle->radius > CONFIG_LCD_WIDTH) {
        circle->x = CONFIG_LCD_WIDTH - circle->radius;
        circle->dx *= -1; // * DAMPENING;
    }

    //collision with left
    if (circle->x < 0) {
        circle->x = 0;
        circle->dx *= -1; // * DAMPENING;
    }

    if (circle->dy > 6) {
        circle->dy = dymax;
    }
}

Bounce::Bounce() {
}

void Bounce::init() {
    _time = 0.f;

    srand(time(NULL));

    // single random value for each circle ...
    // int rx = rand() % 2 + 2;
    // int ry = rand() % 2 + 2;

    for (int i = 0; i < MAX_CIRCLES; i++) {
        circles[i].x = rand() % (CONFIG_LCD_WIDTH - 10) + 5;
        circles[i].y = rand() % (CONFIG_LCD_HEIGHT - 10) + 5;
        circles[i].radius = CIRCLE_RADIUS;

        // ...
        // circles[i].dx = rx;
        // circles[i].dy = ry;

        // ... or random per each circle
        circles[i].dx = rand() % 2 + 3;
        circles[i].dy = rand() % 2 + 3;

        circles[i].color = note_colors[rand() % 7];
        circles[i].flag = rand() % 10 < 3 ? true : false;;
    }

    num_circles = rand() % (MAX_CIRCLES / 2) + (MAX_CIRCLES / 2) + 1;
}

void Bounce::update(float dt) {
    _time += dt;
}

void Bounce::draw(Canvas &canvas) {
    canvas.setBlendMode(BlendMode::Set);
    canvas.setColor(0);
    canvas.fill();

    for (int i = 0; i < num_circles; i++) {
        Circle *circle = &circles[i];

        canvas.setColor(circle->color);
        canvas.fillRect(circle->x, circle->y, circle->radius, circle->radius);

        canvas.setBlendMode(BlendMode::Set);
        canvas.setColor(0);

        canvas.point(circle->x, circle->y);
        canvas.point(circle->x + (circle->radius - 1), circle->y);
        canvas.point(circle->x, circle->y + (circle->radius - 1));
        canvas.point(circle->x + (circle->radius - 1), circle->y + (circle->radius - 1));

        canvas.setBlendMode(BlendMode::Add);
        canvas.setColor(circle->color);

        canvas.vline(circle->x + (circle->radius - 1), circle->y - (circle->radius + 2), circle->radius + 3);

        if (circle->flag) {
            canvas.hline(circle->x + circle->radius, circle->y - (circle->radius + 1), 1);
            canvas.hline(circle->x + circle->radius, circle->y - circle->radius, 2);
        }

        step(circle, circle->dy);
    }
}
