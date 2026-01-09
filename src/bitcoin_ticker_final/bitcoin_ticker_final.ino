/*
 * Bitcoin Price Tracker - Complete Version
 * 
 * Features:
 * - Real-time Bitcoin price fetching from CoinDesk API
 * - OLED display showing current price and trend
 * - RGB LED visual indicators (Green=Up, Red=Down, Blue=Stable)
 * - Audio alerts via passive buzzer
 * - Smart mute function using photoresistor (auto-mute in darkness)
 * - Manual refresh button for instant updates
 * - Non-blocking code for responsive button input
 * 
 * Hardware:
 * - ESP32 Development Board
 * - 0.96" OLED Display (I2C, SSD1306)
 * - RGB LED (Common Cathode) with 220Ω resistors
 * - Passive Buzzer
 * - Photoresistor Module (4-pin with AO output)
 * - Tactile Push Button
 * 
 * Author: Bitcoin Tracker Project
 * License: MIT
 * Version: 1.0.0
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// ==================== CONFIGURATION ====================

// --- WIFI SETTINGS ---
// TODO: Replace with your WiFi credentials
const char* ssid = "YOUR_WIFI_NAME";       
const char* password = "YOUR_WIFI_PASSWORD"; 

// --- PIN DEFINITIONS ---
#define PIN_RED     15   // RGB LED Red pin
#define PIN_GREEN   2    // RGB LED Green pin
#define PIN_BLUE    4    // RGB LED Blue pin
#define PIN_BUZZER  13   // Passive Buzzer signal pin
#define PIN_LDR     34   // Photoresistor Analog Output (AO)
#define PIN_BUTTON  23   // Push button for manual refresh

// --- TIMING CONFIGURATION ---
// Time between automatic updates (milliseconds)
unsigned long timerDelay = 60000; // 60 seconds

// --- LIGHT SENSOR CONFIGURATION ---
// Adjust this value based on your environment
// Values: 0 (complete darkness) to 4095 (very bright)
// Sound will be MUTED when light level is BELOW this threshold
const int LIGHT_THRESHOLD = 1000;

// --- DISPLAY SETTINGS ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1  // Reset pin not used on 4-pin displays
#define SCREEN_ADDRESS 0x3C // I2C address (sometimes 0x3D)

// --- API SETTINGS ---
const char* endpoint = "https://api.coindesk.com/v1/bpi/currentprice/USD.json";

// ==================== GLOBAL OBJECTS ====================

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ==================== GLOBAL VARIABLES ====================

float lastPrice = 0.0;           // Previous price for comparison
unsigned long lastTime = 0;      // Timestamp of last update

// ==================== HELPER FUNCTIONS ====================

/**
 * Check if environment is bright enough for sound
 * Returns: true if bright (sound enabled), false if dark (muted)
 */
bool isEnvironmentBright() {
  int lightVal = analogRead(PIN_LDR);
  Serial.print("Current Light Level: ");
  Serial.println(lightVal);
  
  return (lightVal > LIGHT_THRESHOLD);
}

/**
 * Play success tone (price went up)
 * Only plays if environment is bright enough
 */
void playSuccessTone() {
  if (isEnvironmentBright()) {
    tone(PIN_BUZZER, 2000, 100);  // High pitch
    delay(150);
    tone(PIN_BUZZER, 3000, 200);  // Higher pitch
    delay(200);
    noTone(PIN_BUZZER);
    Serial.println("♪ Success tone played");
  } else {
    Serial.println("(Sound muted - environment is dark)");
  }
}

/**
 * Play failure tone (price went down)
 * Only plays if environment is bright enough
 */
void playFailTone() {
  if (isEnvironmentBright()) {
    tone(PIN_BUZZER, 500, 300);   // Low pitch
    delay(300);
    tone(PIN_BUZZER, 300, 500);   // Lower pitch
    delay(500);
    noTone(PIN_BUZZER);
    Serial.println("♪ Fail tone played");
  } else {
    Serial.println("(Sound muted - environment is dark)");
  }
}

/**
 * Set RGB LED color
 * Parameters: r, g, b (true = on, false = off)
 */
void setLED(bool r, bool g, bool b) {
  digitalWrite(PIN_RED, r ? HIGH : LOW);
  digitalWrite(PIN_GREEN, g ? HIGH : LOW);
  digitalWrite(PIN_BLUE, b ? HIGH : LOW);
}

/**
 * Main function to fetch and display Bitcoin price data
 */
void updateData() {
  // Check WiFi status
  if(WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi Disconnected!");
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 20);
    display.println("WiFi Error!");
    display.println("Reconnecting...");
    display.display();
    return;
  }

  // Show visual indicator that we're fetching data
  display.fillRect(120, 0, 8, 8, WHITE); 
  display.display();

  Serial.println("\n--- Fetching Bitcoin Price ---");
  
  HTTPClient http;
  http.begin(endpoint);
  int httpResponseCode = http.GET();
  
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    
    String payload = http.getString();
    
    // Parse JSON response
    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, payload);

    if (!error) {
      // Extract price data
      float currentPrice = doc["bpi"]["USD"]["rate_float"];
      const char* priceString = doc["bpi"]["USD"]["rate"];
      
      Serial.print("Current Price: $");
      Serial.println(currentPrice);
      
      // ===== LED & SOUND LOGIC =====
      setLED(false, false, false); // Turn all LEDs off first

      if (lastPrice == 0.0) {
        // First run - show blue (initializing)
        setLED(false, false, true);
        Serial.println("Status: Initializing");
      } 
      else if (currentPrice > lastPrice) {
        // Price increased - GREEN + Success sound
        setLED(false, true, false);
        playSuccessTone();
        Serial.println("Status: PRICE UP! 📈");
      } 
      else if (currentPrice < lastPrice) {
        // Price decreased - RED + Fail sound
        setLED(true, false, false);
        playFailTone();
        Serial.println("Status: PRICE DOWN 📉");
      } 
      else {
        // Price unchanged - BLUE
        setLED(false, false, true);
        Serial.println("Status: No change");
      }

      // ===== DISPLAY UPDATE =====
      display.clearDisplay();
      
      // --- Header ---
      display.setTextSize(1);
      display.setCursor(0, 0);
      display.print("BTC TRACKER");
      
      // Sound status indicator (top right)
      display.setCursor(90, 0);
      if (isEnvironmentBright()) {
        display.print("SOUND");
      } else {
        display.print("MUTE");
      }

      // Divider line
      display.drawLine(0, 10, 128, 10, WHITE);
      
      // --- Price Display ---
      display.setTextSize(2);
      display.setCursor(0, 20);
      display.print("$");
      display.println(priceString); 

      // --- Change Amount Display ---
      display.setTextSize(1);
      display.setCursor(0, 50);
      
      if (lastPrice != 0.0) {
        float diff = currentPrice - lastPrice;
        
        if (diff > 0) {
          display.print("+$");
          display.print(diff, 2);
        } 
        else if (diff < 0) {
          display.print("-$");
          display.print(abs(diff), 2);
        } 
        else {
          display.print("No Change");
        }
      } else {
        display.println("Initializing...");
      }
      
      display.display();
      
      // Update last price for next comparison
      lastPrice = currentPrice;
      
    } else {
      // JSON parsing error
      Serial.print("JSON Parse Error: ");
      Serial.println(error.c_str());
      
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0, 20);
      display.println("Parse Error");
      display.display();
    }
  } 
  else {
    // HTTP request failed
    Serial.print("HTTP Error code: ");
    Serial.println(httpResponseCode);
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 20);
    display.println("API Error");
    display.print("Code: ");
    display.println(httpResponseCode);
    display.display();
  }
  
  http.end();
  Serial.println("--- Update Complete ---\n");
}

// ==================== SETUP ====================

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  delay(100);
  Serial.println("\n\n=================================");
  Serial.println("Bitcoin Price Tracker v1.0");
  Serial.println("=================================\n");

  // --- Setup Pins ---
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_LDR, INPUT);
  
  // Setup button with internal pull-up resistor
  // This means: HIGH when not pressed, LOW when pressed
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  
  // Turn off all outputs initially
  setLED(false, false, false);
  
  // --- Initialize Display ---
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) { 
    Serial.println(F("ERROR: SSD1306 display allocation failed!"));
    Serial.println(F("Check wiring and I2C address (0x3C or 0x3D)"));
    // Don't proceed, loop forever
    for(;;);
  }
  
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("System Starting...");
  display.display();
  
  // Test buzzer briefly
  tone(PIN_BUZZER, 1000, 100);
  delay(150);
  
  Serial.println("Display initialized successfully");

  // --- Connect to WiFi ---
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Connecting to WiFi");
  display.print(ssid);
  display.display();
  
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 40) {
    delay(500);
    Serial.print(".");
    display.print(".");
    display.display();
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("WiFi Connected!");
    display.print("IP: ");
    display.println(WiFi.localIP());
    display.display();
    delay(2000);
    
    // Perform initial data fetch
    Serial.println("Performing initial update...");
    updateData();
    lastTime = millis();
    
  } else {
    Serial.println("\nWiFi Connection FAILED!");
    Serial.println("Please check your credentials and try again.");
    
    display.clearDisplay();
    display.setCursor(0, 10);
    display.println("WiFi Failed!");
    display.println("");
    display.println("Check credentials");
    display.println("and restart");
    display.display();
    
    // Flash red LED to indicate error
    for(int i = 0; i < 10; i++) {
      setLED(true, false, false);
      delay(200);
      setLED(false, false, false);
      delay(200);
    }
    
    // Halt execution
    for(;;);
  }
  
  Serial.println("\n=== Setup Complete ===");
  Serial.println("System is now running!\n");
}

// ==================== MAIN LOOP ====================

void loop() {
  // 1. Check if it's time for automatic update
  if ((millis() - lastTime) > timerDelay) {
    Serial.println("[AUTO UPDATE TRIGGERED]");
    updateData();
    lastTime = millis();
  }

  // 2. Check if manual refresh button is pressed
  // Button reads LOW when pressed (due to INPUT_PULLUP)
  if (digitalRead(PIN_BUTTON) == LOW) {
    Serial.println("[MANUAL REFRESH TRIGGERED]");
    
    // Show "Refreshing" message
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(10, 20);
    display.println("REFRESHING");
    display.display();
    
    // Small delay for button debounce
    delay(200);
    
    // Fetch new data
    updateData();
    
    // Reset timer so we don't immediately update again
    lastTime = millis();
    
    // Wait for button release to prevent multiple triggers
    while(digitalRead(PIN_BUTTON) == LOW) {
      delay(10);
    }
  }
  
  // Small delay to prevent excessive loop cycling
  delay(10);
}