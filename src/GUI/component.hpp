#pragma once
#include "../utils.hpp"
#include "ESP32_SPI_9341.h"
class Component {
   public:
    Rectangle bounds;
    Component(Rectangle& rect) {
        this->bounds = rect;
    }
    void draw(LGFX lcd);
    bool checkTouching(LGFX lcd) {
        int pos[2] = {0, 0};
        if (lcd.getTouch(&pos[0], &pos[1])) {
            auto didTouch = this->bounds.checkInside({pos[0], pos[1]});
            // Serial.printf("touch: %d %d touched? %s", pos[0], pos[1], didTouch ? "YES" : "NO");
            return didTouch;
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
};