#pragma once

#include "core/gfx/Canvas.h"

class Life {
public:
    Life();

    void init();

    void update(float dt);
    void draw(Canvas &canvas);
    void evolve();

private:
    float _time;

    uint32_t _delay;
};
