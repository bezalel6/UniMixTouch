#pragma once

#include <vector>
#include <Preferences.h>
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
    void markAllComponentsDirty();

    // Helper methods to create common components
    Button* createButton(int x, int y, int width, int height, const String& text);
    Button* createButton(Rectangle bounds, const String& text);

    // Touch and calibration
    void performTouchCalibration();
    bool processTouchEvents();

    // Touch calibration storage methods
    bool saveTouchCalibration(std::uint16_t* calData);
    bool loadTouchCalibration(std::uint16_t* calData);
    bool hasSavedCalibration();
    void clearTouchCalibration();

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
    Preferences m_preferences;

    // Helper functions
    void drawComponents();
    bool handleComponentTouch();
};