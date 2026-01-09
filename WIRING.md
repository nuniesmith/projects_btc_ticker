# Wiring Guide - Bitcoin Price Tracker

This document provides detailed wiring instructions for the Bitcoin Price Tracker project.

## Table of Contents
- [Safety First](#safety-first)
- [Tools Required](#tools-required)
- [Power Distribution](#power-distribution)
- [Component Wiring](#component-wiring)
- [Pin Reference Table](#pin-reference-table)
- [Wiring Diagrams](#wiring-diagrams)
- [Testing Each Component](#testing-each-component)
- [Common Mistakes](#common-mistakes)

## Safety First

⚠️ **Before you begin:**
- Always disconnect USB power before making wiring changes
- Double-check connections before powering on
- Never short 3.3V to GND (can damage ESP32)
- Always use resistors with LEDs to prevent burnout
- Ensure breadboard connections are firm (loose wires cause intermittent issues)

## Tools Required

- ESP32 starter kit components
- 830-point breadboard
- Male-to-Male jumper wires (various lengths)
- 3x 220Ω resistors (Red-Red-Brown bands)
- Optional: Multimeter for testing connections
- Optional: Wire strippers if using solid core wire

## Power Distribution

The first step is to establish power and ground rails on your breadboard.

### Step 1: Setup Power Rails

```
ESP32 3.3V Pin  →  Breadboard Red Rail (+)
ESP32 GND Pin   →  Breadboard Blue Rail (-)
```

**Why 3.3V and not 5V/VIN?**
- ESP32 GPIO pins are 3.3V logic
- Most I2C devices (like the OLED) prefer 3.3V
- Safer for sensitive components

**Note:** Some breadboards have two sets of rails (top and bottom). You may need to bridge them with jumper wires if you run out of space.

## Component Wiring

### 1. OLED Display (0.96" I2C SSD1306)

The OLED uses I2C communication protocol and requires 4 connections.

#### Pin Identification
Your OLED should have 4 pins labeled:
- **GND** - Ground
- **VCC** (or VDD) - Power
- **SCL** (or SCK) - Clock signal
- **SDA** - Data signal

#### Connections

| OLED Pin | Connect To | ESP32 Pin | Wire Color Suggestion |
|----------|------------|-----------|----------------------|
| GND | Blue Rail (-) | GND | Black |
| VCC | Red Rail (+) | 3.3V | Red |
| SCL | Direct | GPIO 22 | Yellow |
| SDA | Direct | GPIO 21 | Green |

**Important Notes:**
- GPIO 21 and 22 are the default I2C pins on ESP32
- Some OLEDs have reversed pin order - always check labels!
- If display doesn't work, try I2C address 0x3D instead of 0x3C in code

---

### 2. RGB LED (Common Cathode)

The RGB LED allows you to display different colors for price trends.

#### Pin Identification

RGB LEDs typically have 4 pins with one longer than the others:
- **Longest pin** = Common Cathode (connect to GND)
- **3 shorter pins** = Red, Green, Blue anodes

To identify which short pin is which:
1. The longest pin is usually second from the left when flat side faces you
2. Pins are usually in order: R, GND (long), G, B (but test to confirm!)

#### Connections

⚠️ **CRITICAL:** Always use 220Ω resistors on the color legs!

| LED Pin | Resistor | Connect To | ESP32 Pin | Wire Color |
|---------|----------|------------|-----------|------------|
| Long Leg (-) | None | Blue Rail (-) | GND | Black |
| Red Leg | 220Ω | Direct | GPIO 15 | Red |
| Green Leg | 220Ω | Direct | GPIO 2 | Green |
| Blue Leg | 220Ω | Direct | GPIO 4 | Blue |

**Wiring Detail:**
```
[GPIO 15] ---[220Ω]--- [Red Leg of LED]
[GPIO 2]  ---[220Ω]--- [Green Leg of LED]
[GPIO 4]  ---[220Ω]--- [Blue Leg of LED]
[GND]     ------------ [Long Leg of LED]
```

**Testing the LED:**
- Red only = GPIO 15 HIGH, others LOW
- Green only = GPIO 2 HIGH, others LOW
- Blue only = GPIO 4 HIGH, others LOW
- Yellow = Red + Green HIGH
- Cyan = Green + Blue HIGH
- Magenta = Red + Blue HIGH
- White = All HIGH

**Common Anode Alternative:**
If you have a Common Anode LED (less common), the wiring is reversed:
- Long leg connects to 3.3V (Red Rail)
- To turn a color ON, set its GPIO to LOW
- Modify code: `digitalWrite(PIN_RED, LOW)` instead of HIGH

---

### 3. Passive Buzzer

The buzzer provides audio feedback for price changes.

#### Identifying Passive vs Active Buzzer

Your kit likely has both types:

| Type | Appearance | Behavior |
|------|------------|----------|
| **Passive** ✓ | Green circuit board visible on bottom | Needs PWM signal (tone() function) |
| **Active** ✗ | Black sealed bottom, often has sticker | Makes single tone with DC power |

**We need the PASSIVE buzzer** for this project!

#### Connections

| Buzzer Pin | Connect To | ESP32 Pin | Notes |
|------------|------------|-----------|-------|
| - (or shorter leg) | Blue Rail (-) | GND | Ground |
| + or S (longer leg) | Direct | GPIO 13 | Signal |

**No resistor needed** - buzzers have internal impedance.

**Testing:**
```cpp
tone(PIN_BUZZER, 1000, 500); // 1000Hz for 500ms
```

---

### 4. Photoresistor Module (4-Pin LDR)

The light sensor enables automatic muting in darkness.

#### Pin Identification

Your module should have 4 pins:
- **VCC** - Power
- **GND** - Ground  
- **DO** - Digital Output (we don't use this)
- **AO** - Analog Output (we use this!)

The module also has a blue potentiometer dial - ignore it since we're using analog output.

#### Connections

| Module Pin | Connect To | ESP32 Pin | Notes |
|------------|------------|-----------|-------|
| VCC | Red Rail (+) | 3.3V | Power |
| GND | Blue Rail (-) | GND | Ground |
| AO | Direct | GPIO 34 | Analog input |
| DO | Not connected | - | Leave empty |

**Why GPIO 34?**
- GPIO 34-39 are input-only pins, perfect for sensors
- They have ADC (Analog-to-Digital Converter) capability
- Reads values from 0 (dark) to 4095 (bright)

**Testing:**
```cpp
int light = analogRead(34);
Serial.println(light); // Check value in Serial Monitor
```

---

### 5. Push Button (Tactile Switch)

The button allows manual price refresh on demand.

#### Understanding Tactile Buttons

Most tactile buttons have 4 legs arranged in pairs:
```
   [1]     [2]
    |       |
    |       |
   [3]     [4]
```

- Legs 1 & 3 are internally connected
- Legs 2 & 4 are internally connected
- Pressing the button connects all 4 legs together

**Safe approach:** Use diagonal legs (1 & 4, or 2 & 3) to guarantee proper connection.

#### Connections

| Button Pin | Connect To | ESP32 Pin | Notes |
|------------|------------|-----------|-------|
| Leg 1 | Direct | GPIO 23 | Signal |
| Leg 3 (diagonal) | Blue Rail (-) | GND | Ground |

**No pull-up resistor needed** - we use ESP32's internal pull-up in code with `INPUT_PULLUP`.

**How it works:**
- Button NOT pressed: GPIO 23 reads HIGH (pulled up internally)
- Button pressed: GPIO 23 reads LOW (connected to GND)

**Testing:**
```cpp
pinMode(23, INPUT_PULLUP);
if (digitalRead(23) == LOW) {
  Serial.println("Button pressed!");
}
```

---

## Pin Reference Table

### Complete ESP32 Pin Assignment

| GPIO | Function | Component | Notes |
|------|----------|-----------|-------|
| 21 | I2C SDA | OLED Display | I2C Data |
| 22 | I2C SCL | OLED Display | I2C Clock |
| 15 | Digital Out | RGB LED (Red) | Via 220Ω resistor |
| 2 | Digital Out | RGB LED (Green) | Via 220Ω resistor |
| 4 | Digital Out | RGB LED (Blue) | Via 220Ω resistor |
| 13 | PWM Out | Passive Buzzer | Tone generation |
| 34 | Analog In | Photoresistor (AO) | ADC input only |
| 23 | Digital In | Push Button | INPUT_PULLUP mode |
| 3.3V | Power | All VCC pins | Via breadboard rail |
| GND | Ground | All GND pins | Via breadboard rail |

### Reserved/Avoid These Pins

Some ESP32 pins should be avoided:
- **GPIO 0**: Boot mode selection (hold LOW to enter flash mode)
- **GPIO 1 & 3**: UART TX/RX (used for Serial Monitor)
- **GPIO 6-11**: Connected to flash memory (DO NOT USE)
- **GPIO 12**: Boot fail if pulled HIGH

---

## Wiring Diagrams

### Breadboard Layout (Text Diagram)

```
                    [ESP32 DEVKIT V1]
                         (USB)
    ┌────────────────────────────────────────┐
    │                                        │
    │  3.3V ○                         ○ GND  │
    │   D21 ○ (OLED SDA)              ○ D23  │ (Button)
    │   D22 ○ (OLED SCL)              ○ D34  │ (LDR)
    │   D15 ○ (LED Red)               ○ D13  │ (Buzzer)
    │    D2 ○ (LED Green)                    │
    │    D4 ○ (LED Blue)                     │
    │                                        │
    └────────────────────────────────────────┘
         │                              │
         └──────────┬──────────┬────────┘
                    │          │
              [Red Rail]  [Blue Rail]
                (+)          (-)
                 │            │
         ┌───────┴────────────┴───────┐
         │      BREADBOARD             │
         │                             │
         │  [OLED] [LED] [Buzzer]      │
         │  [LDR Module] [Button]      │
         └─────────────────────────────┘
```

### Physical Layout Suggestion

**Left side of breadboard:**
- OLED Display (upright, facing forward)
- Photoresistor module (facing up/forward)

**Center of breadboard:**
- ESP32 (USB port facing toward you for easy access)

**Right side of breadboard:**
- RGB LED (standing upright, visible from front)
- Passive Buzzer (laying flat)
- Push Button (accessible from front)

---

## Testing Each Component

### Test Order (Recommended)

Build and test incrementally to catch issues early:

#### 1. Test OLED Display First
- Wire only the OLED
- Upload `bitcoin_ticker_v1_basic.ino`
- Verify display shows "Booting..."
- Troubleshoot I2C before proceeding

#### 2. Add RGB LED
- Wire the LED with resistors
- Upload test code that cycles colors
- Verify all 3 colors work independently

#### 3. Add Buzzer
- Wire the buzzer
- Upload test code with `tone()` function
- Verify you hear different frequencies

#### 4. Add Photoresistor
- Wire the LDR module
- Read values with `analogRead(34)`
- Cover sensor and verify values change

#### 5. Add Button Last
- Wire the button
- Test with simple `digitalRead(23)` code
- Verify it reads LOW when pressed

### Quick Test Sketch

Upload this to test all components:

```cpp
void setup() {
  Serial.begin(115200);
  pinMode(15, OUTPUT); // Red
  pinMode(2, OUTPUT);  // Green
  pinMode(4, OUTPUT);  // Blue
  pinMode(13, OUTPUT); // Buzzer
  pinMode(23, INPUT_PULLUP); // Button
}

void loop() {
  // Test LEDs
  digitalWrite(15, HIGH); delay(500); digitalWrite(15, LOW);
  digitalWrite(2, HIGH); delay(500); digitalWrite(2, LOW);
  digitalWrite(4, HIGH); delay(500); digitalWrite(4, LOW);
  
  // Test Buzzer
  tone(13, 1000, 200);
  delay(300);
  
  // Test Light Sensor
  Serial.print("Light: ");
  Serial.println(analogRead(34));
  
  // Test Button
  if(digitalRead(23) == LOW) {
    Serial.println("BUTTON PRESSED!");
  }
  
  delay(1000);
}
```

---

## Common Mistakes

### Issue: OLED Doesn't Light Up

**Possible Causes:**
1. ❌ Wrong I2C address (try 0x3D instead of 0x3C)
2. ❌ SDA/SCL swapped
3. ❌ Using 5V instead of 3.3V
4. ❌ Loose breadboard connections
5. ❌ OLED is defective

**How to test:**
```cpp
// Run I2C scanner code
Wire.begin(21, 22);
byte error = Wire.endTransmission();
```

### Issue: RGB LED Not Working or Dim

**Possible Causes:**
1. ❌ Forgot resistors (may burn out LED!)
2. ❌ Wrong leg identification (try other legs)
3. ❌ Common Anode instead of Common Cathode
4. ❌ LED inserted backward

**Test:** Remove LED, touch legs briefly to 3.3V with resistor to identify pins.

### Issue: No Sound from Buzzer

**Possible Causes:**
1. ❌ Using Active buzzer instead of Passive
2. ❌ Buzzer polarity reversed (try flipping it)
3. ❌ GPIO 13 not connected properly
4. ❌ Environment is dark (Smart Mute active!)

**Test:** Try `digitalWrite(13, HIGH)` - Active buzzer will sound, Passive won't.

### Issue: Button Always Reads Pressed (or Never Pressed)

**Possible Causes:**
1. ❌ Not using `INPUT_PULLUP` mode
2. ❌ Connected same-side legs instead of diagonal
3. ❌ Loose connection

**Test:** Read button state without pressing - should be HIGH with INPUT_PULLUP.

### Issue: Photoresistor Values Don't Change

**Possible Causes:**
1. ❌ Using DO pin instead of AO
2. ❌ Module not powered
3. ❌ Wrong GPIO (must use ADC-capable pin)

**Test:** Cover sensor with hand - value should drop significantly.

---

## Final Checklist

Before uploading the final code, verify:

- [ ] All power connections go to Red Rail (3.3V)
- [ ] All ground connections go to Blue Rail (GND)
- [ ] OLED is on GPIO 21 (SDA) and 22 (SCL)
- [ ] RGB LED has 220Ω resistors on all 3 color legs
- [ ] RGB LED long leg is connected to GND
- [ ] Passive buzzer (not Active) is on GPIO 13
- [ ] Photoresistor AO pin is on GPIO 34
- [ ] Button uses INPUT_PULLUP mode on GPIO 23
- [ ] No wires are crossed or shorting
- [ ] All breadboard connections are firm

---

## Troubleshooting Resources

If you encounter issues not covered here:

1. **Serial Monitor**: Set to 115200 baud and check for error messages
2. **Multimeter**: Test continuity and voltage levels
3. **I2C Scanner**: Verify OLED is detected
4. **Component Test**: Test each component individually

**Still stuck?** Open an issue on GitHub with:
- Photo of your wiring
- Serial Monitor output
- Description of the problem

---

**Good luck with your build! 🚀**