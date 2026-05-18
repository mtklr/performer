#include "VuLinePage.h"

VuLinePage::VuLinePage(PageManager &manager, PageContext &context) :
    BasePage(manager, context)
{
    _lastTicks = os::ticks();
}

void VuLinePage::enter() {
    _vuLine.init();
}

void VuLinePage::exit() {
}

void VuLinePage::draw(Canvas &canvas) {
    uint32_t currentTicks = os::ticks();
    float dt = float(currentTicks - _lastTicks) / os::time::ms(1000);
    _lastTicks = currentTicks;

    _vuLine.update(dt, _engine.gateOutput());
    _vuLine.draw(canvas);
}

void VuLinePage::keyDown(KeyEvent &event) {
    const auto &key = event.key();

    if (key.isGlobal()) {
        return;
    }

    // event.consume();
}

void VuLinePage::keyUp(KeyEvent &event) {
    const auto &key = event.key();

    if (key.isGlobal()) {
        return;
    }

    // event.consume();
}

void VuLinePage::keyPress(KeyPressEvent &event) {
    const auto &key = event.key();

    if (key.pageModifier()) {
        BasePage::close();
        return;
    }

    if (key.is(Key::F2)) {
        if (!key.shiftModifier() && !globalKeyState()[Key::Page]) {
            BasePage::close();
            return;
        }
    }
}

void VuLinePage::encoder(EncoderEvent &event) {
    return;
}
