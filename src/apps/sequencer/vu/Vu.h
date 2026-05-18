#pragma once

#include "core/gfx/Canvas.h"

class Vu {
public:
    Vu();

    void init();

    void update(float dt, uint8_t gates);
    void draw(Canvas &canvas);

private:
    float _time;

    uint8_t _pulse_state;
};
