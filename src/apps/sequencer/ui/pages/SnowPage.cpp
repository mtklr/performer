#include "SnowPage.h"

#include "os/os.h"

SnowPage::SnowPage(PageManager &manager, PageContext &context) :
    BasePage(manager, context)
{
    _lastTicks = os::ticks();
}

void SnowPage::enter() {
    _snow.init();
}

void SnowPage::exit() {
}

void SnowPage::draw(Canvas &canvas) {
    uint32_t currentTicks = os::ticks();
    float dt = float(currentTicks - _lastTicks) / os::time::ms(1000);
    _lastTicks = currentTicks;

    _snow.update(dt);
    _snow.draw(canvas);
}

void SnowPage::keyDown(KeyEvent &event) {
    const auto &key = event.key();

    if (key.isGlobal()) {
        return;
    }

    // event.consume();
}

void SnowPage::keyUp(KeyEvent &event) {
    const auto &key = event.key();

    if (key.isGlobal()) {
        return;
    }

    // event.consume();
}

void SnowPage::keyPress(KeyPressEvent &event) {
    const auto &key = event.key();

    if (key.pageModifier()) {
        BasePage::close();
        return;
    }

    if (key.is(Key::F2)) {
        if (!key.shiftModifier() && key.pageModifier()) {
            BasePage::close();
            return;
        }
    }
}

void SnowPage::encoder(EncoderEvent &event) {
    return;
}
