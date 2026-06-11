// adapted from https://rosettacode.org/wiki/Conway's_Game_of_Life#C
// and https://github.com/clckwrkbdgr/life

#include "Life.h"

#include "Config.h"

#include "os/os.h"

#include <ctime>

#define CELL_OUTLINE 1
#define CELL_SIZE 4
#define CELL_PAD 1
#define LIFE_HEIGHT (CONFIG_LCD_HEIGHT / CELL_SIZE)
#define LIFE_WIDTH (CONFIG_LCD_WIDTH / CELL_SIZE)
#define HASH_COUNT 10

bool univ[LIFE_HEIGHT * LIFE_WIDTH];
bool newu[LIFE_HEIGHT * LIFE_WIDTH];

Life::Life() {
}

void Life::init() {
    _time = 0.f;

    _delay = os::ticks();

    srand(time(NULL));

    for (int y = 0; y < LIFE_HEIGHT; y++) {
        for (int x = 0; x < LIFE_WIDTH; x++) {
            univ[y * LIFE_WIDTH + x] = rand() % 2 ? 1 : 0;
        }
    }
}

void Life::update(float dt) {
    _time += dt;
}

void Life::draw(Canvas &canvas) {
    canvas.setBlendMode(BlendMode::Set);
    canvas.setColor(0);
    canvas.fill();

    canvas.setColor(0xc);

    for (int y = 0; y < LIFE_HEIGHT; y++) {
        for (int x = 0; x < LIFE_WIDTH; x++) {
            if (univ[y * LIFE_WIDTH + x] == 1) {
#ifdef CELL_OUTLINE
                canvas.drawRect(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE - CELL_PAD, CELL_SIZE - CELL_PAD);
#else
                canvas.fillRect(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE - CELL_PAD, CELL_SIZE - CELL_PAD);
#endif
            }
        }
    }

    if (os::ticks() > _delay + os::time::ms(100)) {
        _delay = os::ticks();
        evolve();
    }
}

void Life::evolve() {
    int g, n;
    uint32_t hash, currentHash;

    static int current_hash_index;
    static unsigned int hashes[HASH_COUNT];
    static int loopcount = 0;

    hash = 0;

    for (int y = 0; y < LIFE_HEIGHT; y++) {
        for (int x = 0; x < LIFE_WIDTH; x++) {
            n = 0;

            for (int y1 = y - 1; y1 <= y + 1; y1++) {
                for (int x1 = x - 1; x1 <= x + 1; x1++) {
                    if (univ[((y1 + LIFE_HEIGHT) % LIFE_HEIGHT) * LIFE_WIDTH + ((x1 + LIFE_WIDTH) % LIFE_WIDTH)]) {
                        n++;
                    }
                }
            }

            if (univ[y * LIFE_WIDTH + x]) {
                n--;
            }

            newu[y * LIFE_WIDTH + x] = (n == 3 || (n == 2 && univ[y * LIFE_WIDTH + x]));

            hash = (hash << 4) + univ[y * LIFE_WIDTH + x];
            if((g = (hash & 0xf0000000))) {
                hash ^= g >> 23;
            }

            hash &= ~g;
        }
    }

    for (int y = 0; y < LIFE_HEIGHT; y++) {
        for (int x = 0; x < LIFE_WIDTH; x++) {
            univ[y * LIFE_WIDTH + x] = newu[y * LIFE_WIDTH + x];
        }
    }

    currentHash = hash;

    for (int i = 0; i < HASH_COUNT; i++) {
        bool loopDetected = hashes[i] == currentHash;
        if (loopDetected) {
            _delay = os::ticks() + os::time::ms(100 + (loopcount * 100));

            if (loopcount > 10) {
                init();
                loopcount = 0;
            }

            loopcount++;
            break;
        }
    }

    hashes[current_hash_index] = currentHash;
    current_hash_index++;

    while (current_hash_index >= HASH_COUNT) {
        current_hash_index -= HASH_COUNT;
    }
}
