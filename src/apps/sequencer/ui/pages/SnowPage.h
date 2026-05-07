#pragma once

#include "BasePage.h"

#include "snow/Snow.h"

class SnowPage : public BasePage {
public:
    SnowPage(PageManager &manager, PageContext &context);

    virtual void enter() override;
    virtual void exit() override;

    virtual void draw(Canvas &canvas) override;

    virtual bool isModal() const override { return true; }

    virtual void keyDown(KeyEvent &event) override;
    virtual void keyUp(KeyEvent &event) override;
    virtual void keyPress(KeyPressEvent &event) override;
    virtual void encoder(EncoderEvent &event) override;
private:
    Snow _snow;

    uint32_t _lastTicks;
};
