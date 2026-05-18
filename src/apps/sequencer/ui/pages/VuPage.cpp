#include "VuPage.h"

VuPage::VuPage(PageManager &manager, PageContext &context) :
    BasePage(manager, context)
{
    _lastTicks = os::ticks();
}

void VuPage::enter() {
    _vu.init();
}

void VuPage::exit() {
}

void VuPage::draw(Canvas &canvas) {
    uint32_t currentTicks = os::ticks();
    float dt = float(currentTicks - _lastTicks) / os::time::ms(1000);
    _lastTicks = currentTicks;

    _vu.update(dt, _engine.gateOutput());
    _vu.draw(canvas);
}

void VuPage::keyDown(KeyEvent &event) {
    const auto &key = event.key();

    if (key.isGlobal()) {
        return;
    }

    // event.consume();
}

void VuPage::keyUp(KeyEvent &event) {
    const auto &key = event.key();

    if (key.isGlobal()) {
        return;
    }

    // event.consume();
}

void VuPage::keyPress(KeyPressEvent &event) {
    const auto &key = event.key();

    if (key.pageModifier()) {
        BasePage::close();
        return;
    }

    if (key.is(Key::F0)) {
        if (!key.shiftModifier() && !globalKeyState()[Key::Page]) {
            BasePage::close();
            return;
        }
    }
}

void VuPage::encoder(EncoderEvent &event) {
    return;
}
