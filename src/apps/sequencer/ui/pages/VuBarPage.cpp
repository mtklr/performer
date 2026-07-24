#include "VuBarPage.h"

VuBarPage::VuBarPage(PageManager &manager, PageContext &context) :
    BasePage(manager, context)
{
    _lastTicks = os::ticks();
}

void VuBarPage::enter() {
    _vuBar.init();
}

void VuBarPage::exit() {
}

void VuBarPage::draw(Canvas &canvas) {
    uint32_t currentTicks = os::ticks();
    float dt = float(currentTicks - _lastTicks) / os::time::ms(1000);
    _lastTicks = currentTicks;

    _vuBar.update(dt, _engine.gateOutput());
    _vuBar.draw(canvas);
}

void VuBarPage::keyDown(KeyEvent &event) {
    const auto &key = event.key();

    if (key.isGlobal()) {
        return;
    }

    // event.consume();
}

void VuBarPage::keyUp(KeyEvent &event) {
    const auto &key = event.key();

    if (key.isGlobal()) {
        return;
    }

    // event.consume();
}

void VuBarPage::keyPress(KeyPressEvent &event) {
    const auto &key = event.key();

    if (key.pageModifier()) {
        BasePage::close();
        return;
    }

    if (key.is(Key::F1)) {
        if (!key.shiftModifier() && !key.pageModifier()) {
            BasePage::close();
            return;
        }
    }
}

void VuBarPage::encoder(EncoderEvent &event) {
    return;
}
