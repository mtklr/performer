#include "BouncePage.h"

#include "os/os.h"

BouncePage::BouncePage(PageManager &manager, PageContext &context) :
    BasePage(manager, context)
{
    _lastTicks = os::ticks();
}

void BouncePage::enter() {
    _bounce.init();
}

void BouncePage::exit() {
}

void BouncePage::draw(Canvas &canvas) {
    uint32_t currentTicks = os::ticks();
    float dt = float(currentTicks - _lastTicks) / os::time::ms(1000);
    _lastTicks = currentTicks;

    _bounce.update(dt);
    _bounce.draw(canvas);
}

void BouncePage::keyDown(KeyEvent &event) {
    const auto &key = event.key();

    if (key.isGlobal()) {
        return;
    }

    // event.consume();
}

void BouncePage::keyUp(KeyEvent &event) {
    const auto &key = event.key();

    if (key.isGlobal()) {
        return;
    }

    // event.consume();
}

void BouncePage::keyPress(KeyPressEvent &event) {
    const auto &key = event.key();

    if (key.pageModifier()) {
        BasePage::close();
        return;
    }

    if (key.is(Key::F4)) {
        if (!key.shiftModifier() && globalKeyState()[Key::Page]) {
            BasePage::close();
            return;
        }
    }
}

void BouncePage::encoder(EncoderEvent &event) {
    return;
}
