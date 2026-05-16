#include "IntroPage.h"

#include "os/os.h"

IntroPage::IntroPage(PageManager &manager, PageContext &context) :
    BasePage(manager, context)
{
    _lastTicks = os::ticks();
}

void IntroPage::enter() {
}

void IntroPage::exit() {
}

void IntroPage::draw(Canvas &canvas) {
    uint32_t currentTicks = os::ticks();
    float dt = float(currentTicks - _lastTicks) / os::time::ms(1000);
    _lastTicks = currentTicks;

    _intro.update(dt, _engine.gateOutput());
    _intro.draw(canvas);
}

void IntroPage::keyDown(KeyEvent &event) {
    const auto &key = event.key();

    if (key.isGlobal()) {
        return;
    }

    // event.consume();
}

void IntroPage::keyUp(KeyEvent &event) {
    const auto &key = event.key();

    if (key.isGlobal()) {
        return;
    }

    // event.consume();
}

void IntroPage::keyPress(KeyPressEvent &event) {
    const auto &key = event.key();

    if (key.pageModifier()) {
        BasePage::close();
        return;
    }

    if (key.is(Key::F0)) {
        if (key.shiftModifier()) {
            BasePage::close();
            return;
        }
    }
}

void IntroPage::encoder(EncoderEvent &event) {
    return;
}
