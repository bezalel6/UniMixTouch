// 测试通过
#include <SPI.h>

#include <vector>

#include "BluetoothSerial.h"
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

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

void led_set(int i);
void setup(void) {
    pinMode(led_pin[0], OUTPUT);
    pinMode(led_pin[1], OUTPUT);
    pinMode(led_pin[2], OUTPUT);

    Serial.begin(115200);

    // pinMode(LCD_BL, OUTPUT);
    // digitalWrite(LCD_BL, HIGH);

    // Initialize GUI Manager
    guiManager.init();

    // Create and add GUI components using helper method
    Button* btn = guiManager.createButton(10, 10, 200, 100, "hello");
}

void loop(void) {
    // Update GUI (handle touch events and draw components)
    guiManager.update();

    delay(20);
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
