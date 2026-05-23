#pragma once

#include "BasePage.h"

#include "plasma/Plasma.h"

class PlasmaPage : public BasePage {
public:
    PlasmaPage(PageManager &manager, PageContext &context);

    virtual void enter() override;
    virtual void exit() override;

    virtual void draw(Canvas &canvas) override;

    virtual bool isModal() const override { return true; }

    virtual void keyDown(KeyEvent &event) override;
    virtual void keyUp(KeyEvent &event) override;
    virtual void keyPress(KeyPressEvent &event) override;
    virtual void encoder(EncoderEvent &event) override;
private:
    Plasma _plasma;

    uint32_t _lastTicks;
};
