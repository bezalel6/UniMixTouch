#pragma once

#include <vector>
#include "ESP32_SPI_9341.h"
#include "component.hpp"
#include "button.hpp"

#define DEFAULT_TEXT_SIZE 3

class GuiManager {
   public:
    GuiManager(LGFX& lcd);
    ~GuiManager();

    // Core GUI management functions
    void init();
    void update();
    void clear();

    // Component management
    void addComponent(Component* component);
    void removeComponent(Component* component);
    void clearComponents();

    // Helper methods to create common components
    Button* createButton(int x, int y, int width, int height, const String& text);
    Button* createButton(Rectangle bounds, const String& text);

    // Touch and calibration
    void performTouchCalibration();
    bool processTouchEvents();

    // Display functions
    void setTextSize(int size);
    void setTextColor(uint16_t color);
    void setCursor(int x, int y);
    void print(const String& text);
    void println(const String& text);
    void fillScreen(uint16_t color);

    // Getters
    int getWidth() const;
    int getHeight() const;

   private:
    LGFX& m_lcd;
    std::vector<Component*> m_components;
    int m_textSize;
    uint16_t m_textColor;

    // Helper functions
    void drawComponents();
    bool handleComponentTouch();
};