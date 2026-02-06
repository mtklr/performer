#include "StarsPage.h"

#include "os/os.h"

StarsPage::StarsPage(PageManager &manager, PageContext &context) :
    BasePage(manager, context)
{
    _lastTicks = os::ticks();
}

void StarsPage::enter() {
    _stars.init();
}

void StarsPage::exit() {
}

void StarsPage::draw(Canvas &canvas) {
    uint32_t currentTicks = os::ticks();
    float dt = float(currentTicks - _lastTicks) / os::time::ms(1000);
    _lastTicks = currentTicks;

    _stars.update(dt);
    _stars.draw(canvas);
}

void StarsPage::keyDown(KeyEvent &event) {
    const auto &key = event.key();

    if (key.isGlobal()) {
        return;
    }

    // event.consume();
}

void StarsPage::keyUp(KeyEvent &event) {
    const auto &key = event.key();

    if (key.isGlobal()) {
        return;
    }

    // event.consume();
}

void StarsPage::keyPress(KeyPressEvent &event) {
    const auto &key = event.key();

    if (key.pageModifier()) {
        BasePage::close();
        return;
    }

    if (key.is(Key::F1)) {
        if (key.shiftModifier()) {
            BasePage::close();
            return;
        }
    }
}

void StarsPage::encoder(EncoderEvent &event) {
    return;
}
