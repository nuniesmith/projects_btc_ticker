# Bill of Materials (BOM) - Bitcoin Price Tracker

Complete list of components required to build the Bitcoin Price Tracker IoT project.

## Overview

This project is designed to work with standard **ESP32 Starter Kits**, which typically include all necessary components. If you're purchasing parts individually, use this guide.

**Estimated Total Cost:** $15-25 USD (if buying complete starter kit)  
**Estimated Total Cost:** $25-35 USD (if buying components individually)

---

## Required Components

### Core Electronics

| Item | Specification | Quantity | Est. Price | Notes |
|------|---------------|----------|------------|-------|
| **ESP32 Development Board** | ESP32-WROOM-32, 30-pin DevKit | 1 | $6-10 | DOIT ESP32 DEVKIT V1 or compatible |
| **0.96" OLED Display** | I2C, SSD1306, 128x64, 4-pin | 1 | $3-5 | White or blue display |
| **RGB LED** | 5mm Common Cathode, 4-pin | 1 | $0.50 | Can substitute with separate R/G/B LEDs |
| **Passive Buzzer** | 5V, 12mm diameter | 1 | $0.50 | NOT active buzzer |
| **Photoresistor Module** | 4-pin LDR module with AO output | 1 | $1-2 | GL5528 or similar |
| **Tactile Push Button** | 6x6mm, 4-pin | 1 | $0.20 | 12x12mm also works |
| **220Ω Resistors** | 1/4W, 5% tolerance | 3 | $0.30 | For RGB LED legs |
| **830-Point Breadboard** | Standard solderless | 1 | $3-5 | 400-point also works |
| **Male-to-Male Jumper Wires** | Various lengths, 20cm | 20-30 | $2-3 | Multi-color pack recommended |
| **Micro-USB Cable** | Data + Power capable | 1 | $2-4 | Must support data transfer |

**Total (Individual Parts):** ~$25-35 USD

---

## Recommended Starter Kits

These kits contain all required components plus extras for future projects:

### Option 1: Complete ESP32 Kit (Recommended)
**What to look for:**
- ESP32-WROOM-32 board
- 0.96" OLED I2C display
- RGB LED (Common Cathode)
- Passive buzzer
- Photoresistor/LDR module
- Push buttons
- 220Ω resistors
- Breadboard
- Jumper wires

**Search terms:**
- "ESP32 Starter Kit with OLED"
- "ESP32 IoT Kit"
- "ESP32 Development Kit with Sensors"

**Estimated Price:** $15-25 USD on Amazon/AliExpress/eBay

### Option 2: Basic ESP32 + Separate Display
If you already have an ESP32:
- Buy 0.96" I2C OLED separately ($3-5)
- Most other components available in basic electronics kits

---

## Component Specifications (Detailed)

### 1. ESP32 Development Board

**Required Specifications:**
- Chip: ESP32-WROOM-32 or ESP32-WROOM-32D
- Flash: 4MB minimum
- Pins: 30-pin layout (15 pins per side)
- WiFi: 2.4GHz 802.11 b/g/n
- Bluetooth: 4.2 (not used in this project)
- USB: Micro-USB or USB-C with CP2102 or CH340 chip

**Compatible Boards:**
- ✅ DOIT ESP32 DEVKIT V1 (most common)
- ✅ ESP32-DevKitC
- ✅ NodeMCU-32S
- ✅ ESP32-WROOM-32U (with external antenna)

**NOT Compatible:**
- ❌ ESP8266 (different chip, won't work)
- ❌ ESP32-CAM (no standard pin headers)
- ❌ ESP32-S2 (different GPIO layout)

**Where to Buy:**
- Amazon: Search "ESP32 DEVKIT V1"
- AliExpress: Search "ESP32 WROOM 32"
- Adafruit: ESP32 Feather (requires pin adapter)
- SparkFun: ESP32 Thing

---

### 2. OLED Display

**Required Specifications:**
- Size: 0.96 inch
- Resolution: 128x64 pixels
- Driver: SSD1306
- Interface: I2C (4-pin)
- Voltage: 3.3V - 5V
- Color: White or Blue (both work)

**Pin Labels (must have):**
- GND
- VCC (or VDD)
- SCL (or SCK, CLK)
- SDA (or SDD)

**Common Variants:**
- White on black background ✅ (recommended for readability)
- Blue on black background ✅
- Yellow/Blue split screen ✅
- SPI 7-pin version ❌ (different interface, requires code changes)

**Where to Buy:**
- Amazon: "0.96 I2C OLED SSD1306"
- AliExpress: ~$2-3 with free shipping
- Adafruit: Part #326 or #938

---

### 3. RGB LED

**Required Specifications:**
- Type: Common Cathode (NOT Common Anode)
- Size: 5mm diameter (standard)
- Pins: 4 (R, GND, G, B)
- Forward Voltage: ~2-3.3V per color
- Current: 20mA per color

**How to Identify:**
- Longest pin = Common (cathode/negative)
- Usually arranged: Red, GND (long), Green, Blue
- Diffused lens (milky white) preferred

**Alternative:**
If you can't find RGB LED, use 3 separate LEDs:
- 1x Red LED
- 1x Green LED  
- 1x Blue LED
(Each with 220Ω resistor)

**Where to Buy:**
- Any electronics kit
- Amazon: "RGB LED Common Cathode 5mm"
- Local electronics store: ~$0.50 each

---

### 4. Passive Buzzer

**Required Specifications:**
- Type: **Passive** (Magnetic, requires PWM)
- Voltage: 3-5V
- Size: 12mm diameter (typical)
- Pins: 2 (+ and -)

**How to Identify Passive vs Active:**

| Feature | Passive ✅ | Active ❌ |
|---------|-----------|----------|
| Bottom | Green PCB visible | Black sealed |
| Sound | Needs tone() function | Sounds with DC power |
| Use case | Musical tones | Simple beep |
| Sticker | Usually none | Often has sticker |

**IMPORTANT:** This project requires PASSIVE! Active buzzer won't work properly.

**Where to Buy:**
- Amazon: "Passive Buzzer 5V"
- Component kits usually include both types

---

### 5. Photoresistor Module

**Required Specifications:**
- Type: LDR (Light Dependent Resistor) module
- Pins: 4 (VCC, GND, DO, AO)
- Output: Analog (AO pin)
- Sensor: GL5528, GL5516, or similar
- Voltage: 3.3-5V

**Must Have:**
- AO (Analog Output) pin for ESP32 ADC
- Blue potentiometer for sensitivity (we won't use it)

**NOT Required:**
- Raw photoresistor (2-pin) requires additional resistor

**Pin Functions:**
- VCC: Power
- GND: Ground
- DO: Digital output (we ignore this)
- AO: Analog output (0-4095 value)

**Where to Buy:**
- Amazon: "LDR Photoresistor Module"
- AliExpress: "Light Sensor Module"
- Usually included in sensor kits

---

### 6. Tactile Push Button

**Required Specifications:**
- Type: Momentary tactile switch
- Size: 6x6mm or 12x12mm
- Pins: 4 legs (2 pairs)
- Actuation Force: 160-260gf
- Travel: 0.25mm

**Optional:**
- Switch keycap (plastic button top)
- Colors available: various

**How It Works:**
- 4 legs arranged in 2 pairs
- Pressing connects both pairs
- Use diagonal legs for guaranteed connection

**Where to Buy:**
- Any electronics kit
- Amazon: "Tactile Push Button 6x6"
- Very cheap: ~$0.10-0.20 each

---

### 7. Resistors

**Required Specifications:**
- Value: 220Ω (Red-Red-Brown color bands)
- Power: 1/4W (standard)
- Tolerance: 5% (gold band)
- Quantity: 3 minimum (one per RGB LED leg)

**Color Code:**
```
Red - Red - Brown - Gold
 2     2     0     ±5%
= 22 × 10^0 = 220Ω
```

**Alternatives:**
- 150Ω (slightly brighter LED)
- 330Ω (slightly dimmer LED)
- 470Ω (much dimmer, safer)

**Do NOT use:**
- Less than 100Ω (may damage LED or ESP32)
- No resistor (will burn out LED!)

**Where to Buy:**
- Resistor assortment kits (best value)
- Individual packs (10-100 pieces)

---

### 8. Breadboard

**Required Specifications:**
- Type: Solderless
- Size: 830 tie points (recommended) or 400 points (minimum)
- Power rails: 2 (top and bottom preferred)
- Dimensions: ~165mm x 55mm

**Features:**
- Self-adhesive backing (optional)
- Accepts 22-24 AWG wire
- Binding posts (optional)

**Where to Buy:**
- Amazon: "830 Point Breadboard"
- Usually included in starter kits

---

### 9. Jumper Wires

**Required Specifications:**
- Type: Male-to-Male (M-M)
- Length: 10-20cm assorted
- Gauge: 22-26 AWG
- Quantity: 20-30 wires minimum

**Recommended:**
- Multiple colors for organization
- Pre-cut lengths (not spool)
- Flexible stranded (not solid core)

**Color Convention (optional but helpful):**
- Red: Power (3.3V)
- Black: Ground (GND)
- Yellow: I2C Clock (SCL)
- Green: I2C Data (SDA)
- Others: Signal wires

**Where to Buy:**
- Jumper wire packs (40-120 pieces)
- Usually included in breadboard kits

---

## Optional Components

### For Future Enhancements

| Item | Purpose | Est. Price |
|------|---------|------------|
| LiPo Battery 3.7V 1000mAh | Portable power | $5-8 |
| TP4056 Charging Module | Battery charging | $1-2 |
| Switch (SPST) | Power on/off | $0.50 |
| Case/Enclosure | Protection | $3-10 |
| Heat shrink tubing | Wire protection | $2-5 |

---

## Tools (Not Included)

You'll also need:

| Tool | Purpose | Priority |
|------|---------|----------|
| **Computer** | Upload code to ESP32 | Required |
| **Arduino IDE** | Software (free download) | Required |
| **Micro-USB Cable** | Data transfer | Required |
| Wire stripper | Clean connections | Recommended |
| Multimeter | Testing/debugging | Recommended |
| Flush cutters | Trimming wires | Optional |
| Needle-nose pliers | Bending component legs | Optional |
| Anti-static mat | Component safety | Optional |

---

## Where to Buy - Quick Links

### Complete Kits (Best Value)

**Amazon** (Fast shipping):
- Search: "ESP32 Starter Kit" or "ESP32 IoT Kit"
- Filter: Prime eligible
- Price range: $15-30

**AliExpress** (Cheapest, slow shipping):
- Search: "ESP32 Development Board Kit"
- Filter: Orders, ratings
- Price range: $8-20 (2-4 week shipping)

**eBay**:
- Similar to AliExpress pricing
- Sometimes faster shipping

### Individual Components

**Adafruit** (US, quality parts):
- ESP32 Feather: $19.95
- OLED Display: $17.50
- Higher quality, US-based support

**SparkFun** (US, quality parts):
- ESP32 Thing: $20.95
- Sensor kits available

**Digi-Key / Mouser** (Professional):
- Best for specific components
- Higher prices, excellent specs
- Same-day shipping available

**Local Electronics Store**:
- RadioShack (if still available)
- Fry's Electronics
- Micro Center

---

## Money-Saving Tips

1. **Buy the kit first** - Cheaper than individual parts
2. **Check for bundled OLED** - Many ESP32 kits include it
3. **Buy from AliExpress** - If you can wait 2-4 weeks
4. **Share with friends** - Split a large component kit
5. **Check local makers** - Hackerspaces often sell parts

---

## Verification Checklist

Before starting your build, verify you have:

- [ ] ESP32 board (30-pin, ESP32-WROOM-32)
- [ ] 0.96" OLED display (I2C, 4-pin)
- [ ] RGB LED (Common Cathode) OR 3 separate LEDs
- [ ] PASSIVE buzzer (not active!)
- [ ] Photoresistor module with AO pin
- [ ] Push button (tactile switch)
- [ ] 3x 220Ω resistors
- [ ] Breadboard (400+ points)
- [ ] 20+ jumper wires (M-M)
- [ ] Micro-USB cable (data capable)
- [ ] Computer with Arduino IDE installed

---

## Component Substitutions

### Can I use different components?

| Original | Substitution | Works? | Notes |
|----------|--------------|--------|-------|
| ESP32-WROOM-32 | ESP8266 | ❌ | Different chip, incompatible |
| ESP32-WROOM-32 | ESP32-S2 | ⚠️ | Requires pin changes |
| 0.96" OLED | 1.3" OLED | ✅ | May need code adjustment |
| I2C OLED | SPI OLED | ⚠️ | Requires significant code changes |
| Passive Buzzer | Active Buzzer | ⚠️ | Works but no tone control |
| Common Cathode RGB | Common Anode RGB | ⚠️ | Invert HIGH/LOW in code |
| 220Ω Resistor | 330Ω | ✅ | LED slightly dimmer |
| 4-pin LDR Module | Raw Photoresistor | ⚠️ | Need to add 10kΩ resistor |
| 6x6mm Button | 12x12mm Button | ✅ | Larger but same function |

---

## Frequently Asked Questions

### Q: Can I use a 5V power supply instead of USB?
**A:** Yes, connect to VIN pin (not 3.3V pin). USB is easier for development.

### Q: Do I need a programmer for ESP32?
**A:** No, ESP32 has built-in USB-to-serial. Just plug in USB cable.

### Q: Will this work with ESP8266?
**A:** No, different chip with different GPIO and capabilities.

### Q: Can I power this with batteries?
**A:** Yes! Use 3.7V LiPo battery + TP4056 charging module (future enhancement).

### Q: Where do I get the Arduino code?
**A:** It's included in this repository under `/src/` folder.

### Q: Do I need to solder anything?
**A:** No! This is a breadboard project. Soldering is optional for permanent builds.

---

**Last Updated:** January 2025  
**Version:** 1.0.0

*Have questions about parts? Open an issue on GitHub!*