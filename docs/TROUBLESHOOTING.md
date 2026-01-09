# Troubleshooting Guide - Bitcoin Price Tracker

Complete guide for diagnosing and fixing common issues with your Bitcoin Price Tracker.

## Table of Contents

1. [Hardware Issues](#hardware-issues)
2. [Display Problems](#display-problems)
3. [WiFi & Network Issues](#wifi--network-issues)
4. [LED Issues](#led-issues)
5. [Sound/Buzzer Problems](#soundbuzzer-problems)
6. [Sensor Issues](#sensor-issues)
7. [Button Problems](#button-problems)
8. [Software/Code Issues](#softwarecode-issues)
9. [Power Issues](#power-issues)
10. [Diagnostic Tools](#diagnostic-tools)

---

## Hardware Issues

### ESP32 Not Recognized by Computer

**Symptoms:**
- No COM port appears in Arduino IDE
- "Device not recognized" error in Windows
- No /dev/ttyUSB* in Linux

**Solutions:**

1. **Check USB Cable:**
   - ❌ Many cables are charge-only (no data lines)
   - ✅ Try a different cable known to work for data
   - Test: If cable works with phone data transfer, it should work

2. **Install/Reinstall Drivers:**
   - **CP2102 chip:** [Silicon Labs Driver](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)
   - **CH340 chip:** [CH340 Driver](http://www.wch.cn/downloads/CH341SER_ZIP.html)
   - Restart computer after installation

3. **Try Different USB Port:**
   - Use USB 2.0 port (not 3.0/3.1 if having issues)
   - Avoid USB hubs - connect directly to computer

4. **Check for Physical Damage:**
   - Inspect USB port on ESP32 for bent pins
   - Look for cracked solder joints

5. **Linux Permissions:**
   ```bash
   sudo usermod -a -G dialout $USER
   sudo usermod -a -G tty $USER
   # Logout and login for changes to take effect
   ```

---

### ESP32 Gets Hot

**Symptoms:**
- ESP32 warm/hot to touch
- Unexpected shutdowns
- Unstable operation

**Causes & Solutions:**

1. **Normal Operation:**
   - ESP32 running at 240MHz generates heat
   - Slightly warm is normal
   - Very hot = problem!

2. **Short Circuit:**
   - ❌ Check for accidental 3.3V to GND connection
   - ❌ Verify no component legs are touching
   - ✅ Disconnect all wiring and test bare ESP32

3. **Excessive Current Draw:**
   - Check RGB LED resistors are installed
   - Verify no components drawing too much power
   - Measure current: should be <200mA total

4. **Solutions:**
   - Add small heatsink if needed
   - Reduce CPU frequency to 160MHz in code
   - Ensure good ventilation

---

### Breadboard Connections Intermittent

**Symptoms:**
- Works, then stops working
- Wiggly wires cause resets
- Random errors

**Solutions:**

1. **Improve Connections:**
   - Push wires firmly into breadboard
   - Bend wire ends slightly for better grip
   - Replace worn breadboard if very old

2. **Check Wire Quality:**
   - Solid core wire works better than stranded in breadboards
   - Ensure wire gauge is 22-24 AWG
   - Replace oxidized/bent wires

3. **Power Rail Issues:**
   - Some breadboards have disconnected power rails
   - Bridge top/bottom rails with jumper wires
   - Test continuity with multimeter

---

## Display Problems

### OLED Display Blank/Not Working

**Symptoms:**
- Display stays black
- No text appears
- Backlight may or may not be visible

**Diagnostic Steps:**

1. **Check Power:**
   ```cpp
   // Add to setup() to test
   Serial.println("Display test starting...");
   if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
     Serial.println("Display FAILED!");
   } else {
     Serial.println("Display OK!");
   }
   ```

2. **Try Different I2C Address:**
   - Change from `0x3C` to `0x3D`:
   ```cpp
   #define SCREEN_ADDRESS 0x3D  // Instead of 0x3C
   ```

3. **Run I2C Scanner:**
   ```cpp
   #include <Wire.h>
   
   void setup() {
     Serial.begin(115200);
     Wire.begin(21, 22);
     Serial.println("I2C Scanner");
   }
   
   void loop() {
     byte error, address;
     int nDevices = 0;
     
     for(address = 1; address < 127; address++) {
       Wire.beginTransmission(address);
       error = Wire.endTransmission();
       
       if (error == 0) {
         Serial.print("Device found at 0x");
         if (address < 16) Serial.print("0");
         Serial.println(address, HEX);
         nDevices++;
       }
     }
     
     if (nDevices == 0)
       Serial.println("No I2C devices found");
     
     delay(5000);
   }
   ```

4. **Check Wiring:**
   - SDA must be on GPIO 21
   - SCL must be on GPIO 22
   - VCC to 3.3V (not GND!)
   - GND to GND (not 3.3V!)

5. **Verify Library:**
   - Ensure Adafruit SSD1306 library installed
   - Version should be 2.x or newer
   - Reinstall if necessary

**Still Not Working:**
- Try different OLED display (may be defective)
- Test OLED on another ESP32
- Check for physical damage to display

---

### OLED Display Flickers or Has Artifacts

**Symptoms:**
- Random pixels appearing
- Flickering screen
- Partial display

**Solutions:**

1. **Power Issues:**
   - Add 100µF capacitor between 3.3V and GND near display
   - Use shorter wires (reduce resistance)
   - Check breadboard power rail continuity

2. **I2C Bus Issues:**
   - Add pull-up resistors (4.7kΩ) on SDA and SCL lines
   - Reduce wire length
   - Keep I2C wires away from power wires

3. **Code Issues:**
   - Call `display.display()` only once per update
   - Don't update display too frequently
   - Clear display before redrawing

---

### Display Text Too Small/Large

**Solution:**
```cpp
display.setTextSize(1);  // Small (default)
display.setTextSize(2);  // Medium (2x larger)
display.setTextSize(3);  // Large (3x larger)
```

---

## WiFi & Network Issues

### WiFi Won't Connect

**Symptoms:**
- Stuck on "Connecting..."
- Serial shows multiple dots but never connects
- Connection timeout

**Diagnostic Code:**
```cpp
WiFi.begin(ssid, password);
int attempts = 0;
while (WiFi.status() != WL_CONNECTED && attempts < 40) {
  delay(500);
  Serial.print(".");
  Serial.print(" Status: ");
  Serial.println(WiFi.status()); // Shows WiFi status code
  attempts++;
}

if (WiFi.status() == WL_CONNECTED) {
  Serial.println("\nConnected!");
} else {
  Serial.print("\nFailed! Status code: ");
  Serial.println(WiFi.status());
}
```

**WiFi Status Codes:**
- `0` = WL_IDLE_STATUS (idle)
- `1` = WL_NO_SSID_AVAIL (SSID not found)
- `3` = WL_CONNECTED (success!)
- `4` = WL_CONNECT_FAILED (wrong password)
- `6` = WL_DISCONNECTED (connection lost)

**Solutions by Status Code:**

**Status 1 (SSID Not Found):**
- Verify SSID spelling (case-sensitive!)
- Check quotes: `"MyWiFi"` not `MyWiFi`
- Ensure 2.4GHz network (ESP32 can't see 5GHz)
- Move ESP32 closer to router

**Status 4 (Wrong Password):**
- Check password carefully (case-sensitive)
- No extra spaces before/after password
- Special characters may cause issues
- Try simple password for testing

**Status 6 (Disconnected):**
- Router may be blocking device
- Check router MAC address filtering
- Try static IP instead of DHCP
- Router firmware may need update

**General Solutions:**

1. **Test WiFi Credentials:**
   - Connect phone/laptop with same SSID/password
   - Verify 2.4GHz band is enabled on router
   - Check router isn't in guest/isolation mode

2. **Simplify for Testing:**
   ```cpp
   const char* ssid = "TestNetwork";  // Simple name
   const char* password = "12345678";  // Simple password (8+ chars)
   ```

3. **Check Router Settings:**
   - Disable AP isolation
   - Enable SSID broadcast
   - Security: WPA2-PSK (not Enterprise/WEP)
   - Channel: Try 1, 6, or 11

4. **Add WiFi Debug:**
   ```cpp
   WiFi.setAutoReconnect(true);
   WiFi.persistent(true);
   Serial.print("Connecting to: ");
   Serial.println(ssid);
   ```

---

### API Request Fails

**Symptoms:**
- HTTP error codes (not 200)
- "API Error" on display
- No price data received

**Solutions:**

1. **Check HTTP Response Code:**
   ```cpp
   Serial.print("HTTP Response: ");
   Serial.println(httpResponseCode);
   ```

   **Common Codes:**
   - `200` = Success ✅
   - `-1` = Connection failed
   - `403` = Forbidden (rate limited)
   - `404` = URL not found
   - `429` = Too many requests
   - `500+` = Server error

2. **Verify API Endpoint:**
   ```cpp
   const char* endpoint = "https://api.coindesk.com/v1/bpi/currentprice/USD.json";
   // Must be HTTPS, not HTTP
   ```

3. **Test API in Browser:**
   - Visit: https://api.coindesk.com/v1/bpi/currentprice/USD.json
   - Should show JSON data
   - If not, API may be down

4. **Rate Limiting:**
   - Don't update more than once per minute
   - Current code uses 60 second delay ✅
   - Reduce if testing: minimum 10 seconds

5. **SSL Certificate Issues:**
   - CoinDesk API should work without certificates
   - If issues, add certificate bundle
   - Or switch to HTTP API (less secure)

---

## LED Issues

### RGB LED Not Lighting Up

**Symptoms:**
- All colors off
- No response from LED

**Solutions:**

1. **Check Resistors:**
   - ❌ WITHOUT resistors, LED may burn out instantly
   - ✅ Use 220Ω on EACH color leg
   - Too high resistance (>1kΩ) = very dim

2. **Verify LED Type:**
   - Common Cathode: Long leg to GND ✅
   - Common Anode: Long leg to 3.3V (reverse code logic)

3. **Test LED Manually:**
   ```cpp
   void setup() {
     pinMode(15, OUTPUT); // Red
     pinMode(2, OUTPUT);  // Green
     pinMode(4, OUTPUT);  // Blue
     
     digitalWrite(15, HIGH); delay(1000);
     digitalWrite(2, HIGH); delay(1000);
     digitalWrite(4, HIGH); delay(1000);
   }
   ```

4. **Identify LED Pins:**
   - Long leg = Common (GND for cathode)
   - Other 3 legs = R, G, B (order varies!)
   - Test each leg individually to find colors

5. **Check for Burnout:**
   - LED may be dead if connected without resistors
   - Try new LED
   - Verify with multimeter (should have ~2V forward voltage)

---

### LED Wrong Colors

**Symptoms:**
- Red shows as green
- Colors mixed up
- Only some colors work

**Solutions:**

1. **Pin Identification Wrong:**
   - Swap GPIO assignments in code
   - Test each color individually
   - Update pin definitions:
   ```cpp
   #define PIN_RED   4   // Try different GPIO
   #define PIN_GREEN 15
   #define PIN_BLUE  2
   ```

2. **Common Anode Instead of Cathode:**
   - Invert logic in code:
   ```cpp
   // For Common Anode, use LOW to turn ON
   digitalWrite(PIN_RED, LOW);  // ON
   digitalWrite(PIN_RED, HIGH); // OFF
   ```

---

### LED Too Dim or Too Bright

**Solutions:**

**Too Dim:**
- Lower resistor value (150Ω instead of 220Ω)
- Check breadboard connections
- Verify 3.3V power supply is stable

**Too Bright:**
- Higher resistor value (330Ω or 470Ω)
- Add PWM control for brightness

---

## Sound/Buzzer Problems

### No Sound from Buzzer

**Symptoms:**
- Complete silence
- No tone when price changes

**Solutions:**

1. **Verify Passive vs Active:**
   - Look at bottom: Green PCB = Passive ✅
   - Black sealed = Active ❌ (won't work with tone())
   
2. **Check Smart Mute:**
   - Open Serial Monitor
   - Look for: "Current Light Level: XXX"
   - If below threshold, buzzer is muted!
   - Cover photoresistor to test

3. **Test Buzzer Directly:**
   ```cpp
   void setup() {
     pinMode(13, OUTPUT);
   }
   
   void loop() {
     tone(13, 1000, 500);  // 1000Hz for 500ms
     delay(1000);
   }
   ```

4. **Check Polarity:**
   - Try flipping buzzer pins
   - Some buzzers are polarity-sensitive

5. **Volume Too Low:**
   - Move buzzer closer to ear
   - Passive buzzers can be quiet
   - Try different frequencies (500-4000Hz)

---

### Clicking Sound Instead of Tone

**Symptoms:**
- Rapid clicking/buzzing
- No clear tone

**Cause:**
- Using Active buzzer instead of Passive

**Solution:**
- Replace with Passive buzzer
- OR simplify code for Active:
```cpp
digitalWrite(PIN_BUZZER, HIGH);
delay(200);
digitalWrite(PIN_BUZZER, LOW);
```

---

### Sound Always Muted

**Symptoms:**
- Serial Monitor shows "(Muted due to darkness)"
- Even in bright room

**Solutions:**

1. **Check Light Sensor:**
   - Open Serial Monitor
   - Look for light level readings
   - Should be >1000 in bright room

2. **Adjust Threshold:**
   ```cpp
   const int LIGHT_THRESHOLD = 500;  // Lower = easier to activate
   ```

3. **Bypass for Testing:**
   ```cpp
   // Temporarily disable smart mute
   bool isEnvironmentBright() {
     return true;  // Always return true
   }
   ```

---

## Sensor Issues

### Photoresistor Not Responding

**Symptoms:**
- Light value always same
- Doesn't change when covered

**Solutions:**

1. **Check Pin Connection:**
   - Must use AO (Analog Output), not DO
   - Must connect to GPIO 34 (ADC pin)

2. **Test Sensor:**
   ```cpp
   void loop() {
     int light = analogRead(34);
     Serial.print("Light: ");
     Serial.println(light);
     delay(500);
   }
   ```

3. **Expected Values:**
   - Bright room: 2000-4095
   - Normal room: 500-2000
   - Dark: 0-500
   - Hand over sensor: Should drop significantly

4. **Module Power:**
   - VCC to 3.3V
   - GND to GND
   - Check module has LED indicator (should light up)

---

## Button Problems

### Button Not Responding

**Symptoms:**
- Pressing button does nothing
- No "REFRESHING" message

**Solutions:**

1. **Check INPUT_PULLUP:**
   ```cpp
   pinMode(PIN_BUTTON, INPUT_PULLUP);  // Essential!
   ```

2. **Test Button:**
   ```cpp
   void loop() {
     if (digitalRead(23) == LOW) {
       Serial.println("PRESSED!");
     } else {
       Serial.println("Not pressed");
     }
     delay(100);
   }
   ```

3. **Verify Wiring:**
   - Use DIAGONAL legs (not same-side)
   - One leg to GPIO 23
   - Other leg to GND
   - No resistor needed (internal pull-up)

4. **Button Orientation:**
   - Rotate button 90° and try again
   - Some buttons have specific orientation

---

### Button Always Reads as Pressed

**Symptoms:**
- Constant "REFRESHING"
- Button stuck
- Never reads HIGH

**Solutions:**

1. **Check Pull-up:**
   - Must use `INPUT_PULLUP` mode
   - NOT just `INPUT`

2. **Wrong Legs:**
   - May be using same-side legs
   - Switch to diagonal legs

3. **Short Circuit:**
   - GPIO 23 may be shorted to GND elsewhere
   - Check for wire crossings

---

### Button Triggers Multiple Times

**Symptoms:**
- One press causes multiple refreshes
- Bouncing

**Solution:**
```cpp
if (digitalRead(PIN_BUTTON) == LOW) {
  delay(50);  // Debounce delay
  if (digitalRead(PIN_BUTTON) == LOW) {  // Check again
    // Button truly pressed
    updateData();
    while(digitalRead(PIN_BUTTON) == LOW) delay(10);  // Wait for release
  }
}
```

---

## Software/Code Issues

### Compilation Errors

**Error:** `'Adafruit_SSD1306' does not name a type`
- **Fix:** Install Adafruit SSD1306 library

**Error:** `'HTTPClient' was not declared`
- **Fix:** Ensure ESP32 board support installed
- Check: Tools → Board → ESP32 Arduino

**Error:** `'deserializeJson' was not declared`
- **Fix:** Install ArduinoJson library (v6.x)

**Error:** `multiple definition of 'setup'`
- **Fix:** Only open .ino file, not .h or .cpp files

---

### Upload Fails

**Error:** "Timed out waiting for packet header"
- **Fix:** Hold BOOT button during upload

**Error:** "Serial port not found"
- **Fix:** Check COM port selection
- Reconnect ESP32

**Error:** "Sketch too big"
- **Fix:** Change partition scheme
- Tools → Partition Scheme → "Huge APP"

---

### Runtime Crashes

**Symptoms:**
- ESP32 resets randomly
- "Brownout detector triggered"
- Watchdog timer reset

**Solutions:**

1. **Power Issues:**
   - Use good quality USB cable
   - Try different power source
   - Add 100µF capacitor near ESP32

2. **Memory Issues:**
   - Check for infinite loops
   - Reduce JSON buffer size if tight on RAM

3. **Watchdog Timer:**
   ```cpp
   // Add to long operations
   yield();  // or delay(1);
   ```

---

## Power Issues

### ESP32 Keeps Resetting

**Symptoms:**
- Boot loop
- Constant restarts
- "Brownout" messages

**Solutions:**

1. **Insufficient Power:**
   - USB ports provide 500mA max
   - Use powered USB hub
   - Or 5V 1A wall adapter

2. **Check Current Draw:**
   - RGB LED: ~60mA (20mA × 3)
   - OLED: ~30mA
   - ESP32: ~100-200mA
   - Total: ~200-300mA (should be fine)

3. **Add Capacitor:**
   - 100µF electrolytic capacitor
   - Connect between 3.3V and GND
   - Near ESP32 power pins

---

## Diagnostic Tools

### Serial Monitor Debug Template

Add this to troubleshoot:

```cpp
void printDiagnostics() {
  Serial.println("\n=== DIAGNOSTICS ===");
  Serial.print("WiFi Status: ");
  Serial.println(WiFi.status());
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Signal Strength: ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
  Serial.print("Free Heap: ");
  Serial.print(ESP.getFreeHeap());
  Serial.println(" bytes");
  Serial.print("Light Level: ");
  Serial.println(analogRead(34));
  Serial.print("Button State: ");
  Serial.println(digitalRead(23));
  Serial.println("==================\n");
}
```

Call in loop: `printDiagnostics();`

---

### Component Test Sketch

```cpp
void setup() {
  Serial.begin(115200);
  pinMode(15, OUTPUT); pinMode(2, OUTPUT); pinMode(4, OUTPUT);
  pinMode(13, OUTPUT); pinMode(23, INPUT_PULLUP);
}

void loop() {
  Serial.println("\n--- Component Test ---");
  
  // Test LEDs
  Serial.println("Testing RED LED...");
  digitalWrite(15, HIGH); delay(500); digitalWrite(15, LOW);
  
  Serial.println("Testing GREEN LED...");
  digitalWrite(2, HIGH); delay(500); digitalWrite(2, LOW);
  
  Serial.println("Testing BLUE LED...");
  digitalWrite(4, HIGH); delay(500); digitalWrite(4, LOW);
  
  // Test Buzzer
  Serial.println("Testing Buzzer...");
  tone(13, 1000, 300); delay(500);
  
  // Test Sensor
  Serial.print("Light Level: ");
  Serial.println(analogRead(34));
  
  // Test Button
  Serial.print("Button: ");
  Serial.println(digitalRead(23) == LOW ? "PRESSED" : "Not Pressed");
  
  delay(2000);
}
```

---

## Getting Help

If you're still stuck after trying these solutions:

1. **Open GitHub Issue:**
   - Include Serial Monitor output
   - Describe what you've tried
   - Photos of wiring

2. **Provide Information:**
   - ESP32 board model
   - Component details
   - Error messages (exact text)
   - Arduino IDE version

3. **Check Common Issues:**
   - 90% of problems are wiring or WiFi
   - Double-check connections
   - Test components individually

---

**Good luck debugging!** 🔧

*Most issues are simple wiring mistakes - take your time and check systematically.*