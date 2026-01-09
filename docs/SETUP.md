# Complete Setup Guide - Bitcoin Price Tracker

This guide will walk you through the complete setup process from scratch, including installing all necessary software and configuring your development environment.

## Table of Contents

1. [Prerequisites](#prerequisites)
2. [Software Installation](#software-installation)
3. [Arduino IDE Configuration](#arduino-ide-configuration)
4. [Library Installation](#library-installation)
5. [Board Configuration](#board-configuration)
6. [First Upload Test](#first-upload-test)
7. [WiFi Configuration](#wifi-configuration)
8. [Troubleshooting Setup Issues](#troubleshooting-setup-issues)

---

## Prerequisites

Before starting, ensure you have:

- ✅ Windows, macOS, or Linux computer
- ✅ USB port (USB-A or USB-C with adapter)
- ✅ Internet connection (for downloading software and libraries)
- ✅ All hardware components (see [BOM.md](../BOM.md))
- ✅ Basic understanding of how to extract ZIP files

**Estimated Setup Time:** 30-45 minutes (first time)

---

## Software Installation

### Step 1: Install Arduino IDE

Arduino IDE is the software used to write and upload code to the ESP32.

#### Option A: Arduino IDE 2.x (Recommended - Modern Interface)

1. **Download:**
   - Visit: [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software)
   - Click "Download" for Arduino IDE 2.x
   - Choose your operating system:
     - Windows: Download `.exe` installer
     - macOS: Download `.dmg` file
     - Linux: Download AppImage or `.zip`

2. **Install:**
   - **Windows:** Run the `.exe` and follow installer prompts
   - **macOS:** Open `.dmg` and drag Arduino to Applications folder
   - **Linux:** Make AppImage executable: `chmod +x arduino-ide_*.AppImage`

3. **Launch:**
   - Open Arduino IDE
   - You should see a modern dark/light theme interface

#### Option B: Arduino IDE 1.8.x (Legacy - Stable)

1. **Download:**
   - Visit: [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software)
   - Scroll to "Legacy IDE (1.8.X)"
   - Download for your OS

2. **Install:**
   - Follow similar installation steps as above

**Recommendation:** Use Arduino IDE 2.x for better performance and modern features.

---

### Step 2: Install USB Drivers (Windows/macOS)

ESP32 boards use a USB-to-Serial chip. Common chips are CP2102 or CH340.

#### Check if Drivers Are Needed:

1. Connect ESP32 to computer via USB
2. **Windows:** Open Device Manager → Look under "Ports (COM & LPT)"
3. **macOS:** Open Terminal → Type `ls /dev/cu.*`
4. **Linux:** Type `ls /dev/ttyUSB*` or `ls /dev/ttyACM*`

If you see a port (COM3, /dev/cu.usbserial, etc.), drivers are installed ✅

#### If No Port Appears:

**For CP2102 Chip:**
- Download from: [Silicon Labs CP210x Driver](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)
- Install and restart computer

**For CH340 Chip:**
- **Windows:** [CH340 Windows Driver](http://www.wch.cn/downloads/CH341SER_ZIP.html)
- **macOS:** [CH340 macOS Driver](https://github.com/adrianmihalko/ch340g-ch34g-ch34x-mac-os-x-driver)
- **Linux:** Usually built into kernel (no driver needed)

**After Installation:**
- Reconnect ESP32
- Verify port appears in Device Manager/Terminal

---

## Arduino IDE Configuration

### Step 3: Add ESP32 Board Support

The ESP32 is not included by default in Arduino IDE. We need to add it.

#### For Arduino IDE 2.x:

1. **Open Preferences:**
   - Click `File` → `Preferences` (Windows/Linux)
   - Or `Arduino IDE` → `Settings` (macOS)

2. **Add Board Manager URL:**
   - Find the field: **"Additional boards manager URLs"**
   - Paste this URL:
     ```
     https://dl.espressif.com/dl/package_esp32_index.json
     ```
   - If there are already URLs, add a comma and paste on a new line
   - Click "OK"

3. **Open Boards Manager:**
   - Click the Boards Manager icon in the left sidebar (looks like a chip)
   - Or go to `Tools` → `Board` → `Boards Manager...`

4. **Install ESP32:**
   - Search for: `esp32`
   - Find: **"esp32 by Espressif Systems"**
   - Click "Install" (latest version, usually 2.0.x or 3.0.x)
   - Wait for download and installation (may take 5-10 minutes)
   - Click "Close" when done

#### For Arduino IDE 1.8.x:

1. **Open Preferences:**
   - `File` → `Preferences`

2. **Add Board Manager URL:**
   - Same as above

3. **Open Boards Manager:**
   - `Tools` → `Board` → `Boards Manager...`
   - Search and install "esp32" as above

**Verification:**
- Go to `Tools` → `Board`
- You should now see "ESP32 Arduino" section with many ESP32 boards listed

---

## Library Installation

Our project requires three libraries for OLED display and JSON parsing.

### Step 4: Install Required Libraries

#### Method 1: Using Library Manager (Recommended)

1. **Open Library Manager:**
   - **Arduino IDE 2.x:** Click library icon (books) in left sidebar
   - **Arduino IDE 1.8.x:** `Sketch` → `Include Library` → `Manage Libraries...`

2. **Install Adafruit SSD1306:**
   - Search for: `Adafruit SSD1306`
   - Find: **"Adafruit SSD1306 by Adafruit"**
   - Click "Install"
   - If prompted "Install dependencies?", click **"Install All"** ✅

3. **Install Adafruit GFX Library:**
   - Search for: `Adafruit GFX`
   - Find: **"Adafruit GFX Library by Adafruit"**
   - Click "Install"
   - (May already be installed from step 2)

4. **Install ArduinoJson:**
   - Search for: `ArduinoJson`
   - Find: **"ArduinoJson by Benoit Blanchon"**
   - Click "Install"
   - **Important:** Use version 6.x (not 7.x beta)

**Verification:**
- Go to `Sketch` → `Include Library`
- You should see all three libraries listed

#### Method 2: Manual Installation (If Library Manager Fails)

1. Download ZIP files from GitHub:
   - [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306)
   - [Adafruit GFX](https://github.com/adafruit/Adafruit-GFX-Library)
   - [ArduinoJson](https://github.com/bblanchon/ArduinoJson)

2. Install:
   - `Sketch` → `Include Library` → `Add .ZIP Library...`
   - Select each downloaded ZIP file

---

## Board Configuration

### Step 5: Select the Correct Board and Port

Now we'll tell Arduino IDE which ESP32 board we're using.

#### Select Board:

1. **Connect ESP32 to computer via USB**

2. **Select Board:**
   - Go to `Tools` → `Board` → `ESP32 Arduino`
   - Select: **"DOIT ESP32 DEVKIT V1"**
   
   **Alternative boards (if DOIT not listed):**
   - "ESP32 Dev Module" (generic, works with most boards)
   - "NodeMCU-32S" (if you have this specific board)
   - "ESP32-WROOM-DA Module"

#### Select Port:

1. **Windows:**
   - `Tools` → `Port`
   - Select: `COM3` (or COM4, COM5, etc.)
   - The one that appears when ESP32 is connected

2. **macOS:**
   - `Tools` → `Port`
   - Select: `/dev/cu.usbserial-XXXX` or `/dev/cu.wchusbserial-XXXX`

3. **Linux:**
   - `Tools` → `Port`
   - Select: `/dev/ttyUSB0` or `/dev/ttyACM0`
   - **If permission denied:** Run `sudo usermod -a -G dialout $USER` then logout/login

#### Additional Settings (Leave as Default):

- **Upload Speed:** 921600 (or 115200 if uploads fail)
- **CPU Frequency:** 240MHz
- **Flash Frequency:** 80MHz
- **Flash Mode:** QIO
- **Flash Size:** 4MB (or match your board)
- **Partition Scheme:** Default 4MB with spiffs
- **Core Debug Level:** None

---

## First Upload Test

### Step 6: Test Your Setup with Blink Example

Before uploading the Bitcoin Tracker code, let's verify everything works.

#### Upload Blink Sketch:

1. **Open Example:**
   - `File` → `Examples` → `01.Basics` → `Blink`

2. **Modify for ESP32:**
   - Change `LED_BUILTIN` to `2`
   - Most ESP32 boards have LED on GPIO 2

   ```cpp
   void setup() {
     pinMode(2, OUTPUT);
   }
   
   void loop() {
     digitalWrite(2, HIGH);
     delay(1000);
     digitalWrite(2, LOW);
     delay(1000);
   }
   ```

3. **Upload:**
   - Click the **Upload** button (→ arrow icon)
   - Watch the bottom console for messages

4. **During Upload:**
   - You should see:
     ```
     Connecting........_____....._____
     ```
   - **If stuck on "Connecting...":**
     - Press and HOLD the **BOOT button** on ESP32
     - Keep holding until upload starts
     - Release when you see "Writing at 0x00001000..."

5. **Success:**
   - Upload completes with "Leaving..."
   - Blue LED on ESP32 should blink every second ✅

**Troubleshooting Upload Issues:** See [Troubleshooting](#troubleshooting-setup-issues) section below.

---

## WiFi Configuration

### Step 7: Configure Your WiFi Credentials

Now you're ready for the Bitcoin Tracker code!

#### Download Project Files:

1. **Clone or Download Repository:**
   ```bash
   git clone https://github.com/nuniesmith/projects_btc_ticker.git
   ```
   Or download ZIP from GitHub and extract

2. **Open Sketch:**
   - Navigate to: `src/bitcoin_ticker_final/bitcoin_ticker_final.ino`
   - Double-click to open in Arduino IDE

#### Edit WiFi Settings:

1. **Find these lines near the top:**
   ```cpp
   const char* ssid = "YOUR_WIFI_NAME";       
   const char* password = "YOUR_WIFI_PASSWORD";
   ```

2. **Replace with your WiFi:**
   ```cpp
   const char* ssid = "MyHomeWiFi";       
   const char* password = "MySecurePassword123";
   ```

   **Important Notes:**
   - SSID and password are **case-sensitive**
   - Use your 2.4GHz WiFi network (ESP32 doesn't support 5GHz)
   - Keep the quotes `""`
   - Avoid special characters if possible

3. **Save:**
   - `File` → `Save` (or Ctrl+S / Cmd+S)

---

### Step 8: Upload the Final Code

1. **Verify Wiring:**
   - Double-check all components are wired correctly
   - See [WIRING.md](../WIRING.md) for reference

2. **Compile (Verify):**
   - Click the **Checkmark** icon (Verify)
   - Ensure no errors appear
   - Warnings are usually okay

3. **Upload:**
   - Click the **Arrow** icon (Upload)
   - Wait for "Leaving..." message

4. **Open Serial Monitor:**
   - Click the **Serial Monitor** icon (magnifying glass)
   - Set baud rate to **115200** (bottom right dropdown)

5. **Watch Boot Sequence:**
   ```
   =================================
   Bitcoin Price Tracker v1.0
   =================================
   
   Display initialized successfully
   Connecting to WiFi: MyHomeWiFi
   .........
   WiFi Connected!
   IP Address: 192.168.1.100
   
   --- Fetching Bitcoin Price ---
   HTTP Response code: 200
   Current Price: $42,567.89
   Status: Initializing
   ```

6. **Check OLED Display:**
   - Should show "BITCOIN TRACKER" header
   - Current BTC price
   - Sound mute status

**Success!** Your Bitcoin Price Tracker is now running! 🎉

---

## Troubleshooting Setup Issues

### Issue: "Board at COMX is not available"

**Solutions:**
1. Check USB cable supports data (some are charge-only)
2. Try different USB port
3. Restart Arduino IDE
4. Reinstall USB drivers
5. Try different USB cable

---

### Issue: "Upload Failed" or "Timed Out"

**Solutions:**
1. **Hold BOOT button during upload:**
   - Click Upload
   - When "Connecting..." appears, press and hold BOOT
   - Release after upload starts

2. **Lower upload speed:**
   - `Tools` → `Upload Speed` → `115200`

3. **Check port:**
   - Verify correct COM/serial port selected
   - Disconnect/reconnect ESP32

4. **Reset ESP32:**
   - Press EN button (reset)
   - Try upload again

---

### Issue: "WiFi Connection Failed"

**Solutions:**
1. **Verify credentials:**
   - Check SSID spelling (case-sensitive)
   - Check password (case-sensitive)
   - Ensure no extra spaces

2. **Check WiFi network:**
   - Must be 2.4GHz (not 5GHz)
   - WPA2 security (not WEP or Enterprise)
   - ESP32 within range of router

3. **Debug in Serial Monitor:**
   - Open Serial Monitor (115200 baud)
   - Look for error messages
   - Check WiFi status codes

---

### Issue: "Compilation Error"

**Common errors and fixes:**

**Error:** `'Adafruit_SSD1306' does not name a type`
- **Fix:** Install Adafruit SSD1306 library

**Error:** `'deserializeJson' was not declared`
- **Fix:** Install ArduinoJson library (version 6.x)

**Error:** `'HTTPClient' was not declared`
- **Fix:** Ensure ESP32 board support is installed

**Error:** `Multiple libraries found for "WiFi.h"`
- **Fix:** Normal warning, ignore it

---

### Issue: "OLED Display Not Working"

**Solutions:**
1. **Check I2C address:**
   - Change `0x3C` to `0x3D` in code:
     ```cpp
     #define SCREEN_ADDRESS 0x3D
     ```

2. **Run I2C Scanner:**
   - Upload I2C scanner sketch to find address
   - Available in Arduino examples

3. **Check wiring:**
   - Verify SDA → GPIO 21
   - Verify SCL → GPIO 22
   - Check power connections

4. **Test with example:**
   - `File` → `Examples` → `Adafruit SSD1306` → `ssd1306_128x64_i2c`

---

### Issue: "Serial Monitor Shows Garbage Characters"

**Solution:**
- Set baud rate to **115200** in Serial Monitor dropdown
- Must match `Serial.begin(115200)` in code

---

### Issue: Linux Permission Denied on /dev/ttyUSB0

**Solution:**
```bash
sudo usermod -a -G dialout $USER
sudo usermod -a -G tty $USER
# Then logout and login again
```

Or for single upload:
```bash
sudo chmod a+rw /dev/ttyUSB0
```

---

## Next Steps

After successful setup:

1. ✅ **Calibrate Light Sensor:**
   - Open Serial Monitor
   - Cover/uncover photoresistor
   - Adjust `LIGHT_THRESHOLD` value if needed

2. ✅ **Test All Features:**
   - Press button for manual refresh
   - Wait 60 seconds for auto-update
   - Check LED colors change with price
   - Verify sound plays (if room is bright)

3. ✅ **Customize:**
   - Adjust update interval (change `timerDelay`)
   - Modify LED colors
   - Change sound frequencies

4. ✅ **Build Enclosure:**
   - See 3D printing suggestions in main README
   - Or create cardboard/acrylic case

---

## Quick Reference Card

### Essential Settings:
- **Board:** DOIT ESP32 DEVKIT V1
- **Port:** COM3 (Windows) or /dev/ttyUSB0 (Linux)
- **Upload Speed:** 921600 (or 115200)
- **Baud Rate:** 115200 (Serial Monitor)

### Essential Pins:
- OLED: GPIO 21 (SDA), 22 (SCL)
- RGB LED: GPIO 15 (R), 2 (G), 4 (B)
- Buzzer: GPIO 13
- Light Sensor: GPIO 34
- Button: GPIO 23

### Keyboard Shortcuts:
- **Verify:** Ctrl+R / Cmd+R
- **Upload:** Ctrl+U / Cmd+U
- **Serial Monitor:** Ctrl+Shift+M / Cmd+Shift+M
- **Save:** Ctrl+S / Cmd+S

---

## Additional Resources

- **Arduino IDE Help:** [arduino.cc/reference](https://www.arduino.cc/reference/en/)
- **ESP32 Documentation:** [docs.espressif.com](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)
- **Adafruit Learning:** [learn.adafruit.com](https://learn.adafruit.com/)
- **Project Issues:** [GitHub Issues Page](https://github.com/nuniesmith/projects_btc_ticker/issues)

---

**Setup Complete!** 🚀

You're now ready to track Bitcoin prices in real-time with your custom IoT device!

*Last Updated: January 2025*