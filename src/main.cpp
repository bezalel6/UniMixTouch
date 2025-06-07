// 测试通过
#include <SPI.h>

#include <vector>

#include "ESP32_SPI_9341.h"
#include "GUI/GuiManager.hpp"

using namespace std;

#define SD_SCK 18
#define SD_MISO 19
#define SD_MOSI 23
#define SD_CS 5

#define DEFAULT_TEXT_SIZE 3

#define LIGHT_ADC 34

int led_pin[3] = {17, 4, 16};

LGFX lcd;
GuiManager guiManager(lcd);

// Global UI components for the app
ProgressBar* progressBar = nullptr;
Label* valueLabel = nullptr;
Button* plusButton = nullptr;
Button* minusButton = nullptr;

// App state
float currentProgress = 50.0f;  // Start at 50%

void onPlusButtonClick();
void onMinusButtonClick();
void updateProgressDisplay();
void led_set(int i);
void setup(void) {
    pinMode(led_pin[0], OUTPUT);
    pinMode(led_pin[1], OUTPUT);
    pinMode(led_pin[2], OUTPUT);

    Serial.begin(115200);

    // Initialize GUI Manager
    guiManager.init();

    // Clear screen with a clean background
    guiManager.fillScreen(TFT_BLACK);

    // === MINIMALISTIC PROGRESS BAR APP ===

    // Create main container panel
    Panel* mainPanel = guiManager.createFlexPanel(LayoutType::FLEX_COLUMN);
    mainPanel->bounds = Rectangle(0, 0, guiManager.getWidth(), guiManager.getHeight());
    mainPanel->setBackgroundColor(TFT_BLACK);
    mainPanel->setPadding(20);
    mainPanel->setGap(20);
    mainPanel->setAlignItems(Alignment::CENTER);

    // App title
    Label* titleLabel = guiManager.createLabel("Progress Control");
    titleLabel->layoutProps.heightMode = SizeMode::FIXED;
    titleLabel->layoutProps.height = 35;
    titleLabel->layoutProps.widthMode = SizeMode::FILL;
    titleLabel->setTextColor(TFT_CYAN);
    titleLabel->setTextSize(3);
    titleLabel->setAlignment(Alignment::CENTER);
    mainPanel->addComponent(titleLabel);

    // Current value display
    valueLabel = guiManager.createLabel("Value: 50.0");
    valueLabel->layoutProps.heightMode = SizeMode::FIXED;
    valueLabel->layoutProps.height = 25;
    valueLabel->layoutProps.widthMode = SizeMode::FILL;
    valueLabel->setTextColor(TFT_WHITE);
    valueLabel->setTextSize(2);
    valueLabel->setAlignment(Alignment::CENTER);
    mainPanel->addComponent(valueLabel);

    // Progress bar
    progressBar = guiManager.createProgressBar(0.0f, 100.0f);
    progressBar->layoutProps.heightMode = SizeMode::FIXED;
    progressBar->layoutProps.height = 40;
    progressBar->layoutProps.widthMode = SizeMode::FILL;
    progressBar->setColors(TFT_DARKGRAY, TFT_GREEN, TFT_WHITE);
    progressBar->setBorderWidth(2);
    progressBar->setTextColor(TFT_BLACK);
    progressBar->setValue(currentProgress);
    mainPanel->addComponent(progressBar);

    // Control buttons panel
    Panel* buttonPanel = guiManager.createFlexPanel(LayoutType::FLEX_ROW);
    buttonPanel->layoutProps.heightMode = SizeMode::FIXED;
    buttonPanel->layoutProps.height = 60;
    buttonPanel->layoutProps.widthMode = SizeMode::FILL;
    buttonPanel->setGap(20);
    buttonPanel->setAlignItems(Alignment::CENTER);
    buttonPanel->setJustifyContent(Alignment::CENTER);
    mainPanel->addComponent(buttonPanel);

    // Minus button
    minusButton = guiManager.createButton("-");
    minusButton->layoutProps.widthMode = SizeMode::FIXED;
    minusButton->layoutProps.width = 80;
    minusButton->layoutProps.heightMode = SizeMode::FILL;
    minusButton->setColors(TFT_RED, TFT_WHITE);
    buttonPanel->addComponent(minusButton);

    // Plus button
    plusButton = guiManager.createButton("+");
    plusButton->layoutProps.widthMode = SizeMode::FIXED;
    plusButton->layoutProps.width = 80;
    plusButton->layoutProps.heightMode = SizeMode::FILL;
    plusButton->setColors(TFT_GREEN, TFT_WHITE);
    buttonPanel->addComponent(plusButton);

    // Instructions
    Label* instructionLabel = guiManager.createLabel("Touch + or - to adjust progress");
    instructionLabel->layoutProps.heightMode = SizeMode::FIXED;
    instructionLabel->layoutProps.height = 20;
    instructionLabel->layoutProps.widthMode = SizeMode::FILL;
    instructionLabel->setTextColor(TFT_DARKGRAY);
    instructionLabel->setTextSize(1);
    instructionLabel->setAlignment(Alignment::CENTER);
    mainPanel->addComponent(instructionLabel);

    // Set up button callbacks
    plusButton->onClick = onPlusButtonClick;
    minusButton->onClick = onMinusButtonClick;

    // Force layout calculation
    mainPanel->calculateLayout();

    // Initialize the display
    updateProgressDisplay();

    Serial.println("Minimalistic Progress Control App initialized!");
}

void loop(void) {
    // Update GUI (handle touch events and draw components)
    guiManager.update();

    delay(200);
}

// Button callback functions
void onPlusButtonClick() {
    currentProgress += 5.0f;
    if (currentProgress > 100.0f) {
        currentProgress = 100.0f;
    }
    updateProgressDisplay();
    Serial.println("Plus button clicked - Progress: " + String(currentProgress));
}

void onMinusButtonClick() {
    currentProgress -= 5.0f;
    if (currentProgress < 0.0f) {
        currentProgress = 0.0f;
    }
    updateProgressDisplay();
    Serial.println("Minus button clicked - Progress: " + String(currentProgress));
}

void updateProgressDisplay() {
    if (progressBar) {
        progressBar->setValue(currentProgress);
    }
    if (valueLabel) {
        valueLabel->setText("Value: " + String(currentProgress, 1));
    }

    // Control LEDs based on progress level
    if (currentProgress <= 33.0f) {
        led_set(0);  // Red LED
    } else if (currentProgress <= 66.0f) {
        led_set(1);  // Green LED
    } else {
        led_set(2);  // Blue LED
    }
}

void led_set(int i) {
    if (i == 0) {
        digitalWrite(led_pin[0], LOW);
        digitalWrite(led_pin[1], HIGH);
        digitalWrite(led_pin[2], HIGH);
    }
    if (i == 1) {
        digitalWrite(led_pin[0], HIGH);
        digitalWrite(led_pin[1], LOW);
        digitalWrite(led_pin[2], HIGH);
    }
    if (i == 2) {
        digitalWrite(led_pin[0], HIGH);
        digitalWrite(led_pin[1], HIGH);
        digitalWrite(led_pin[2], LOW);
    }

    if (i == 3) {
        digitalWrite(led_pin[0], LOW);
        digitalWrite(led_pin[1], LOW);
        digitalWrite(led_pin[2], LOW);
    }
}
