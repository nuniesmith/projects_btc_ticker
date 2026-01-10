/*
 * Bitcoin Price Tracker - Phase 2: OLED + RGB LED
 * * ACTIVE FEATURES:
 * - Real-time Bitcoin price from Binance (HTTPS Fixed)
 * - OLED Display
 * - RGB LED Indicators (Green=Up, Red=Down, Blue=Stable)
 * * DISABLED FEATURES (Uncomment later):
 * - [LATER] Passive Buzzer (Audio alerts)
 * - [LATER] Photoresistor (Auto-mute)
 * - [LATER] Push Button (Manual Refresh)
 * * HARDWARE WIRING:
 * - OLED: SDA -> D21, SCL -> D22
 * - LED Red:   D15 (use 220 ohm resistor)
 * - LED Green: D2  (use 220 ohm resistor)
 * - LED Blue:  D4  (use 220 ohm resistor)
 * - LED Cathode: GND
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h> // Required for Binance
#include <ArduinoJson.h>

// ==================== CONFIGURATION ====================

const char* ssid = "SSID";        
const char* password = "PASS"; 

// --- PIN DEFINITIONS ---
#define PIN_RED     15   // RGB LED Red pin
#define PIN_GREEN   2    // RGB LED Green pin
#define PIN_BLUE    4    // RGB LED Blue pin

// [LATER] #define PIN_BUZZER  13   
// [LATER] #define PIN_LDR     34   
// [LATER] #define PIN_BUTTON  23   

// --- TIMING ---
unsigned long timerDelay = 15000; // Check every 15 seconds (Binance is fast)

// --- DISPLAY ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1 
#define SCREEN_ADDRESS 0x3C 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// --- API (BINANCE) ---
const char* endpoint = "https://api.binance.com/api/v3/ticker/price?symbol=BTCUSDT";

// ==================== GLOBAL VARIABLES ====================
float lastPrice = 0.0;           
unsigned long lastTime = 0;      

// ==================== HELPER FUNCTIONS ====================

/**
 * Set RGB LED color
 * Parameters: r, g, b (true = on, false = off)
 */
void setLED(bool r, bool g, bool b) {
  digitalWrite(PIN_RED, r ? HIGH : LOW);
  digitalWrite(PIN_GREEN, g ? HIGH : LOW);
  digitalWrite(PIN_BLUE, b ? HIGH : LOW);
}

// [LATER] void playSuccessTone() { ... }
// [LATER] void playFailTone() { ... }

/**
 * Main function to fetch and display Bitcoin price data
 */
void updateData() {
  if(WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi Disconnected!");
    return;
  }

  // Visual indicator on screen that we are updating
  display.fillRect(120, 0, 8, 8, WHITE); 
  display.display();

  Serial.println("\n--- Fetching Bitcoin Price ---");
  
  WiFiClientSecure client;
  client.setInsecure(); // IGNORE SSL CERTIFICATE
  
  HTTPClient http;
  http.setUserAgent("Mozilla/5.0 (ESP32)"); // PREVENT BLOCKING
  http.begin(client, endpoint);
  
  int httpResponseCode = http.GET();
  
  if (httpResponseCode > 0) {
    String payload = http.getString();
    
    // Parse JSON (Binance format)
    StaticJsonDocument<768> doc;
    DeserializationError error = deserializeJson(doc, payload);

    if (!error) {
      // Binance returns price as a String, convert to Float for math
      String priceStrRaw = doc["price"];
      float currentPrice = priceStrRaw.toFloat();
      
      // Format string for display (2 decimal places)
      char priceString[20];
      sprintf(priceString, "%.2f", currentPrice);

      Serial.print("Price: $");
      Serial.println(priceString);
      
      // ===== LED LOGIC (Active) =====
      setLED(false, false, false); // Reset

      if (lastPrice == 0.0) {
        // First run - BLUE
        setLED(false, false, true); 
        Serial.println("Status: Initializing");
      } 
      else if (currentPrice > lastPrice) {
        // Price UP - GREEN
        setLED(false, true, false);
        // [LATER] playSuccessTone();
        Serial.println("Status: UP -> GREEN");
      } 
      else if (currentPrice < lastPrice) {
        // Price DOWN - RED
        setLED(true, false, false);
        // [LATER] playFailTone();
        Serial.println("Status: DOWN -> RED");
      } 
      else {
        // No change - BLUE
        setLED(false, false, true);
        Serial.println("Status: Stable -> BLUE");
      }

      // ===== DISPLAY UPDATE =====
      display.clearDisplay();
      
      display.setTextSize(1);
      display.setCursor(0, 0);
      display.print("BTC LIVE");
      
      // Divider
      display.drawLine(0, 10, 128, 10, WHITE);
      
      // Price
      display.setTextSize(2);
      display.setCursor(0, 20);
      display.print("$");
      // Print the formatted string to avoid scientific notation
      display.println(priceString); 

      // Change Indicator
      display.setTextSize(1);
      display.setCursor(0, 50);
      
      if (lastPrice != 0.0) {
        float diff = currentPrice - lastPrice;
        if (diff > 0) display.print("+$");
        else if (diff < 0) display.print("-$"); // Minus sign handled manually?
        
        display.print(abs(diff), 2);
      } else {
        display.println("Waiting trend...");
      }
      
      display.display();
      
      lastPrice = currentPrice;
      
    } else {
      Serial.print("JSON Error: ");
      Serial.println(error.c_str());
    }
  } 
  else {
    Serial.print("HTTP Error: ");
    Serial.println(httpResponseCode);
  }
  
  http.end();
}

// ==================== SETUP ====================

void setup() {
  Serial.begin(115200);
  delay(100);

  // --- Setup Pins ---
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  
  // [LATER] pinMode(PIN_BUZZER, OUTPUT);
  // [LATER] pinMode(PIN_LDR, INPUT);
  // [LATER] pinMode(PIN_BUTTON, INPUT_PULLUP);
  
  // Test LEDs (Red -> Green -> Blue)
  setLED(true, false, false); delay(200);
  setLED(false, true, false); delay(200);
  setLED(false, false, true); delay(200);
  setLED(false, false, false);
  
  // --- Initialize Display ---
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("System Starting...");
  display.display();

  // --- Connect to WiFi ---
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi Connected!");
  
  display.clearDisplay();
  display.println("WiFi OK!");
  display.display();
  delay(1000);

  // Perform initial data fetch
  updateData();
  lastTime = millis();
}

// ==================== MAIN LOOP ====================

void loop() {
  // 1. Automatic Timer Update
  if ((millis() - lastTime) > timerDelay) {
    updateData();
    lastTime = millis();
  }

  // [LATER] 2. Manual Button Logic
  /*
  if (digitalRead(PIN_BUTTON) == LOW) {
    display.clearDisplay();
    display.setCursor(0, 20);
    display.println("Refreshing...");
    display.display();
    updateData();
    lastTime = millis();
    while(digitalRead(PIN_BUTTON) == LOW); // Wait for release
  }
  */
  
  delay(100);
}