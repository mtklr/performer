#include "FishPage.h"

#include "os/os.h"

FishPage::FishPage(PageManager &manager, PageContext &context) :
    BasePage(manager, context)
{
    _lastTicks = os::ticks();
}

void FishPage::enter() {
    _fish.init();
}

void FishPage::exit() {
}

void FishPage::draw(Canvas &canvas) {
    uint32_t currentTicks = os::ticks();
    float dt = float(currentTicks - _lastTicks) / os::time::ms(1000);
    _lastTicks = currentTicks;

    _fish.update(dt);
    _fish.draw(canvas);
}

void FishPage::keyDown(KeyEvent &event) {
    const auto &key = event.key();

    if (key.isGlobal()) {
        return;
    }

    // event.consume();
}

void FishPage::keyUp(KeyEvent &event) {
    const auto &key = event.key();

    if (key.isGlobal()) {
        return;
    }

    // event.consume();
}

void FishPage::keyPress(KeyPressEvent &event) {
    const auto &key = event.key();

    if (key.pageModifier()) {
        BasePage::close();
        return;
    }

    if (key.is(Key::F3)) {
        if (!key.shiftModifier() && globalKeyState()[Key::Page]) {
            BasePage::close();
            return;
        }
    }
}

void FishPage::encoder(EncoderEvent &event) {
    return;
}
