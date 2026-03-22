#pragma once

#include "BasePage.h"

#include "life/Life.h"

class LifePage : public BasePage {
public:
    LifePage(PageManager &manager, PageContext &context);

    virtual void enter() override;
    virtual void exit() override;

    virtual void draw(Canvas &canvas) override;

    virtual bool isModal() const override { return true; }

    virtual void keyDown(KeyEvent &event) override;
    virtual void keyUp(KeyEvent &event) override;
    virtual void keyPress(KeyPressEvent &event) override;
    virtual void encoder(EncoderEvent &event) override;
private:
    Life _life;

    uint32_t _lastTicks;
};
