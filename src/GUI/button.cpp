#include "button.hpp"

void Button::draw(LGFX lcd) {
    // Draw button background
    lcd.setColor(TFT_LIGHTGRAY);
    Rectangle bounds = this->bounds;
    lcd.fillRect(bounds.origin.x, bounds.origin.y, bounds.w, bounds.h);

    // Calculate text dimensions
    int16_t textWidth = lcd.textWidth(this->text);
    int16_t textHeight = lcd.fontHeight();

    // Calculate centered position
    Point middle = bounds.getMiddle();
    int16_t textX = middle.x - (textWidth / 2);
    int16_t textY = middle.y - (textHeight / 2);

    // Draw centered text
    lcd.setCursor(textX, textY);
    lcd.setColor(TFT_BLACK);
    lcd.print(this->text);
}
