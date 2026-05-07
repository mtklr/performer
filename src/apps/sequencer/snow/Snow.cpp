/* adapted from https://github.com/deadcodesoc/asnow.git */

#include "Snow.h"

#include "Config.h"

#include "os/os.h"

#include <ctime>

#define RANDF(x) ((float)rand()/((float)RAND_MAX/((float)x)))
#define MAX_SNOW 100
#define INTENSITY 20

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
} Snowflake;

typedef struct {
    size_t size;
    size_t used;
    Snowflake *flake;
} Snowfall;

static const float temperature = -8.0f;
static const int melt_threshold = (int) (-1.0f * temperature * (CONFIG_LCD_WIDTH * CONFIG_LCD_HEIGHT) / 70);
static const float Pi = 3.1415926536f;
static const uint8_t flake_colors[7] = { 0x3, 0x5, 0x7, 0x9, 0xa, 0xc, 0xf };

Snowfall *snow;
float wind;

// all-screensavers-enabled (sometimes) fails on stm32,
// prob because of bg[] and fire px[], too big for firmware...
// can use calloc for these and it works but not sure that's ok?
// uint8_t *bg; // calloc
static uint8_t bg[CONFIG_LCD_WIDTH * CONFIG_LCD_HEIGHT];

static void flake_init(Snowflake *flake, const int width) {
    flake->size = rand() % 2 + 1; // 1..2
    flake->color = flake_colors[rand() % 7];
    flake->x = RANDF(width);
    flake->y = 0.0f;
    flake->speed = 0.3f + RANDF(1.2f);
    flake->phase = RANDF(2.0f * Pi);
    flake->freq = RANDF(0.2f);
    flake->wobble = 0.5f + RANDF(2.5f);
    flake->falling = 1;
}

static Snowfall* snow_start(const int intensity, const int width) {
    Snowfall *snow = (Snowfall *) std::calloc(1, sizeof (Snowfall));
    // Snowfall *snow = new Snowfall; // works...

    if (snow == NULL) {
        // fprintf(stderr,"error: calloc snow\n");
        return NULL;
    }

    snow->flake = (Snowflake *) std::calloc(MAX_SNOW, sizeof(Snowflake));
    // snow->flake = new Snowflake; // ... crash, whyyyy... inside snow?

    if (snow->flake == NULL) {
        // fprintf(stderr,"error: calloc snow->flake\n");
        return NULL;
    }

    snow->size = MAX_SNOW;
    snow->used = intensity;

    for (size_t i = 0; i < snow->used; i++) {
        flake_init(&snow->flake[i], width);
    }

    return snow;
}

static int flake_blocked(Snowflake * flake, int x) {
    int y = (int) floorf(flake->y);

    if (y + flake->size > CONFIG_LCD_HEIGHT - 1) {
        flake->y = CONFIG_LCD_HEIGHT - flake->size;
        return 1;
    }

    for (int fs = 1; fs <= flake->size; fs++) {
        // below not blank
        if (bg[(y + 1) * CONFIG_LCD_WIDTH + x] != 0) {
            int dir = rand() % 2 * -2 + 1; // -1 or 1

            if (bg[(y + fs) * CONFIG_LCD_WIDTH + (x + dir)] == 0) {
                flake->x += dir;
                return 0;
            } else if (bg[(y + fs) * CONFIG_LCD_WIDTH + (x - dir)] == 0) {
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
    for (int x = 0; x < CONFIG_LCD_WIDTH; x++) {
        for (int y = CONFIG_LCD_HEIGHT - 1; y > 0; y--) {
            int index = y * CONFIG_LCD_WIDTH + x;

            // "drop" snow pile down 1 row
            bg[index] = bg[(y - 1) * CONFIG_LCD_WIDTH + x];
        }
    }
}

Snow::Snow() {
}

void Snow::init() {
    _time = 0.f;

    _delay = os::ticks();

    srand(time(NULL));

    // use calloc bg instead of static uint8_t above?
    // bg = (uint8_t *) std::calloc(CONFIG_LCD_WIDTH * CONFIG_LCD_HEIGHT, sizeof(uint8_t));

    wind = 0.1f * (rand() % 2 + 1);

    if (rand() % 2 == 0) {
        wind = -wind;
    };

    for (int i = 0; i < CONFIG_LCD_WIDTH * CONFIG_LCD_HEIGHT; i++) {
        bg[i] = 0;
    }

    snow = snow_start(INTENSITY, CONFIG_LCD_WIDTH);

    if (snow == NULL) {
        return;
    }
}

void Snow::cleanup() {
    if (snow->flake) {
        std::free(snow->flake);
        snow->flake = NULL;
        // fprintf(stderr, "freed snow->flake\n");
    }

    if (snow) {
        std::free(snow);
        snow = NULL;

        // fprintf(stderr,"freed snow\n");
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
        Snowflake *flake = &snow->flake[i];

        flake->x += wind;

        // wrap x from left to right
        if (flake->x < 0) {
            flake->x = CONFIG_LCD_WIDTH;
        }

        // % _WIDTH = wrap x from right to left
        int x = (int) floorf(flake->x + flake->wobble * sinf(flake->phase)) % CONFIG_LCD_WIDTH;

        if (flake_blocked(flake, x) == 1) {
            flake->falling = 0;

            // snow pile
            if (flake->color > 0x5) { // skip darker flakes
                for (int fx = 0; fx < flake->size; fx++) {
                    for (int fy = 0; fy < flake->size; fy++) {
                        bg[(int) (flake->y + fy) * CONFIG_LCD_WIDTH + (x + fx)] = rand() % 8 < 6 ? 0xf : 0xc;
                    }
                }
            }
        }

        if (flake->falling) {
            canvas.setColor(flake->color);
            canvas.drawRect(x, flake->y, flake->size, flake->size);

            flake->y += flake->speed;
            flake->phase += flake->freq;
        } else {
            flake_init(flake, CONFIG_LCD_WIDTH);
        }
    }

    // draw bg
    for (int x = 0; x < CONFIG_LCD_WIDTH; x++) {
        for (int y = 0; y < CONFIG_LCD_HEIGHT; y++) {
            if (bg[y * CONFIG_LCD_WIDTH + x] == 0) {
                continue;
            }
            canvas.setColor(bg[y * CONFIG_LCD_WIDTH + x]);
            canvas.point(x, y);
        }
    }

    if (rand() % melt_threshold == 0) {
        melt_flakes();
    }

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
