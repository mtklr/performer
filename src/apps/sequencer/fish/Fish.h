#pragma once

#include "core/gfx/Canvas.h"

class Fish {
public:
    Fish();

    void init();

    void update(float dt);
    void draw(Canvas &canvas);

private:
    float _time;

    uint32_t _delay;
};
