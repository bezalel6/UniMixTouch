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
    m_lcd.setRotation(0);
    performTouchCalibration();
}

void GuiManager::update() {
    drawComponents();
    handleComponentTouch();
}

void GuiManager::clear() {
    m_lcd.clear();
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
    m_lcd.calibrateTouch(nullptr, fg, bg, std::max(m_lcd.width(), m_lcd.height()) >> 3);

    m_lcd.clear();
    m_lcd.setTextSize(m_textSize);
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
}

int GuiManager::getWidth() const {
    return m_lcd.width();
}

int GuiManager::getHeight() const {
    return m_lcd.height();
}

void GuiManager::drawComponents() {
    for (auto* component : m_components) {
        if (component != nullptr) {
            component->draw(m_lcd);
        }
    }
}

bool GuiManager::handleComponentTouch() {
    bool touchHandled = false;
    for (auto* component : m_components) {
        if (component != nullptr && component->checkTouching(m_lcd)) {
            component->clicked();
            touchHandled = true;
            break;  // Handle only the first touched component
        }
    }
    return touchHandled;
}