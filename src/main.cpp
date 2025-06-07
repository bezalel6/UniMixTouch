// 测试通过
#include <SPI.h>

#include <vector>

#include "BluetoothSerial.h"
#include "ESP32_SPI_9341.h"
#include "GUI/button.hpp"
#include "GUI/component.hpp"
using namespace std;

#define SD_SCK 18
#define SD_MISO 19
#define SD_MOSI 23
#define SD_CS 5

#define DEFAULT_TEXT_SIZE 3

#define LIGHT_ADC 34

int led_pin[3] = {17, 4, 16};

LGFX lcd;

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

vector<Button> components;

void touch_calibration();
void led_set(int i);
void setup(void) {
    pinMode(led_pin[0], OUTPUT);
    pinMode(led_pin[1], OUTPUT);
    pinMode(led_pin[2], OUTPUT);

    Serial.begin(115200);

    SerialBT.begin("ESP32test");  // Bluetooth device name
    Serial.println("The device started, now you can pair it with bluetooth!");
    // pinMode(LCD_BL, OUTPUT);
    // digitalWrite(LCD_BL, HIGH);

    lcd.init();
    lcd.setTextSize(DEFAULT_TEXT_SIZE);
    lcd.setRotation(0);
    // print_img(SD, "/logo.bmp", 240, 320);
    Rectangle bounds = {10, 10, 100, 100};
    Button btn(bounds, "hello");
    components.push_back(btn);
    // components.push_back())

    touch_calibration();
}

void loop(void) {
    // lcd.clear();
    for (auto component : components) {
        if (component.checkTouching(lcd)) {
            component.clicked();
        }
        component.draw(lcd);
    }
    if (Serial.available()) {
        SerialBT.write(Serial.read());
    }

    if (SerialBT.available()) {
        lcd.println(SerialBT.readString());
        // Serial.write(SerialBT.read());
    }
    delay(20);
}

void touch_calibration() {
    lcd.fillScreen(TFT_YELLOW);

    lcd.setTextColor(TFT_BLACK);
    lcd.setTextSize(4);
    lcd.setCursor(70, 110);
    lcd.println("SCREEN");
    lcd.setCursor(70, 150);
    lcd.println("CALIBRATION");

    // タッチを使用する場合、キャリブレーションを行います。画面の四隅に表示される矢印の先端を順にタッチしてください。
    std::uint16_t fg = TFT_WHITE;
    std::uint16_t bg = TFT_BLACK;
    if (lcd.isEPD())
        std::swap(fg, bg);
    lcd.calibrateTouch(nullptr, fg, bg, std::max(lcd.width(), lcd.height()) >> 3);

    lcd.clear();
    lcd.setTextSize(DEFAULT_TEXT_SIZE);
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
