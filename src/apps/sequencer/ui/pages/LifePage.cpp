#include "LifePage.h"

#include "os/os.h"

LifePage::LifePage(PageManager &manager, PageContext &context) :
    BasePage(manager, context)
{
    _lastTicks = os::ticks();
}

void LifePage::enter() {
    _life.init();
}

void LifePage::exit() {
}

void LifePage::draw(Canvas &canvas) {
    uint32_t currentTicks = os::ticks();
    float dt = float(currentTicks - _lastTicks) / os::time::ms(1000);
    _lastTicks = currentTicks;

    _life.update(dt);
    _life.draw(canvas);
}

void LifePage::keyDown(KeyEvent &event) {
    const auto &key = event.key();

    if (key.isGlobal()) {
        return;
    }

    // event.consume();
}

void LifePage::keyUp(KeyEvent &event) {
    const auto &key = event.key();

    if (key.isGlobal()) {
        return;
    }

    // event.consume();
}

void LifePage::keyPress(KeyPressEvent &event) {
    const auto &key = event.key();

    if (key.pageModifier()) {
        BasePage::close();
        return;
    }

    if (key.is(Key::F3)) {
        if (key.shiftModifier()) {
            BasePage::close();
            return;
        }
    }
}

void LifePage::encoder(EncoderEvent &event) {
    return;
}
