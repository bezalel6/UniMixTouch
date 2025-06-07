#include "Arduino.h"
#include "component.hpp"

class Button : public Component {
   public:
    Button(Rectangle rect, String text) : Component(rect) {
        this->text = text;
        this->onClick = Button::func;
    }
    static void func() {
        Serial.println("IM HERE");
    };
    void draw(LGFX lcd);

   private:
    String text;
};