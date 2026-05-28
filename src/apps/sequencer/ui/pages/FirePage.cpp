#include "FirePage.h"

#include "os/os.h"

FirePage::FirePage(PageManager &manager, PageContext &context) :
    BasePage(manager, context)
{
    _lastTicks = os::ticks();
}

void FirePage::enter() {
    _fire.init();
}

void FirePage::exit() {
}

void FirePage::draw(Canvas &canvas) {
    uint32_t currentTicks = os::ticks();
    float dt = float(currentTicks - _lastTicks) / os::time::ms(1000);
    _lastTicks = currentTicks;

    _fire.update(dt);
    _fire.draw(canvas);
}

void FirePage::keyDown(KeyEvent &event) {
    const auto &key = event.key();

    if (key.isGlobal()) {
        return;
    }

    // event.consume();
}

void FirePage::keyUp(KeyEvent &event) {
    const auto &key = event.key();

    if (key.isGlobal()) {
        return;
    }

    // event.consume();
}

void FirePage::keyPress(KeyPressEvent &event) {
    const auto &key = event.key();

    if (key.pageModifier()) {
        BasePage::close();
        return;
    }

    if (key.is(Key::F2)) {
        if (key.shiftModifier()) {
            BasePage::close();
            return;
        }
    }
}

void FirePage::encoder(EncoderEvent &event) {
    return;
}
