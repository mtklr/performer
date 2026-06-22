#include "WormsPage.h"

#include "os/os.h"

WormsPage::WormsPage(PageManager &manager, PageContext &context) :
    BasePage(manager, context)
{
    _lastTicks = os::ticks();
}

void WormsPage::enter() {
    _worms.init();
}

void WormsPage::exit() {
}

void WormsPage::draw(Canvas &canvas) {
    uint32_t currentTicks = os::ticks();
    float dt = float(currentTicks - _lastTicks) / os::time::ms(1000);
    _lastTicks = currentTicks;

    _worms.update(dt, _engine.gateOutput());
    _worms.draw(canvas);
}

void WormsPage::keyDown(KeyEvent &event) {
    const auto &key = event.key();

    if (key.isGlobal()) {
        return;
    }

    // event.consume();
}

void WormsPage::keyUp(KeyEvent &event) {
    const auto &key = event.key();

    if (key.isGlobal()) {
        return;
    }

    // event.consume();
}

void WormsPage::keyPress(KeyPressEvent &event) {
    const auto &key = event.key();

    if (key.pageModifier()) {
        BasePage::close();
        return;
    }

    if (key.is(Key::F1)) {
    if (!key.shiftModifier() && globalKeyState()[Key::Page]) {
            BasePage::close();
            return;
        }
    }
}

void WormsPage::encoder(EncoderEvent &event) {
    return;
}
