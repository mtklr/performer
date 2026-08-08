#pragma once

#include "core/gfx/Canvas.h"

class SequencePainter {
public:
    static void drawLoopStart(Canvas &canvas, int x, int y, int w);
    static void drawLoopEnd(Canvas &canvas, int x, int y, int w);

    static void drawOffset(Canvas &canvas, int x, int y, int w, int h, int offset, int minOffset, int maxOffset, bool stepGate);
    static void drawRetrigger(Canvas &canvas, int x, int y, int w, int h, int retrigger, int maxRetrigger, bool stepGate);
    static void drawProbability(Canvas &canvas, int x, int y, int w, int h, int probability, int maxProbability, bool stepGate);
    static void drawLength(Canvas &canvas, int x, int y, int w, int h, int length, int maxLength, bool stepGate);
    static void drawLengthRange(Canvas &canvas, int x, int y, int w, int h, int length, int range, int maxLength, bool stepGate);
    static void drawSlide(Canvas &canvas, int x, int y, int w, int h, bool active, bool stepGate);

    static void drawSequenceProgress(Canvas &canvas, int x, int y, int w, int h, float progress);
};
