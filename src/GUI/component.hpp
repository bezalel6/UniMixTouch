#pragma once
#include "../utils.hpp"
#include "ESP32_SPI_9341.h"
class Component {
   public:
    Rectangle bounds;
    bool needsRedraw;

    Component(Rectangle& rect) {
        this->bounds = rect;
        this->needsRedraw = true;  // Initially needs to be drawn
    }
    virtual void draw(LGFX lcd) = 0;

    void markDirty() {
        needsRedraw = true;
    }

    void markClean() {
        needsRedraw = false;
    }

    bool checkTouching(LGFX lcd) {
        int pos[2] = {0, 0};
        if (lcd.getTouch(&pos[0], &pos[1])) {
            if (this->isDebouncing) {
                return false;
            }
            auto didTouch = this->bounds.checkInside({pos[0], pos[1]});
            // Serial.printf("touch: %d %d touched? %s", pos[0], pos[1], didTouch ? "YES" : "NO");
            this->isDebouncing = didTouch;
            return didTouch;
        } else {
            this->isDebouncing = false;
        }

        return false;
    }
    void clicked() {
        // if (this->onClick) {
        this->onClick();
        // }
    };

    //    private:
    f_void onClick;

   private:
    bool isDebouncing = false;
};