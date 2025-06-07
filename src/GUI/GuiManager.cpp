#include "GuiManager.hpp"
#include <algorithm>

GuiManager::GuiManager(LGFX& lcd)
    : m_lcd(lcd), m_textSize(DEFAULT_TEXT_SIZE), m_textColor(TFT_WHITE) {
}

GuiManager::~GuiManager() {
    clearComponents();
}

void GuiManager::init() {
    m_lcd.init();
    m_lcd.setTextSize(m_textSize);
    m_lcd.setRotation(3);

    // Check if we have saved calibration data
    if (hasSavedCalibration()) {
        // Load and apply saved calibration
        std::uint16_t calData[8];  // Standard calibration data array size
        if (loadTouchCalibration(calData)) {
            m_lcd.setTouchCalibrate(calData);
            Serial.println("Touch calibration loaded from storage");
        } else {
            Serial.println("Failed to load touch calibration, performing new calibration");
            performTouchCalibration();
        }
    } else {
        // No saved calibration found, perform calibration
        Serial.println("No saved touch calibration found, performing calibration");
        performTouchCalibration();
    }
}

void GuiManager::update() {
    drawComponents();
    handleComponentTouch();
}

void GuiManager::clear() {
    m_lcd.clear();
    markAllComponentsDirty();  // Components need to be redrawn after clearing
}

void GuiManager::addComponent(Component* component) {
    if (component != nullptr) {
        m_components.push_back(component);
    }
}

void GuiManager::removeComponent(Component* component) {
    auto it = std::find(m_components.begin(), m_components.end(), component);
    if (it != m_components.end()) {
        m_components.erase(it);
    }
}

void GuiManager::clearComponents() {
    for (auto* component : m_components) {
        delete component;
    }
    m_components.clear();
}

Button* GuiManager::createButton(int x, int y, int width, int height, const String& text) {
    Rectangle bounds(x, y, width, height);
    return createButton(bounds, text);
}

Button* GuiManager::createButton(Rectangle bounds, const String& text) {
    Button* button = new Button(bounds, text);
    addComponent(button);
    return button;
}

void GuiManager::performTouchCalibration() {
    m_lcd.fillScreen(TFT_YELLOW);

    m_lcd.setTextColor(TFT_BLACK);
    m_lcd.setTextSize(4);
    m_lcd.setCursor(70, 110);
    m_lcd.println("SCREEN");
    m_lcd.setCursor(70, 150);
    m_lcd.println("CALIBRATION");

    // Touch calibration for the four corners of the screen
    std::uint16_t fg = TFT_WHITE;
    std::uint16_t bg = TFT_BLACK;
    if (m_lcd.isEPD()) {
        std::swap(fg, bg);
    }

    std::uint16_t calData[8];  // Array to store calibration data
    m_lcd.calibrateTouch(calData, fg, bg, std::max(m_lcd.width(), m_lcd.height()) >> 3);

    // Save the calibration data to preferences
    if (saveTouchCalibration(calData)) {
        Serial.println("Touch calibration saved to storage");
    } else {
        Serial.println("Failed to save touch calibration");
    }

    m_lcd.clear();
    m_lcd.setTextSize(m_textSize);
}

bool GuiManager::saveTouchCalibration(std::uint16_t* calData) {
    if (calData == nullptr) {
        Serial.println("Error: Invalid calibration data");
        return false;
    }

    m_preferences.begin("touch_cal", false);

    // Save calibration data as bytes
    size_t bytesWritten = m_preferences.putBytes("cal_data", calData, 8 * sizeof(std::uint16_t));
    bool success = (bytesWritten == 8 * sizeof(std::uint16_t));

    if (success) {
        // Set a flag to indicate we have valid calibration data
        m_preferences.putBool("cal_valid", true);

        // Log the calibration values for debugging
        Serial.print("Saved calibration data: ");
        for (int i = 0; i < 8; i++) {
            Serial.print(calData[i]);
            if (i < 7) Serial.print(", ");
        }
        Serial.println();
    } else {
        Serial.println("Error: Failed to save calibration data to preferences");
    }

    m_preferences.end();
    return success;
}

bool GuiManager::loadTouchCalibration(std::uint16_t* calData) {
    if (calData == nullptr) {
        Serial.println("Error: Invalid calibration data buffer");
        return false;
    }

    m_preferences.begin("touch_cal", true);  // Read-only mode

    // Check if we have valid calibration data
    bool hasValidData = m_preferences.getBool("cal_valid", false);
    if (!hasValidData) {
        m_preferences.end();
        return false;
    }

    // Load calibration data
    size_t bytesRead = m_preferences.getBytes("cal_data", calData, 8 * sizeof(std::uint16_t));
    bool success = (bytesRead == 8 * sizeof(std::uint16_t));

    if (success) {
        // Log the loaded calibration values for debugging
        Serial.print("Loaded calibration data: ");
        for (int i = 0; i < 8; i++) {
            Serial.print(calData[i]);
            if (i < 7) Serial.print(", ");
        }
        Serial.println();
    } else {
        Serial.println("Error: Failed to load calibration data from preferences");
    }

    m_preferences.end();
    return success;
}

bool GuiManager::hasSavedCalibration() {
    m_preferences.begin("touch_cal", true);  // Read-only mode
    bool hasValid = m_preferences.getBool("cal_valid", false);
    m_preferences.end();
    return hasValid;
}

void GuiManager::clearTouchCalibration() {
    m_preferences.begin("touch_cal", false);
    m_preferences.clear();
    m_preferences.end();
    Serial.println("Touch calibration data cleared");
}

bool GuiManager::processTouchEvents() {
    return handleComponentTouch();
}

void GuiManager::setTextSize(int size) {
    m_textSize = size;
    m_lcd.setTextSize(size);
}

void GuiManager::setTextColor(uint16_t color) {
    m_textColor = color;
    m_lcd.setTextColor(color);
}

void GuiManager::setCursor(int x, int y) {
    m_lcd.setCursor(x, y);
}

void GuiManager::print(const String& text) {
    m_lcd.print(text);
}

void GuiManager::println(const String& text) {
    m_lcd.println(text);
}

void GuiManager::fillScreen(uint16_t color) {
    m_lcd.fillScreen(color);
    markAllComponentsDirty();  // Components need to be redrawn after filling screen
}

int GuiManager::getWidth() const {
    return m_lcd.width();
}

int GuiManager::getHeight() const {
    return m_lcd.height();
}

void GuiManager::drawComponents() {
    for (auto* component : m_components) {
        if (component != nullptr && component->needsRedraw) {
            component->draw(m_lcd);
        }
    }
}

void GuiManager::markAllComponentsDirty() {
    for (auto* component : m_components) {
        if (component != nullptr) {
            component->markDirty();
        }
    }
}

bool GuiManager::handleComponentTouch() {
    bool touchHandled = false;
    for (auto* component : m_components) {
        if (component != nullptr && component->checkTouching(m_lcd)) {
            component->markDirty();  // Mark for redraw to show visual feedback
            component->clicked();
            touchHandled = true;
            break;  // Handle only the first touched component
        }
    }
    return touchHandled;
}