# Quick Start Guide - Bitcoin Price Tracker

Get your Bitcoin Price Tracker running in under 30 minutes!

## What You Need

✅ ESP32 Development Board (DOIT ESP32 DEVKIT V1 or similar)  
✅ 0.96" OLED Display (I2C, 4-pin)  
✅ RGB LED (Common Cathode)  
✅ Passive Buzzer  
✅ Photoresistor Module (4-pin with AO)  
✅ Push Button  
✅ 3x 220Ω Resistors  
✅ Breadboard  
✅ Jumper Wires  
✅ Micro-USB Cable  
✅ Computer with internet

## Step 1: Install Software (10 minutes)

### Arduino IDE
1. Download from: https://www.arduino.cc/en/software
2. Install and open Arduino IDE

### ESP32 Support
1. Go to `File` → `Preferences`
2. Add to "Additional boards manager URLs":
   ```
   https://dl.espressif.com/dl/package_esp32_index.json
   ```
3. Go to `Tools` → `Board` → `Boards Manager`
4. Search "esp32" and install "esp32 by Espressif Systems"

### Install Libraries
1. Go to `Sketch` → `Include Library` → `Manage Libraries`
2. Install these three libraries:
   - **Adafruit SSD1306** (click "Install All" for dependencies)
   - **Adafruit GFX Library**
   - **ArduinoJson** (version 6.x)

## Step 2: Wire Everything (15 minutes)

### Power Rails First
```
ESP32 3.3V  →  Breadboard Red Rail (+)
ESP32 GND   →  Breadboard Blue Rail (-)
```

### Components

**OLED Display:**
```
GND → Blue Rail (-)
VCC → Red Rail (+)
SCL → GPIO 22
SDA → GPIO 21
```

**RGB LED (with 220Ω resistors!):**
```
Long Leg (-)     → Blue Rail (-)
Red Leg          → 220Ω → GPIO 15
Green Leg        → 220Ω → GPIO 2
Blue Leg         → 220Ω → GPIO 4
```

**Passive Buzzer:**
```
- (Short)  → Blue Rail (-)
+ (Long)   → GPIO 13
```

**Photoresistor Module:**
```
GND → Blue Rail (-)
VCC → Red Rail (+)
AO  → GPIO 34
DO  → (leave empty)
```

**Push Button:**
```
Leg 1 (diagonal)  → GPIO 23
Leg 2 (diagonal)  → Blue Rail (-)
```

## Step 3: Upload Code (5 minutes)

### Download Code
1. Download this repository (green "Code" button → Download ZIP)
2. Extract the ZIP file
3. Navigate to: `src/bitcoin_ticker_final/`
4. Open `bitcoin_ticker_final.ino` in Arduino IDE

### Configure WiFi
Find these lines and edit:
```cpp
const char* ssid = "YOUR_WIFI_NAME";       
const char* password = "YOUR_WIFI_PASSWORD";
```

**Important:**
- Use your 2.4GHz WiFi network (ESP32 doesn't support 5GHz)
- SSID and password are case-sensitive
- Keep the quotes `""`

### Upload to ESP32
1. Connect ESP32 to computer via USB
2. Select board: `Tools` → `Board` → `ESP32 Arduino` → `DOIT ESP32 DEVKIT V1`
3. Select port: `Tools` → `Port` → (Choose COM port or /dev/ttyUSB*)
4. Click Upload (→ arrow button)
5. **If stuck on "Connecting..."**: Hold the BOOT button on ESP32 until upload starts

## Step 4: Test It! (2 minutes)

### Open Serial Monitor
1. Click Serial Monitor icon (magnifying glass)
2. Set baud rate to **115200** (bottom-right dropdown)

### You Should See:
```
=================================
Bitcoin Price Tracker v1.0
=================================

Display initialized successfully
Connecting to WiFi: YourWiFi
..........
WiFi Connected!
IP Address: 192.168.1.100

--- Fetching Bitcoin Price ---
HTTP Response code: 200
Current Price: $42,567.89
```

### Check Display
The OLED should show:
```
BITCOIN TRACKER    SOUND
═══════════════════════
$42,567.89

+$125.34
```

### LED Should Light:
- 🔵 Blue = Initializing (first run)
- 🟢 Green = Price went up
- 🔴 Red = Price went down

### Buzzer Should Play:
- Happy tone = Price increased
- Sad tone = Price decreased
- (Silent if room is dark - Smart Mute feature!)

## Step 5: Calibrate Light Sensor (Optional)

Watch Serial Monitor for:
```
Current Light Level: 2450
```

**To adjust mute threshold:**
1. Cover sensor with hand (should show ~500 or less)
2. Note the value in normal room lighting
3. Edit this line in code:
   ```cpp
   const int LIGHT_THRESHOLD = 1000;  // Adjust as needed
   ```
4. Upload again

## That's It! 🎉

Your Bitcoin Price Tracker is now running!

## Quick Tips

### Manual Refresh
Press the button anytime for instant price update

### Auto Updates
Device automatically fetches new price every 60 seconds

### Smart Mute
Buzzer auto-mutes in darkness (based on light sensor)

### Adjusting Update Frequency
Change this value (in milliseconds):
```cpp
unsigned long timerDelay = 60000;  // 60 seconds
```

## Troubleshooting

### Display Blank
- Try I2C address `0x3D` instead of `0x3C`
- Check SDA/SCL wiring (GPIO 21/22)
- Verify power connections

### WiFi Won't Connect
- Check SSID/password spelling
- Must be 2.4GHz network (not 5GHz)
- Move closer to router

### No Sound
- Using Passive buzzer? (green PCB, not sealed black)
- Room bright enough? (check Serial Monitor for light level)
- Try covering photoresistor

### LED Not Working
- Check 220Ω resistors are installed
- Verify Common Cathode LED (long leg to GND)
- Try different LED pins in code

### Upload Fails
- Hold BOOT button when "Connecting..." appears
- Try different USB cable (must support data)
- Check COM port selection

## Full Documentation

Need more help?
- **Complete Guide**: [README.md](README.md)
- **Detailed Wiring**: [WIRING.md](WIRING.md)
- **Setup Help**: [docs/SETUP.md](docs/SETUP.md)
- **Troubleshooting**: [docs/TROUBLESHOOTING.md](docs/TROUBLESHOOTING.md)

## Pin Reference Card

Quick reminder of all connections:

| Component | Pin | ESP32 GPIO |
|-----------|-----|------------|
| OLED SDA | SDA | 21 |
| OLED SCL | SCL | 22 |
| RGB Red | R | 15 (via 220Ω) |
| RGB Green | G | 2 (via 220Ω) |
| RGB Blue | B | 4 (via 220Ω) |
| Buzzer | S | 13 |
| Light Sensor | AO | 34 |
| Button | Leg | 23 |
| Power | VCC | 3.3V |
| Ground | GND | GND |

---

**Enjoy tracking Bitcoin in real-time!** 🚀💰

*Questions? Open an issue on GitHub!*