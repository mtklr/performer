#include "OverviewPage.h"

#include "Pages.h"

#include "model/NoteTrack.h"

#include "ui/painters/WindowPainter.h"

static int drawNoteTrack(Canvas &canvas, int trackIndex, const NoteTrackEngine &trackEngine, const NoteSequence &sequence) {
    canvas.setBlendMode(BlendMode::Set);

    int stepOffset = (std::max(0, trackEngine.currentStep()) / 16) * 16;
    int y = trackIndex * 8;

    for (int i = 0; i < 16; ++i) {
        int stepIndex = stepOffset + i;
        const auto &step = sequence.step(stepIndex);

        int x = 64 + i * 8;

        if (trackEngine.currentStep() == stepIndex) {
            canvas.setColor(step.gate() ? 0xf : 0x7);
            canvas.drawRect(x + 1, y + 1, 6, 6);
            if (step.gate()) {
                canvas.fillRect(x + 3, y + 3, 2, 2);
            }
        } else {
            canvas.setColor(step.gate() ? 0x7 : 0x3);
            canvas.drawRect(x + 1, y + 1, 6, 6);
            // if (step.gate()) {
            //     canvas.fillRect(x + 3, y + 3, 2, 2);
            // }
        }

        // if (trackEngine.currentStep() == stepIndex) {
        //     canvas.setColor(0xf);
        //     canvas.drawRect(x + 1, y + 1, 6, 6);
        // }
    }

    return trackEngine.currentStep();
}

static void drawCurve(Canvas &canvas, int x, int y, int w, int h, float &lastY, const Curve::Function function, float min, float max) {
    const int Step = 1;

    auto eval = [=] (float x) {
        return (1.f - (function(x) * (max - min) + min)) * h;
    };

    float fy0 = y + eval(0.f);

    if (lastY >= 0.f && lastY != fy0) {
        canvas.line(x, lastY, x, fy0);
    }

    for (int i = 0; i < w; i += Step) {
        float fy1 = y + eval((float(i) + Step) / w);
        canvas.line(x + i, fy0, x + i + Step, fy1);
        fy0 = fy1;
    }

    lastY = fy0;
}

static int drawCurveTrack(Canvas &canvas, int trackIndex, const CurveTrackEngine &trackEngine, const CurveSequence &sequence) {
    canvas.setBlendMode(BlendMode::Add);
    canvas.setColor(0xa);

    int stepOffset = (std::max(0, trackEngine.currentStep()) / 16) * 16;
    int y = trackIndex * 8;

    float lastY = -1.f;

    for (int i = 0; i < 16; ++i) {
        int stepIndex = stepOffset + i;
        const auto &step = sequence.step(stepIndex);
        float min = step.minNormalized();
        float max = step.maxNormalized();
        const auto function = Curve::function(Curve::Type(std::min(Curve::Last - 1, step.shape())));

        int x = 64 + i * 8;

        drawCurve(canvas, x, y + 1, 8, 6, lastY, function, min, max);
    }

    if (trackEngine.currentStep() >= 0) {
        int x = 64 + ((trackEngine.currentStep() - stepOffset) + trackEngine.currentStepFraction()) * 8;
        canvas.setBlendMode(BlendMode::Set);
        canvas.setColor(0xf);
        canvas.vline(x, y + 1, 7);
    }

    return trackEngine.currentStep();
}


OverviewPage::OverviewPage(PageManager &manager, PageContext &context) :
    BasePage(manager, context)
{}

void OverviewPage::enter() {
}

void OverviewPage::exit() {
}

void OverviewPage::draw(Canvas &canvas) {
    int currentStep = 0;
    int stepOffset = 0;
    int seqPage = 0;

    WindowPainter::clear(canvas);

    if (!_drawOverview) return;

    canvas.setFont(Font::Tiny);
    canvas.setBlendMode(BlendMode::Set);
    canvas.setColor(0x7);

    canvas.vline(64 - 3, 0, 64);
    canvas.vline(64 - 2, 0, 64);
    canvas.vline(192 + 1, 0, 64);
    canvas.vline(192 + 2, 0, 64);

    for (int trackIndex = 0; trackIndex < 8; trackIndex++) {
        const auto &track = _project.track(trackIndex);
        const auto &trackState = _project.playState().trackState(trackIndex);
        const auto &trackEngine = _engine.trackEngine(trackIndex);

        canvas.setBlendMode(BlendMode::Set);
        canvas.setColor(0x7);

        int y = 5 + trackIndex * 8;

        // track number / pattern number
        canvas.setColor(trackState.mute() ? 0x7 : 0xf);
        canvas.drawText(2, y, FixedStringBuilder<8>("T%d", trackIndex + 1));
        canvas.drawText(18, y, FixedStringBuilder<8>("P%d", trackState.pattern() + 1));

        // gate output
        bool gate = _engine.gateOutput() & (1 << trackIndex);
        canvas.setColor(gate ? 0xf : 0x7);
        canvas.drawRect(256 - 48 + 1, trackIndex * 8 + 1, 6, 6);
        if (gate) {
            canvas.fillRect(256 - 48 + 3, trackIndex * 8 + 3, 2, 2);
        }

        // cv output
        canvas.setColor(0xf);
        canvas.drawText(256 - 32, y, FixedStringBuilder<8>("%.2fV", _engine.cvOutput().channel(trackIndex)));

        switch (track.trackMode()) {
        case Track::TrackMode::Note:
            currentStep = drawNoteTrack(canvas, trackIndex, trackEngine.as<NoteTrackEngine>(), track.noteTrack().sequence(trackState.pattern()));
            stepOffset = (std::max(0, currentStep) / 16) * 16;
            seqPage = 4 * stepOffset / 64;

            canvas.setColor(0xf);
            if (trackState.mute()) {
                canvas.setColor(0x7);
            }

            if (_drawPageStepCount) {
                canvas.drawText(38, y, FixedStringBuilder<8>("%d", seqPage + 1));
                canvas.drawText(46, y, FixedStringBuilder<8>("%d", currentStep < 0 ? 1 : currentStep + 1));
            }
            break;
        case Track::TrackMode::Curve:
            currentStep = drawCurveTrack(canvas, trackIndex, trackEngine.as<CurveTrackEngine>(), track.curveTrack().sequence(trackState.pattern()));
            stepOffset = (std::max(0, currentStep) / 16) * 16;
            seqPage = 4 * stepOffset / 64;
            canvas.setColor(0xf);
            if (trackState.mute()) {
                canvas.setColor(0x7);
            }

            if (_drawPageStepCount) {
                canvas.drawText(38, y, FixedStringBuilder<8>("%d", seqPage + 1));
                canvas.drawText(46, y, FixedStringBuilder<8>("%d", currentStep < 0 ? 1 : currentStep + 1));
            }
            break;
        case Track::TrackMode::MidiCv:
            break;
        case Track::TrackMode::Last:
            break;
        }
    }
}

void OverviewPage::updateLeds(Leds &leds) {
}

void OverviewPage::keyDown(KeyEvent &event) {
    const auto &key = event.key();

    if (key.isGlobal()) {
        return;
    }

    // event.consume();
}

void OverviewPage::keyUp(KeyEvent &event) {
    const auto &key = event.key();

    if (key.isGlobal()) {
        return;
    }

    // event.consume();
}

void OverviewPage::keyPress(KeyPressEvent &event) {
    const auto &key = event.key();

#ifdef CONFIG_ENABLE_INTRO
    if (key.is(Key::F0)) {
        if (key.shiftModifier()) {
            _manager.pages().intro.show();
        }
        event.consume();
    }
#endif

#ifdef CONFIG_ENABLE_STARS
    if (key.is(Key::F1)) {
        if (key.shiftModifier()) {
            _manager.pages().stars.show();
        }
        event.consume();
    }
#endif

#ifdef CONFIG_ENABLE_FIRE
    if (key.is(Key::F2)) {
        if (key.shiftModifier()) {
            _manager.pages().fire.show();
        }
        event.consume();
    }
#endif

#ifdef CONFIG_ENABLE_LIFE
    if (key.is(Key::F3)) {
        if (key.shiftModifier()) {
            _manager.pages().life.show();
        }
        event.consume();
    }
#endif

    if (key.is(Key::F4)) {
        if (key.shiftModifier()) {
            _drawOverview = !_drawOverview;
        } else {
            _drawPageStepCount = !_drawPageStepCount;
        }
        event.consume();
    }

    if (key.isGlobal()) {
        return;
    }

    // event.consume();
}

void OverviewPage::encoder(EncoderEvent &event) {
    // event.consume();
}
