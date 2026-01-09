# Bitcoin Price Tracker - ESP32 IoT Project

A real-time Bitcoin price monitoring system built with ESP32, featuring visual and audio alerts, automatic night-mode muting, and instant refresh capability.

![Project Status](https://img.shields.io/badge/status-active-success.svg)
![License](https://img.shields.io/badge/license-MIT-blue.svg)

## 🌟 Features

- **Real-Time Price Display**: Fetches current Bitcoin price from CoinDesk API every 60 seconds
- **Visual Trend Indicators**: 
  - RGB LED shows green (price up), red (price down), or blue (stable/initializing)
  - OLED display shows current price and trend direction
- **Audio Alerts**: 
  - High-pitched "ding" when price increases
  - Low-pitched "womp-womp" when price decreases
- **Smart Mute**: Automatically disables sound in dark environments using photoresistor
- **Instant Refresh**: Manual button press for immediate price update
- **Price Change Display**: Shows the exact dollar amount change from last update

## 📦 Hardware Requirements

### Core Components
- **ESP32 Development Board** (DOIT ESP32 DEVKIT V1 or similar)
- **0.96" OLED Display** (I2C, SSD1306, 128x64)
- **830-Point Breadboard**
- **Micro-USB Cable**

### Electronic Components
- **RGB LED** (Common Cathode)
- **Passive Buzzer**
- **Photoresistor Module** (4-pin with AO output)
- **Tactile Push Button** (with optional keycap)
- **3x 220Ω Resistors** (for RGB LED)
- **Male-to-Male Jumper Wires**

> **Note**: Most of these components are included in standard ESP32 starter kits.

## 🔧 Software Requirements

### Arduino IDE Setup

1. **Install Arduino IDE** (1.8.x or 2.x)
   - Download from [arduino.cc](https://www.arduino.cc/en/software)

2. **Install ESP32 Board Support**
   - Open Arduino IDE
   - Go to `File` → `Preferences`
   - Add to "Additional Board Manager URLs":
     ```
     https://dl.espressif.com/dl/package_esp32_index.json
     ```
   - Go to `Tools` → `Board` → `Boards Manager`
   - Search for "ESP32" and install "esp32 by Espressif Systems"

3. **Install Required Libraries**
   - Go to `Sketch` → `Include Library` → `Manage Libraries`
   - Install the following:
     - **Adafruit SSD1306** (by Adafruit)
     - **Adafruit GFX Library** (by Adafruit)
     - **ArduinoJson** (by Benoit Blanchon)

### Board Configuration

- **Board**: Tools → Board → ESP32 Arduino → DOIT ESP32 DEVKIT V1
- **Port**: Select the COM/Serial port that appears when ESP32 is connected
- **Upload Speed**: 115200

## 🔌 Wiring Guide

### Power Rails Setup
```
ESP32 3.3V  →  Breadboard Red Rail (+)
ESP32 GND   →  Breadboard Blue Rail (-)
```

### Component Connections

| Component | Pin | ESP32 GPIO | Notes |
|-----------|-----|------------|-------|
| **OLED Display** |
| VCC | → | 3.3V (Red Rail) | Power |
| GND | → | GND (Blue Rail) | Ground |
| SCL | → | GPIO 22 | I2C Clock |
| SDA | → | GPIO 21 | I2C Data |
| **RGB LED** |
| Long Leg (-) | → | GND (Blue Rail) | Common Cathode |
| Red Leg | → | GPIO 15 | Via 220Ω resistor |
| Green Leg | → | GPIO 2 | Via 220Ω resistor |
| Blue Leg | → | GPIO 4 | Via 220Ω resistor |
| **Passive Buzzer** |
| - (Short) | → | GND (Blue Rail) | Ground |
| S (Long) | → | GPIO 13 | Signal |
| **Photoresistor Module** |
| GND | → | GND (Blue Rail) | Ground |
| VCC | → | 3.3V (Red Rail) | Power |
| AO | → | GPIO 34 | Analog Output |
| DO | → | Not Connected | Leave empty |
| **Push Button** |
| Leg 1 | → | GPIO 23 | Signal |
| Leg 2 (diagonal) | → | GND (Blue Rail) | Ground |

> **Important**: Always use 220Ω resistors with the RGB LED to prevent damage!

See [WIRING.md](WIRING.md) for detailed wiring diagrams and photos.

## 🚀 Quick Start

### 1. Assemble Hardware
Follow the wiring guide above to connect all components.

### 2. Configure WiFi
Open the sketch and update these lines with your WiFi credentials:
```cpp
const char* ssid = "YOUR_WIFI_NAME";       // <--- CHANGE THIS
const char* password = "YOUR_WIFI_PASSWORD"; // <--- CHANGE THIS
```

### 3. Calibrate Light Sensor
The light threshold determines when the buzzer mutes in darkness.

1. Upload the code
2. Open Serial Monitor (Tools → Serial Monitor, set to 115200 baud)
3. Observe the "Current Light Level" values
4. Cover the sensor (simulate darkness) and note the value
5. Adjust the threshold in code if needed:
```cpp
const int LIGHT_THRESHOLD = 1000; // Adjust based on your environment
```

### 4. Upload and Run
1. Connect ESP32 via USB
2. Select correct Board and Port in Arduino IDE
3. Click Upload
4. If upload fails at "Connecting...", hold the BOOT button on ESP32

## 📱 Usage

### Normal Operation
- Device auto-updates every 60 seconds
- Green LED + happy tone = Price increased
- Red LED + sad tone = Price decreased  
- Blue LED = No change or initializing
- Display shows current price and change amount

### Manual Refresh
Press the button for instant price update (resets 60-second timer)

### Night Mode
Buzzer automatically mutes when room is dark (based on light threshold)

## 📂 Project Structure

```
projects_btc_ticker/
├── README.md                          # This file
├── WIRING.md                          # Detailed wiring diagrams
├── BOM.md                             # Bill of Materials
├── docs/
│   ├── SETUP.md                       # Detailed setup instructions
│   └── TROUBLESHOOTING.md             # Common issues and solutions
├── src/
│   ├── bitcoin_ticker_v1_basic/       # Basic version (display only)
│   ├── bitcoin_ticker_v2_led/         # Added RGB LED
│   ├── bitcoin_ticker_v3_sound/       # Added buzzer
│   └── bitcoin_ticker_final/          # Complete version with all features
└── images/                            # Photos and diagrams
```

## 🎯 Progressive Build Versions

This project is designed to be built in stages:

1. **v1_basic**: WiFi + OLED display only
2. **v2_led**: Adds RGB LED visual indicators
3. **v3_sound**: Adds passive buzzer audio alerts
4. **v4_final**: Adds photoresistor + button (complete system)

Each version builds upon the previous, allowing you to test as you go.

## 🛠️ Troubleshooting

### Display Not Working
- Verify I2C address (usually 0x3C, sometimes 0x3D)
- Check SDA/SCL are on GPIO 21/22
- Ensure display is getting 3.3V power

### No Sound from Buzzer
- Confirm you're using the **Passive** buzzer (green PCB), not Active
- Check GPIO 13 connection
- Verify room is bright enough (not muted by light sensor)

### LED Issues
- Ensure 220Ω resistors are used on all three color legs
- Verify Common Cathode type (long leg to GND)
- If LED is always on or reversed, you may have Common Anode

### WiFi Connection Fails
- Double-check SSID and password (case-sensitive)
- Ensure 2.4GHz WiFi (ESP32 doesn't support 5GHz)
- Check router isn't blocking new devices

### Button Not Responding
- Verify INPUT_PULLUP is enabled in code
- Check diagonal legs are connected (not same-side legs)
- Add delay(200) for debouncing if multiple triggers occur

See [TROUBLESHOOTING.md](docs/TROUBLESHOOTING.md) for more detailed solutions.

## 📊 API Information

This project uses the free **CoinDesk Bitcoin Price Index API**:
- Endpoint: `https://api.coindesk.com/v1/bpi/currentprice/USD.json`
- No API key required
- Rate limit: Reasonable for personal use (we query once per minute)
- Documentation: [CoinDesk API](https://www.coindesk.com/coindesk-api)

## 🔋 Future Enhancements

- [ ] Battery power with LiPo + TP4056 charging module
- [ ] 3D-printed enclosure/stand
- [ ] Historical price graph on display
- [ ] Multiple cryptocurrency support
- [ ] Configurable price alerts (notify at specific thresholds)
- [ ] Web interface for configuration
- [ ] MQTT integration for home automation
- [ ] E-ink display for lower power consumption

## 🎓 Learning Outcomes

This project teaches:
- **WiFi connectivity** with ESP32
- **RESTful API** consumption
- **JSON parsing** with ArduinoJson
- **I2C communication** (OLED)
- **Analog input** reading (photoresistor)
- **Digital I/O** (LED, button, buzzer)
- **PWM tone generation**
- **Non-blocking code** patterns (millis() instead of delay())
- **Pull-up resistors** for buttons
- **State management** in embedded systems

## 📄 License

This project is licensed under the MIT License - see the LICENSE file for details.

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## 💡 Credits

- CoinDesk API for free Bitcoin price data
- Adafruit for excellent display libraries
- ESP32 community for comprehensive documentation

## 📞 Support

If you encounter issues:
1. Check the [TROUBLESHOOTING.md](docs/TROUBLESHOOTING.md) guide
2. Open an issue on GitHub
3. Include Serial Monitor output and photos of your wiring

---

**Built with ❤️ for IoT enthusiasts and cryptocurrency fans**

*This is an excellent "Hello World" project for Internet of Things (IoT) development!*