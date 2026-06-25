// adapted from - https://demo-effects.sourceforge.net

/*
Copyright (C) 2002 W.P. van Paassen - peter@paassen.tmfweb.nl

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2 of the License, or (at your
option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

note that the code has not been fully optimized
*/

#include "Stars.h"

#include "Config.h"

#include <ctime>

#define NUMBER_OF_STARS 200

typedef struct {
  float xpos, ypos;
  short zpos, speed;
  // uint8_t color;
} STAR;

STAR stars[NUMBER_OF_STARS];

static void init_star(STAR* star, int i) {
    // randomly init stars, generate them around the center of the screen
    star->xpos = -10.0 + (20.0 * (rand() / (RAND_MAX + 1.0)));
    star->ypos = -10.0 + (20.0 * (rand() / (RAND_MAX + 1.0)));

    star->xpos *= 3072.0; // change viewpoint
    star->ypos *= 3072.0;

    star->zpos = i;
    star->speed = 2 + (int) (2.0 * (rand() / (RAND_MAX + 1.0)));

    // star->color = i >> 2; // the closer to the viewer the brighter
}

Stars::Stars() {
}

void Stars::init() {
    _time = 0.f;

    srand(time(NULL));
}

void Stars::update(float dt) {
    _time += dt;
}

void Stars::draw(Canvas &canvas) {
    canvas.setBlendMode(BlendMode::Set);
    canvas.setColor(0);
    canvas.fill();

    int x, y;

    for (int i = 0; i < NUMBER_OF_STARS; i++) {
        stars[i].zpos -= stars[i].speed;

        if (stars[i].zpos <= 0) {
            init_star(stars + i, i + 1);
        }

        x = (stars[i].xpos / stars[i].zpos) + (CONFIG_LCD_WIDTH >> 1);
        y = (stars[i].ypos / stars[i].zpos) + (CONFIG_LCD_HEIGHT >> 1);

        // check if a star leaves the screen
        if (x < 0 || x > CONFIG_LCD_WIDTH - 1 || y < 0 || y > CONFIG_LCD_HEIGHT - 1) {
            init_star(stars + i, i + 1);
            continue;
        }

        if (stars[i].zpos > 70) {
            canvas.setColor(0x3);
            // canvas.drawText(x, y, "PER");
            canvas.point(x, y);
        } else if (stars[i].zpos > 30) {
            canvas.setColor(0x9);
            // canvas.drawText(x, y, "FOR");
            canvas.point(x, y);
        } else {
            canvas.setColor(0xf);
            // canvas.drawText(x, y, "MER");
            // canvas.point(x, y);
            canvas.fillRect(x, y, 2, 2);
        }
    }
}
