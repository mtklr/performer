#include "Vu.h"

#include "core/math/Math.h"
#include "core/math/Vec3.h"
#include "core/math/Mat4.h"

typedef struct {
    float x, y, dx, dy;
} Bar;

static Bar bars[8];

const int y_min = -36;
const int y_max = 0;
const float dy = 3.2f;

const Vec3 boxVertices[] = {
    { -1.f, -1.f, -1.f },
    { -1.f, -1.f,  1.f },
    { -1.f,  1.f, -1.f },
    { -1.f,  1.f,  1.f },
    {  1.f, -1.f, -1.f },
    {  1.f, -1.f,  1.f },
    {  1.f,  1.f, -1.f },
    {  1.f,  1.f,  1.f },
};

const uint8_t boxLineIndices[] = {
    0, 1,
    1, 3,
    3, 2,
    2, 0,

    4, 5,
    5, 7,
    7, 6,
    6, 4,

    0, 4,
    1, 5,
    2, 6,
    3, 7
};

static void transformVertices(const Mat4 &transform, const Vec3 *src, Vec3 *dst, size_t count) {
    for (size_t i = 0; i < count; ++i) {
        dst[i] = transform * src[i];
    }
}

static void step(Bar *bar, bool pulse) {
    bar->y += bar->dy;
    bar->dy = dy;

    if (pulse) {
        bar->dy *= -1;
        bar->dy *= 4;
    } else {
        bar->dy -= 2;
    }

    if (bar->y < y_min) {
        bar->y = y_min;
    }

    if (bar->y > y_max) {
        bar->y = y_max;
    }
}

Vu::Vu() {
}

void Vu::init() {
    _time = 0.f;
    _pulse_state = 0;
}

void Vu::update(float dt, uint8_t gates) {
    _time += dt;
    _pulse_state = gates;
}

void Vu::draw(Canvas &canvas) {
    canvas.setBlendMode(BlendMode::Set);
    canvas.setColor(0);
    canvas.fill();
    canvas.setColor(0xf);

    //Vec3 eye(3.f, 3.f, -22.f); // centered x, y
    Vec3 eye(3.f, 6.f, -22.f);
    // Vec3 target(3.f); // centered, "2d"
    Vec3 target(3.f, 9.f, 0.f);
    Vec3 up(0.f, 1.f, 0.f); // leave as-is
    Mat4 viewMatrix = Mat4::lookAt(eye, target - eye, up);

    Mat4 projMatrix = Mat4::perspective(deg2rad(20.f), 1.f, 0.1f, 40.f); // centered

    Vec2 positions[8];

    canvas.setBlendMode(BlendMode::Add);
    canvas.setColor(0xa);

    for (int instance = -3; instance <= 4; ++instance) {
        Bar *bar = &bars[instance + 3];
        bool pulse = (_pulse_state >> (4 - instance)) & 1;

        Mat4 modelMatrix = Mat4::translate(Vec3(instance * 6.f, -3.f, 0.f))* Mat4::rotXYZ(Vec3((_time + instance) * 0.7, 0, 1.57f));

        Mat4 modelViewProjMatrix = projMatrix * viewMatrix * modelMatrix;

        for (int i = 0; i < 8; ++i) {
            const auto &src = boxVertices[i];
            Vec4 srch(src);
            Vec4 dsth = modelViewProjMatrix * srch;
            Vec2 pos(dsth.x / dsth.w, dsth.y / dsth.w);
            positions[i] = pos * 20.f + Vec2(128.f, 32.f);
        }

        for (int i = 0; i < 8; ++i) {
            const auto &pos = positions[i];
            int x = std::round(pos.x);
            int y = std::round(pos.y);
            if (i < 4) {
                canvas.point(x, y + bar->y); // top
            } else {
                canvas.point(x, y);
            }
        }

        for (int i = 0; i < 12; ++i) {
            const auto &p0 = positions[boxLineIndices[i * 2]];
            const auto &p1 = positions[boxLineIndices[i * 2 + 1]];

            if (i < 4) {
                canvas.line(p0.x, p0.y + bar->y, p1.x, p1.y + bar->y); // top
            } else if (i > 7 && i < 12) {
                canvas.line(p0.x, p0.y + bar->y, p1.x, p1.y); // sides
            } else {
                canvas.line(p0.x, p0.y, p1.x, p1.y); // bottom
            }
        }

        step(bar, pulse);
    }
}
