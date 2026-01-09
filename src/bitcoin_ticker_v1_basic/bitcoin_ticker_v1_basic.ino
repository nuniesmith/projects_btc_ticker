/*
 * Bitcoin Price Tracker - Basic Version (v1)
 * 
 * Features:
 * - WiFi connectivity
 * - Real-time Bitcoin price fetching from CoinDesk API
 * - OLED display showing current price and basic info
 * 
 * Hardware:
 * - ESP32 Development Board
 * - 0.96" OLED Display (I2C, SSD1306)
 * 
 * This is the simplest version - perfect for testing your display
 * and WiFi connection before adding more components.
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

// --- DISPLAY SETTINGS ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1  // Reset pin not used on 4-pin displays
#define SCREEN_ADDRESS 0x3C // I2C address (usually 0x3C, sometimes 0x3D)

// Create display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// --- API SETTINGS ---
// Free CoinDesk API for current Bitcoin price
const char* endpoint = "https://api.coindesk.com/v1/bpi/currentprice/USD.json";

// ==================== SETUP ====================

void setup() {
  Serial.begin(115200);
  delay(100);
  
  Serial.println("\n=================================");
  Serial.println("Bitcoin Tracker - Basic v1.0");
  Serial.println("=================================\n");

  // 1. Initialize Display
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) { 
    Serial.println(F("SSD1306 allocation failed"));
    Serial.println(F("Check wiring and I2C address!"));
    for(;;); // Don't proceed, loop forever
  }
  
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Booting...");
  display.display();
  
  Serial.println("Display initialized");

  // 2. Connect to WiFi
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    display.print(".");
    display.display();
  }
  
  Serial.println("\nWiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("WiFi Connected!");
  display.display();
  delay(1000);
  
  Serial.println("\nSetup complete!\n");
}

// ==================== MAIN LOOP ====================

void loop() {
  // Check WiFi connection status
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    
    Serial.println("Fetching Bitcoin price...");
    
    // Begin HTTP request
    http.begin(endpoint);
    int httpResponseCode = http.GET();
    
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response: ");
      Serial.println(httpResponseCode);
      
      String payload = http.getString();
      
      // Parse JSON response
      // 1024 bytes is enough for this specific API response
      StaticJsonDocument<1024> doc;
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
        // Extract price from JSON path: bpi -> USD -> rate
        const char* price = doc["bpi"]["USD"]["rate"];
        const char* code = doc["bpi"]["USD"]["code"]; // "USD"
        
        Serial.print("Current Price: $");
        Serial.println(price);
        
        // ===== Display on OLED =====
        display.clearDisplay();
        
        // Header
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.println("BITCOIN TRACKER");
        
        // Divider line
        display.drawLine(0, 10, 128, 10, WHITE);
        
        // Price (make it bigger)
        display.setTextSize(2);
        display.setCursor(0, 20);
        display.print("$");
        // The API returns a string like "95,123.4567"
        display.println(price); 
        
        // Footer (Currency label)
        display.setTextSize(1);
        display.setCursor(0, 50);
        display.print("Currency: ");
        display.println(code);
        
        display.display();
        
      } else {
        Serial.print("JSON Parse Error: ");
        Serial.println(error.c_str());
        
        display.clearDisplay();
        display.setCursor(0, 20);
        display.println("Parse Error");
        display.display();
      }
    }
    else {
      Serial.print("HTTP Error: ");
      Serial.println(httpResponseCode);
      
      display.clearDisplay();
      display.setCursor(0, 20);
      display.println("API Error");
      display.display();
    }
    
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected!");
    
    display.clearDisplay();
    display.setCursor(0, 20);
    display.println("WiFi Error");
    display.display();
  }

  // Wait 60 seconds before next update
  // This respects API rate limits and reduces power consumption
  Serial.println("Waiting 60 seconds...\n");
  delay(60000); 
}