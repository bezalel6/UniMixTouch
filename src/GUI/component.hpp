#pragma once
#include "../utils.hpp"
#include "ESP32_SPI_9341.h"
#include "layout.hpp"

class Component {
   public:
    Rectangle bounds;
    bool needsRedraw;
    LayoutProps layoutProps;

    // Parent panel reference for layout calculations
    class Panel* parentPanel = nullptr;

    Component(Rectangle& rect) {
        this->bounds = rect;
        this->needsRedraw = true;  // Initially needs to be drawn
    }

    // Constructor for layout-managed components (no absolute bounds)
    Component() {
        this->bounds = Rectangle(0, 0, 0, 0);
        this->needsRedraw = true;
    }
    virtual void draw(LGFX lcd) = 0;

    // Virtual method to check component type (replaces dynamic_cast)
    virtual bool isPanel() const { return false; }

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