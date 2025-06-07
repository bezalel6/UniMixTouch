# UniMixTouch Input System

A powerful, abstracted Input System for ESP32-based projects supporting buttons and rotary encoders with comprehensive event handling.

## Features

### 🔘 **Button Support**

- **Debouncing**: Hardware debouncing using the Bounce2 library
- **Event Types**: Press, Release, Click, Double-Click, Long Press
- **Configurable Timing**: Customizable debounce, long press, and double-click windows
- **Pull-up/Pull-down**: Configurable internal pull resistors

### 🔄 **Rotary Encoder Support**

- **Hardware Optimized**: Uses ESP32Encoder library for efficient interrupt-based operation
- **Range Limiting**: Set minimum and maximum values with automatic constraining
- **Button Integration**: Optional built-in encoder button with full button event support
- **Step Configuration**: Configurable steps per notch for different encoder types

### 🎯 **Event-Driven Architecture**

- **Callback System**: Global and device-specific event handlers
- **Event Types**: Comprehensive event enumeration for all input types
- **Timestamping**: All events include precise timing information
- **Device Management**: Enable/disable devices at runtime

### 🏗️ **Object-Oriented Design**

- **Abstract Base Classes**: Clean inheritance hierarchy
- **Singleton Pattern**: Global access through `Input::getInstance()`
- **RAII**: Automatic resource management with smart pointers
- **Type Safety**: Strong typing with enum classes

## Quick Start

### 1. Include the System

```cpp
#include "InputSystem.hpp"
```

### 2. Simple Setup

```cpp
void setup() {
    // Add a button on pin 25
    ButtonConfig btn(25);
    INPUT_SYSTEM.addButton(btn);

    // Add an encoder on pins 32, 33 with button on pin 34
    EncoderConfig enc(32, 33, 34, true, 0, 100, 50);
    INPUT_SYSTEM.addRotaryEncoder(enc);

    // Set global event handler
    INPUT_SYSTEM.setGlobalCallback([](const InputEvent& event) {
        Serial.printf("Event: Device %d, Type %d, Value %ld\n",
                      event.deviceId, (int)event.type, event.value);
    });

    // Initialize the system
    INPUT_SETUP();
}

void loop() {
    INPUT_LOOP();  // This is all you need!
    delay(10);
}
```

## Configuration

### Button Configuration

```cpp
ButtonConfig(
    uint8_t pin,              // GPIO pin number
    bool pullUp = true,       // Use internal pull-up
    uint32_t debounceTime = 50,    // Debounce time in ms
    uint32_t longPressTime = 1000, // Long press threshold in ms
    uint32_t doubleClickTime = 300  // Double-click window in ms
);
```

### Encoder Configuration

```cpp
EncoderConfig(
    uint8_t pinA,             // Encoder pin A
    uint8_t pinB,             // Encoder pin B
    uint8_t buttonPin = 255,  // Optional button pin (255 = no button)
    bool useInterrupts = true,     // Use hardware interrupts
    int32_t minValue = INT32_MIN,  // Minimum value
    int32_t maxValue = INT32_MAX,  // Maximum value
    int32_t initialValue = 0       // Starting value
);
```

## Event Types

### Button Events

```cpp
InputEventType::BUTTON_PRESS        // Button pressed down
InputEventType::BUTTON_RELEASE      // Button released
InputEventType::BUTTON_CLICK        // Single click completed
InputEventType::BUTTON_DOUBLE_CLICK // Double click detected
InputEventType::BUTTON_LONG_PRESS   // Long press threshold reached
```

### Encoder Events

```cpp
InputEventType::ENCODER_CLOCKWISE        // Rotated clockwise
InputEventType::ENCODER_COUNTERCLOCKWISE // Rotated counterclockwise
InputEventType::ENCODER_BUTTON_PRESS     // Encoder button pressed
InputEventType::ENCODER_BUTTON_RELEASE   // Encoder button released
```

## Advanced Usage

### Device-Specific Handlers

```cpp
uint8_t volumeKnob = INPUT_SYSTEM.addRotaryEncoder(volumeConfig);

INPUT_SYSTEM.setDeviceCallback(volumeKnob, [](const InputEvent& event) {
    if (event.type == InputEventType::ENCODER_CLOCKWISE) {
        increaseVolume(event.value);
    } else if (event.type == InputEventType::ENCODER_COUNTERCLOCKWISE) {
        decreaseVolume(event.value);
    }
});
```

### Runtime Device Control

```cpp
// Disable a device (useful for mode switching)
INPUT_SYSTEM.disableDevice(deviceId);

// Re-enable later
INPUT_SYSTEM.enableDevice(deviceId);

// Check status
if (INPUT_SYSTEM.isDeviceEnabled(deviceId)) {
    // Device is active
}
```

### Multi-Device Setup

```cpp
void setupMixerControls() {
    // Channel volume encoders
    for (int i = 0; i < 8; i++) {
        EncoderConfig channel(2 + i*2, 3 + i*2, 255, true, 0, 127, 64);
        uint8_t encoderId = INPUT_SYSTEM.addRotaryEncoder(channel);

        INPUT_SYSTEM.setDeviceCallback(encoderId, [i](const InputEvent& event) {
            updateChannelGain(i, event.value);
        });
    }

    // Mute buttons
    for (int i = 0; i < 8; i++) {
        ButtonConfig mute(20 + i);
        uint8_t buttonId = INPUT_SYSTEM.addButton(mute);

        INPUT_SYSTEM.setDeviceCallback(buttonId, [i](const InputEvent& event) {
            if (event.type == InputEventType::BUTTON_CLICK) {
                toggleChannelMute(i);
            }
        });
    }
}
```

## Integration with Existing Code

The Input System is designed to integrate seamlessly with existing projects:

```cpp
void setup() {
    // Your existing setup code
    initializeDisplay();
    setupGUI();

    // Add Input System
    setupInputDevices();
    INPUT_SETUP();

    // Continue with existing code
}

void loop() {
    // Your existing loop code
    updateDisplay();
    handleNetworking();

    // Add Input System (just one line!)
    INPUT_LOOP();

    // Continue with existing code
    delay(10);
}
```

## Memory and Performance

- **Efficient**: Interrupt-driven encoder reading for minimal CPU overhead
- **Scalable**: Supports dozens of input devices with minimal performance impact
- **Memory Conscious**: Smart pointer usage prevents memory leaks
- **Configurable**: Enable/disable features and devices as needed

## Hardware Requirements

- **ESP32**: Tested on ESP32-DEV boards
- **Buttons**: Any momentary switch, configurable pull-up/down
- **Rotary Encoders**: Standard quadrature encoders (e.g., KY-040)
- **Pull Resistors**: Internal pull-ups available, external resistors optional

## Dependencies

```ini
lib_deps =
    lovyan03/LovyanGFX@^1.1.6
    paulstoffregen/Encoder@^1.4.4
    madhephaestus/ESP32Encoder@^0.10.2
    thomasfredericks/Bounce2@^2.71
```

## Files Structure

```
include/
├── InputSystem.hpp          # Main system interface
└── InputExamples.hpp        # Usage examples

src/InputSystem/
├── Input.cpp               # Main system implementation
├── ButtonInput.hpp/.cpp    # Button device implementation
└── RotaryEncoderInput.hpp/.cpp # Encoder device implementation
```

## Example Projects

See `src/main_with_input_example.cpp` for a complete integration example showing:

- GUI touch buttons + hardware buttons working together
- Rotary encoder controlling the same UI elements
- Event handling for different input types
- Seamless integration with existing GUI code

## Troubleshooting

### Common Issues

1. **Encoder not responding**: Check pin connections and ensure `ESP32Encoder` library is installed
2. **Button bouncing**: Increase `debounceTime` in `ButtonConfig`
3. **Events not firing**: Verify `INPUT_SETUP()` is called before `INPUT_LOOP()`
4. **Memory issues**: Check that devices are properly initialized and callbacks are set correctly

### Debug Mode

Enable debug output by defining `DEBUG_INPUT_EVENTS` before including the header:

```cpp
#define DEBUG_INPUT_EVENTS
#include "InputSystem.hpp"
```

## Contributing

The Input System is designed to be extensible. To add new input device types:

1. Inherit from `InputDevice`
2. Implement `begin()` and `update()` methods
3. Add configuration structure
4. Add device creation method to `Input` class

## License

This Input System is part of the UniMixTouch project and follows the same licensing terms.
