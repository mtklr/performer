#pragma once

#include "core/gfx/Canvas.h"

class Snow {
public:
    Snow();

    void init();

    void update(float dt);
    void draw(Canvas &canvas);
    void cleanup();

private:
    float _time;

    uint32_t _delay;
};
