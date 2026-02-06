#pragma once

#include "BasePage.h"

#include "stars/Stars.h"

class StarsPage : public BasePage {
public:
    StarsPage(PageManager &manager, PageContext &context);

    virtual void enter() override;
    virtual void exit() override;

    virtual void draw(Canvas &canvas) override;

    virtual bool isModal() const override { return true; }

    virtual void keyDown(KeyEvent &event) override;
    virtual void keyUp(KeyEvent &event) override;
    virtual void keyPress(KeyPressEvent &event) override;
    virtual void encoder(EncoderEvent &event) override;
private:
    Stars _stars;

    uint32_t _lastTicks;
};
