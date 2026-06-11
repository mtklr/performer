/* adapted from https://github.com/deadcodesoc/asnow.git */

#include "Snow.h"

#include "Config.h"

#include "os/os.h"

#include <ctime>

#define RANDF(x) ((float)rand()/((float)RAND_MAX/((float)x)))
#define MAX_SNOW 100
#define INTENSITY 20

// #define SNOW_LORES 1
// #define SNOW_PILE 1
// #define SNOW_SHAPES 1

#ifdef SNOW_LORES
#define FB_SCALE 2
#else
#define FB_SCALE 1
#endif

#define FB_WIDTH CONFIG_LCD_WIDTH / FB_SCALE
#define FB_HEIGHT CONFIG_LCD_HEIGHT / FB_SCALE

typedef struct {
    int size;
    uint8_t color;
    float x;
    float y;
    float speed;
    float phase;
    float freq;
    float wobble;
    int falling;
#ifdef SNOW_SHAPES
    int shape;
#endif
} Snowflake;

typedef struct {
    size_t size;
    size_t used;
    Snowflake flake[MAX_SNOW];
} Snowfall;

#ifdef SNOW_PILE
static const float temperature = -8.0f;
static const int melt_threshold = (int) (-1.0f * temperature * (FB_WIDTH * FB_HEIGHT) / 70);
#endif
static const float Pi = 3.1415926536f;
static const uint8_t flake_colors[7] = { 0x3, 0x5, 0x7, 0x9, 0xa, 0xc, 0xf };

Snowfall snow;
float wind;

#ifdef SNOW_PILE
static uint8_t snow_fb[FB_WIDTH * FB_HEIGHT];
#endif

static void flake_init(Snowflake *flake, const int width) {
    flake->size = rand() % 2 + 1; // 1..2
    flake->color = flake_colors[rand() % 7];
    flake->x = RANDF(width);
    flake->y = 0.0f;
    flake->speed = (0.3f + RANDF(1.2f)) / FB_SCALE;
    flake->phase = RANDF(2.0f * Pi);
    flake->freq = RANDF(0.2f);
    flake->wobble = 0.5f + RANDF(2.5f);
    flake->falling = 1;
#ifdef SNOW_SHAPES
    flake->shape = rand() % 6;
#endif
}

static Snowfall* snow_start(const int intensity, const int width) {
    Snowfall *s = &snow;
    s->size = MAX_SNOW;
    s->used = intensity;

    for (size_t i = 0; i < s->used; i++) {
        flake_init(&s->flake[i], width);
    }

    return s;
}

#ifdef SNOW_PILE
static int flake_blocked(Snowflake * flake, int x) {
    int y = (int) floorf(flake->y);

    if (y + flake->size > FB_HEIGHT - 1) {
        flake->y = FB_HEIGHT - flake->size;
        return 1;
    }

    for (int fs = 1; fs <= flake->size; fs++) {
        // below not blank
        if (snow_fb[(y + 1) * FB_WIDTH + x] != 0) {
            int dir = rand() % 2 * -2 + 1; // -1 or 1

            if (snow_fb[(y + fs) * FB_WIDTH + (x + dir)] == 0) {
                flake->x += dir;
                return 0;
            } else if (snow_fb[(y + fs) * FB_WIDTH + (x - dir)] == 0) {
                flake->x -= dir;
                return 0;
            } else {
                return 1;
            }
        }
    }

    return 0;
}

static void melt_flakes() {
    for (int x = 0; x < FB_WIDTH; x++) {
        for (int y = FB_HEIGHT - 1; y > 0; y--) {
            int index = y * FB_WIDTH + x;

            // move snow pile down 1 row
            snow_fb[index] = snow_fb[(y - 1) * FB_WIDTH + x];
        }
    }
}
#endif

Snow::Snow() {
}

void Snow::init() {
    _time = 0.f;

    _delay = os::ticks();

    srand(time(NULL));

    wind = 0.1f * (rand() % 2 + 1);

    if (rand() % 2 == 0) {
        wind = -wind;
    };

#ifdef SNOW_PILE
    for (int i = 0; i < FB_WIDTH * FB_HEIGHT; i++) {
        snow_fb[i] = 0;
    }
#endif

    Snowfall *s = &snow;
    s = snow_start(INTENSITY, FB_WIDTH);

    if (s == NULL) {
        return;
    }
}

void Snow::update(float dt) {
    _time += dt;
}

void Snow::draw(Canvas &canvas) {
    canvas.setBlendMode(BlendMode::Set);
    canvas.setColor(0);
    canvas.fill();

    for (int i = 0; i < INTENSITY; i++) {
        Snowflake *flake = &snow.flake[i];

        flake->x += wind;

        // wrap x from left to right
        if (flake->x * 2 < 0) {
            flake->x = FB_WIDTH;
        }

        // % _WIDTH = wrap x from right to left
        int x = (int) floorf(flake->x * FB_SCALE + flake->wobble * sinf(flake->phase)) % FB_WIDTH;

#ifdef SNOW_PILE
        if (flake_blocked(flake, x) == 1) {
            flake->falling = 0;

            // snow pile
            if (flake->color > 0x5) { // skip darker flakes
                for (int fx = 0; fx < flake->size; fx++) {
                    for (int fy = 0; fy < flake->size; fy++) {
                        snow_fb[(int) (flake->y + fy) * FB_WIDTH + (x + fx)] = rand() % 8 < 6 ? 0xf : 0xc;
                    }
                }
            }
        }
#else
        if (flake->y > FB_HEIGHT - 1) {
            flake->falling = 0;
        }
#endif

        if (flake->falling) {
            canvas.setColor(flake->color);

#ifdef SNOW_SHAPES
            switch (flake->shape) {
                case 0:
                    canvas.point(x * FB_SCALE, flake->y * FB_SCALE);
                    break;
                case 1:
                    canvas.drawRect(x * FB_SCALE, flake->y * FB_SCALE, flake->size * FB_SCALE, flake->size * FB_SCALE);
                    break;
                case 2:
                    canvas.fillRect(x * FB_SCALE, flake->y * FB_SCALE, flake->size * FB_SCALE, flake->size * FB_SCALE);
                    break;
                case 3:
                    canvas.drawRect(x * FB_SCALE, flake->y * FB_SCALE + 1, 4, 2);
                    canvas.drawRect(x * FB_SCALE + 1, flake->y * FB_SCALE, 2, 4);
                    break;
                case 4:
                    canvas.hline(x * FB_SCALE + 1, flake->y * FB_SCALE, 2);
                    canvas.hline(x * FB_SCALE + 1, flake->y * FB_SCALE + 3, 2);
                    canvas.vline(x * FB_SCALE, flake->y * FB_SCALE + 1, 2);
                    canvas.vline(x * FB_SCALE + 3, flake->y * FB_SCALE + 1, 2);
                    break;
                case 5:
                    canvas.point(x * FB_SCALE, flake->y * FB_SCALE);
                    canvas.point(x * FB_SCALE + 3, flake->y * FB_SCALE);
                    canvas.fillRect(x * FB_SCALE + 1, flake->y * FB_SCALE + 1, 2, 2);
                    canvas.point(x * FB_SCALE, flake->y * FB_SCALE + 3);
                    canvas.point(x * FB_SCALE + 3, flake->y * FB_SCALE + 3);
                    break;
                default:
                    break;
            }
#else
            canvas.fillRect(x * FB_SCALE, flake->y * FB_SCALE, flake->size * FB_SCALE, flake->size * FB_SCALE);
#endif

            flake->y += flake->speed;
            flake->phase += flake->freq;
        } else {
            flake_init(flake, FB_WIDTH);
        }
    }

#ifdef SNOW_PILE
    // draw snow_fb
    for (int x = 0; x < FB_WIDTH; x++) {
        for (int y = 0; y < FB_HEIGHT; y++) {
            if (snow_fb[y * FB_WIDTH + x] == 0) {
                continue;
            }
            canvas.setColor(snow_fb[y * FB_WIDTH + x]);
            canvas.fillRect(x * FB_SCALE, y * FB_SCALE, FB_SCALE, FB_SCALE);
        }
    }

    if (rand() % melt_threshold == 0) {
        melt_flakes();
    }
#endif

    if (os::ticks() > _delay + os::time::ms(5000)) {
        _delay = os::ticks();

        if (rand() % 2 == 0) {
            if (wind > -1.0f) {
                wind -= 0.1f;
            }
        } else {
            if (wind < 1.0f) {
                wind += 0.1f;
            }
        }
    }
}
