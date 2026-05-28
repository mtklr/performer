#include "PlasmaPage.h"

#include "os/os.h"

PlasmaPage::PlasmaPage(PageManager &manager, PageContext &context) :
    BasePage(manager, context)
{
    _lastTicks = os::ticks();
}

void PlasmaPage::enter() {
    _plasma.init();
}

void PlasmaPage::exit() {
}

void PlasmaPage::draw(Canvas &canvas) {
    uint32_t currentTicks = os::ticks();
    float dt = float(currentTicks - _lastTicks) / os::time::ms(1000);
    _lastTicks = currentTicks;

    _plasma.update(dt);
    _plasma.draw(canvas);
}

void PlasmaPage::keyDown(KeyEvent &event) {
    const auto &key = event.key();

    if (key.isGlobal()) {
        return;
    }

    // event.consume();
}

void PlasmaPage::keyUp(KeyEvent &event) {
    const auto &key = event.key();

    if (key.isGlobal()) {
        return;
    }

    // event.consume();
}

void PlasmaPage::keyPress(KeyPressEvent &event) {
    const auto &key = event.key();

    if (key.pageModifier()) {
        BasePage::close();
        return;
    }

    if (key.is(Key::F4)) {
        if (key.shiftModifier()) {
            BasePage::close();
            return;
        }
    }
}

void PlasmaPage::encoder(EncoderEvent &event) {
    return;
}
