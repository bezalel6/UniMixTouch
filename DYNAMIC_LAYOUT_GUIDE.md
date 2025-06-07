# Dynamic Layout System Guide

This guide explains how to use the new dynamic layout system that makes GUI components smarter and easier to manage.

## Overview

The new system introduces:

- **Panel Components**: Containers that can hold multiple components with automatic layout
- **Dynamic Sizing**: Components can be sized relatively (percentages, flex, auto)
- **Layout Engines**: Flexbox-style, Grid, and Stack layout algorithms
- **Layout Properties**: Margins, padding, alignment, spacing

## Key Concepts

### Layout Types

```cpp
enum class LayoutType {
    NONE,        // No automatic layout (absolute positioning)
    FLEX_ROW,    // Horizontal flex layout
    FLEX_COLUMN, // Vertical flex layout
    GRID,        // Grid layout
    STACK        // Stack layout (components on top of each other)
};
```

### Size Modes

```cpp
enum class SizeMode {
    FIXED,       // Fixed pixel size
    PERCENT,     // Percentage of parent container
    FLEX,        // Flexible size (grows/shrinks)
    AUTO,        // Size based on content
    FILL         // Fill remaining space
};
```

### Alignment Options

```cpp
enum class Alignment {
    START,       // Left/Top alignment
    CENTER,      // Center alignment
    END,         // Right/Bottom alignment
    STRETCH      // Stretch to fill
};
```

## Basic Usage

### Creating Layout-Managed Components

Instead of specifying absolute positions:

```cpp
// Old way - absolute positioning
Button* btn = guiManager.createButton(10, 10, 200, 100, "Click Me");

// New way - layout-managed
Button* btn = guiManager.createButton("Click Me");
btn->layoutProps.setSize(200, 100);  // Optional - can be auto-sized
btn->layoutProps.setMargin(10);
```

### Creating Panels

```cpp
// Create a vertical flex panel
Panel* mainPanel = guiManager.createFlexPanel(LayoutType::FLEX_COLUMN);
mainPanel->bounds = Rectangle(0, 0, width, height);  // Set container size
mainPanel->setGap(10);         // Space between child components
mainPanel->setPadding(20);     // Internal padding

// Add components to the panel
Button* btn1 = guiManager.createButton("Button 1");
Button* btn2 = guiManager.createButton("Button 2");
mainPanel->addComponent(btn1);
mainPanel->addComponent(btn2);

// Layout is calculated automatically
mainPanel->calculateLayout();
```

## Layout Properties

Each component has a `layoutProps` object that controls its layout behavior:

### Size Configuration

```cpp
// Fixed size
component->layoutProps.setSize(200, 100);

// Percentage of parent
component->layoutProps.setSizePercent(0.5f, 0.3f);  // 50% width, 30% height

// Flexible sizing
component->layoutProps.setFlex(1, 1);  // grow=1, shrink=1

// Mixed sizing
component->layoutProps.widthMode = SizeMode::FLEX;
component->layoutProps.heightMode = SizeMode::FIXED;
component->layoutProps.height = 50;
component->layoutProps.flexGrow = 2;  // Takes 2x space compared to flex=1
```

### Spacing

```cpp
// Margins (space outside component)
component->layoutProps.setMargin(10);           // All sides
component->layoutProps.setMargin(10, 20);       // vertical, horizontal
component->layoutProps.marginTop = 5;           // Individual sides

// Padding (space inside component - for panels)
panel->layoutProps.setPadding(15);
panel->layoutProps.setPadding(10, 20);
```

## Layout Examples

### 1. Vertical Button List

```cpp
Panel* buttonList = guiManager.createFlexPanel(LayoutType::FLEX_COLUMN);
buttonList->setGap(5);
buttonList->setPadding(10);

for (int i = 0; i < 5; i++) {
    Button* btn = guiManager.createButton("Button " + String(i + 1));
    btn->layoutProps.heightMode = SizeMode::FIXED;
    btn->layoutProps.height = 40;
    btn->layoutProps.widthMode = SizeMode::FILL;
    buttonList->addComponent(btn);
}
```

### 2. Horizontal Button Row

```cpp
Panel* buttonRow = guiManager.createFlexPanel(LayoutType::FLEX_ROW);
buttonRow->setGap(10);

Button* btn1 = guiManager.createButton("OK");
btn1->layoutProps.setFlex(1);
buttonRow->addComponent(btn1);

Button* btn2 = guiManager.createButton("Cancel");
btn2->layoutProps.setFlex(1);
buttonRow->addComponent(btn2);

Button* btn3 = guiManager.createButton("Help");
btn3->layoutProps.widthMode = SizeMode::FIXED;
btn3->layoutProps.width = 60;  // Fixed width, others flex
buttonRow->addComponent(btn3);
```

### 3. Grid Layout

```cpp
Panel* grid = guiManager.createGridPanel(3, 2);  // 3 columns, 2 rows
grid->bounds = Rectangle(0, 0, 300, 200);

for (int i = 0; i < 6; i++) {
    Button* btn = guiManager.createButton("Grid " + String(i + 1));
    btn->layoutProps.gridColumn = i % 3;
    btn->layoutProps.gridRow = i / 3;
    btn->layoutProps.setMargin(2);
    grid->addComponent(btn);
}
```

### 4. Complex Layout with Header, Content, Footer

```cpp
// Main container
Panel* mainPanel = guiManager.createFlexPanel(LayoutType::FLEX_COLUMN);
mainPanel->bounds = Rectangle(0, 0, screenWidth, screenHeight);

// Header
Label* header = guiManager.createLabel("Application Title");
header->layoutProps.heightMode = SizeMode::FIXED;
header->layoutProps.height = 50;
header->layoutProps.widthMode = SizeMode::FILL;
header->setAlignment(Alignment::CENTER);
header->setBackgroundColor(TFT_BLUE);
mainPanel->addComponent(header);

// Content area (takes remaining space)
Panel* content = guiManager.createFlexPanel(LayoutType::FLEX_ROW);
content->layoutProps.setFlex(1);  // Grows to fill remaining space
content->layoutProps.widthMode = SizeMode::FILL;
content->setGap(10);
content->setPadding(10);
mainPanel->addComponent(content);

// Sidebar
Panel* sidebar = guiManager.createFlexPanel(LayoutType::FLEX_COLUMN);
sidebar->layoutProps.widthMode = SizeMode::FIXED;
sidebar->layoutProps.width = 150;
sidebar->layoutProps.heightMode = SizeMode::FILL;
sidebar->setBackgroundColor(TFT_DARKGRAY);
content->addComponent(sidebar);

// Main content
Panel* mainContent = guiManager.createPanel();
mainContent->layoutProps.setFlex(1);  // Takes remaining width
mainContent->layoutProps.heightMode = SizeMode::FILL;
mainContent->setBackgroundColor(TFT_WHITE);
content->addComponent(mainContent);

// Footer
Label* footer = guiManager.createLabel("Status: Ready");
footer->layoutProps.heightMode = SizeMode::FIXED;
footer->layoutProps.height = 30;
footer->layoutProps.widthMode = SizeMode::FILL;
footer->setBackgroundColor(TFT_DARKGREEN);
mainPanel->addComponent(footer);

// Calculate layout
mainPanel->calculateLayout();
```

## Panel Alignment

Control how child components are aligned within a panel:

```cpp
panel->setAlignItems(Alignment::CENTER);     // Cross-axis alignment
panel->setJustifyContent(Alignment::CENTER); // Main-axis alignment

// For flex layouts:
// - In FLEX_ROW: main axis = horizontal, cross axis = vertical
// - In FLEX_COLUMN: main axis = vertical, cross axis = horizontal
```

## Best Practices

1. **Use Panels for Organization**: Group related components in panels
2. **Set Container Sizes First**: Define the bounds of your main panels
3. **Use Flex for Responsive Design**: Components adapt to different screen sizes
4. **Combine Size Modes**: Mix fixed, flex, and percentage sizing as needed
5. **Test Different Screen Orientations**: Ensure layouts work in both portrait and landscape

## Migration from Absolute Positioning

To migrate existing code:

1. **Replace fixed positioning** with layout-managed components
2. **Group related components** into panels
3. **Use flex layouts** for dynamic sizing
4. **Replace manual calculations** with layout properties

```cpp
// Before
Button* btn1 = guiManager.createButton(10, 10, 100, 50, "Button 1");
Button* btn2 = guiManager.createButton(120, 10, 100, 50, "Button 2");

// After
Panel* buttonPanel = guiManager.createFlexPanel(LayoutType::FLEX_ROW);
buttonPanel->bounds = Rectangle(10, 10, 230, 50);
buttonPanel->setGap(10);

Button* btn1 = guiManager.createButton("Button 1");
btn1->layoutProps.setFlex(1);
buttonPanel->addComponent(btn1);

Button* btn2 = guiManager.createButton("Button 2");
btn2->layoutProps.setFlex(1);
buttonPanel->addComponent(btn2);
```

## Performance Notes

- Layout calculation is only triggered when components are added/removed or when explicitly called
- Components are marked dirty automatically when layout changes
- Use `panel->calculateLayout()` to force recalculation when needed
- Nested panels automatically handle their own layout calculations
