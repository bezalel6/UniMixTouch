// /*
//  * GuiManager Usage Example
//  *
//  * This file demonstrates how to use the GuiManager class to create
//  * and manage GUI components in your ESP32 project.
//  */

// #include "GuiManager.hpp"
// #include "ESP32_SPI_9341.h"

// // Example usage in your main application:

// LGFX lcd;
// GuiManager guiManager(lcd);

// void exampleSetup() {
//     // Initialize the GUI Manager (this replaces lcd.init() and touch calibration)
//     guiManager.init();

//     // Method 1: Create buttons using helper method (recommended)
//     Button* homeButton = guiManager.createButton(10, 10, 100, 50, "Home");
//     Button* settingsButton = guiManager.createButton(10, 70, 100, 50, "Settings");
//     Button* exitButton = guiManager.createButton(10, 130, 100, 50, "Exit");

//     // Method 2: Create components manually and add them
//     Rectangle customBounds = {150, 10, 80, 40};
//     Button* customButton = new Button(customBounds, "Custom");
//     guiManager.addComponent(customButton);

//     // Set display properties
//     guiManager.setTextSize(2);
//     guiManager.setTextColor(TFT_WHITE);
//     guiManager.fillScreen(TFT_BLACK);
// }

// void exampleLoop() {
//     // Update GUI (handles touch events and draws all components)
//     guiManager.update();

//     // You can also check if any touch event was handled
//     bool touchHandled = guiManager.processTouchEvents();
//     if (touchHandled) {
//         // Do something when a component was touched
//     }

//     // Display text at specific positions
//     guiManager.setCursor(10, 200);
//     guiManager.println("Status: Ready");

//     delay(20);
// }

// void exampleCleanup() {
//     // Clear all components (automatically called in destructor)
//     guiManager.clearComponents();

//     // Clear screen
//     guiManager.clear();
// }

// /*
//  * Key Benefits of using GuiManager:
//  *
//  * 1. Simplified setup - init() handles LCD initialization and touch calibration
//  * 2. Automatic component management - no need to manually track components
//  * 3. Centralized touch handling - update() processes all touch events
//  * 4. Memory management - destructor automatically cleans up components
//  * 5. Helper methods - createButton() simplifies component creation
//  * 6. Consistent interface - all display operations go through GuiManager
//  *
//  * Migration from direct LCD usage:
//  * - Replace lcd.init() with guiManager.init()
//  * - Replace manual component loops with guiManager.update()
//  * - Replace lcd.println() with guiManager.println()
//  * - Use createButton() instead of manual Button creation
//  */