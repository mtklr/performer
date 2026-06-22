// adapted from - https://github.com/godfreybrew/Ball-Simulation-in-C.git
// inspiration - http://freetoolsassociation.com/detail_snapshot_Noise_Tracker_noisetracker_3.html

#include "Bounce.h"

#include "Config.h"

#include <ctime>

#define GRAVITY 0.4
#define DAMPENING 0.7
#define MAX_NOTES 12
#define NOTE_RADIUS 4

typedef struct {
    float x, y, radius, dx, dy;
    uint8_t color;
    bool flag;
} note;

static const int note_colors[7] = { 0x3, 0x5, 0x7, 0x9, 0xa, 0xc, 0xf };

int num_notes;
note notes[MAX_NOTES];

static void step(note *note, float dymax) {
    note->x += note->dx;
    note->y += note->dy;
    note->dy += GRAVITY;

    // collision with top
    if (note->y < 0) {
        note->y = 0;
        note->dy *= -1; // * DAMPENING;
    }

    // collision with bottom
    if (note->y + note->radius > CONFIG_LCD_HEIGHT) {
        note->y = CONFIG_LCD_HEIGHT - note->radius;
        note->dy *= -1; // * DAMPENING;
    }

    //collision with right
    if (note->x + note->radius > CONFIG_LCD_WIDTH) {
        note->x = CONFIG_LCD_WIDTH - note->radius;
        note->dx *= -1; // * DAMPENING;
    }

    //collision with left
    if (note->x < 0) {
        note->x = 0;
        note->dx *= -1; // * DAMPENING;
    }

    if (note->dy > 6) {
        note->dy = dymax;
    }
}

Bounce::Bounce() {
}

void Bounce::init() {
    _time = 0.f;

    srand(time(NULL));

    // single random value for each note ...
    // int rx = rand() % 2 + 2;
    // int ry = rand() % 2 + 2;

    num_notes = rand() % MAX_NOTES + 1;

    for (int i = 0; i < num_notes; i++) {
        notes[i].x = rand() % (CONFIG_LCD_WIDTH - 10) + 5;
        notes[i].y = rand() % (CONFIG_LCD_HEIGHT - 10) + 5;
        notes[i].radius = NOTE_RADIUS;

        // ...
        // notes[i].dx = rx;
        // notes[i].dy = ry;

        // ... or random per each note
        notes[i].dx = rand() % 2 + 3;
        notes[i].dy = rand() % 2 + 3;

        notes[i].color = note_colors[rand() % 7];
        notes[i].flag = rand() % 10 < 3 ? true : false;
    }
}

void Bounce::update(float dt) {
    _time += dt;
}

void Bounce::draw(Canvas &canvas) {
    canvas.setBlendMode(BlendMode::Set);
    canvas.setColor(0);
    canvas.fill();

    for (int i = 0; i < num_notes; i++) {
        note *note = &notes[i];

        canvas.setColor(note->color);
        canvas.fillRect(note->x, note->y, note->radius, note->radius);

        canvas.setBlendMode(BlendMode::Set);
        canvas.setColor(0);

        canvas.point(note->x, note->y);
        canvas.point(note->x + (note->radius - 1), note->y);
        canvas.point(note->x, note->y + (note->radius - 1));
        canvas.point(note->x + (note->radius - 1), note->y + (note->radius - 1));

        canvas.setBlendMode(BlendMode::Add);
        canvas.setColor(note->color);

        canvas.vline(note->x + (note->radius - 1), note->y - (note->radius + 2), note->radius + 3);

        if (note->flag) {
            canvas.hline(note->x + note->radius, note->y - (note->radius + 1), 1);
            canvas.hline(note->x + note->radius, note->y - note->radius, 2);
        }

        step(note, note->dy);
    }
}
