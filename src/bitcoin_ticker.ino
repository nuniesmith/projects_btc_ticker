/*
 * Crypto Tracker v3.0 - Multi-Asset Support
 * * NEW FEATURES:
 * - Button on GPIO 23 switches assets (BTC -> ETH -> SOL)
 * - Auto-reset LED logic when switching coins
 * * * WIRING:
 * - OLED: SDA -> D21, SCL -> D22
 * - Red LED: D15, Green LED: D2, Blue LED: D4
 * - Button: One leg to D23, other leg to GND
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

// ==================== CONFIGURATION ====================

const char* ssid = "SSID";        
const char* password = "PASS"; 

// --- ASSET CONFIGURATION ---
// Array of symbols to track
const char* assets[] = {"BTCUSDT", "ETHUSDT", "SOLUSDT"};
const char* assetNames[] = {"BTC", "ETH", "SOL"};
const int totalAssets = 3;
int currentAssetIndex = 0; // Start with BTC (Index 0)

// --- PIN DEFINITIONS ---
#define PIN_RED     15   
#define PIN_GREEN   2    
#define PIN_BLUE    4    
#define PIN_BUTTON  23   // <-- NEW BUTTON PIN

// --- TIMING ---
unsigned long timerDelay = 15000; 
unsigned long lastTime = 0;      

// --- DISPLAY ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1 
#define SCREEN_ADDRESS 0x3C 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ==================== GLOBAL VARIABLES ====================
float lastPrice = 0.0;           

// ==================== HELPER FUNCTIONS ====================

void setLED(bool r, bool g, bool b) {
  digitalWrite(PIN_RED, r ? HIGH : LOW);
  digitalWrite(PIN_GREEN, g ? HIGH : LOW);
  digitalWrite(PIN_BLUE, b ? HIGH : LOW);
}

void updateData() {
  if(WiFi.status() != WL_CONNECTED) return;

  // Visual loading indicator
  display.fillRect(118, 0, 10, 10, WHITE); 
  display.display();

  // 1. Construct the API URL dynamically based on current asset
  String url = "https://api.binance.com/api/v3/ticker/price?symbol=";
  url += assets[currentAssetIndex];

  Serial.print("Fetching: ");
  Serial.println(assets[currentAssetIndex]);
  
  WiFiClientSecure client;
  client.setInsecure(); 
  
  HTTPClient http;
  http.setUserAgent("Mozilla/5.0 (ESP32)"); 
  http.begin(client, url); // Use dynamic URL
  
  int httpResponseCode = http.GET();
  
  if (httpResponseCode > 0) {
    String payload = http.getString();
    StaticJsonDocument<768> doc;
    DeserializationError error = deserializeJson(doc, payload);

    if (!error) {
      String priceStrRaw = doc["price"];
      float currentPrice = priceStrRaw.toFloat();
      
      // Format price (2 decimals)
      char priceString[20];
      sprintf(priceString, "%.2f", currentPrice);

      // ===== LED LOGIC =====
      setLED(false, false, false); 

      if (lastPrice == 0.0) {
        setLED(false, false, true); // Blue (New Asset/Init)
      } 
      else if (currentPrice > lastPrice) {
        setLED(false, true, false); // Green
      } 
      else if (currentPrice < lastPrice) {
        setLED(true, false, false); // Red
      } 
      else {
        setLED(false, false, true); // Blue
      }

      // ===== DISPLAY =====
      display.clearDisplay();
      
      // Top Left: Asset Name
      display.setTextSize(2);
      display.setCursor(0, 0);
      display.print(assetNames[currentAssetIndex]); 
      
      // Top Right: "LIVE" text small
      display.setTextSize(1);
      display.setCursor(100, 4);
      display.print("LIVE");

      display.drawLine(0, 18, 128, 18, WHITE);
      
      // Price
      display.setTextSize(2);
      display.setCursor(0, 25);
      display.print("$");
      display.println(priceString); 

      // Bottom: Trend
      display.setTextSize(1);
      display.setCursor(0, 50);
      if (lastPrice != 0.0) {
        float diff = currentPrice - lastPrice;
        if (diff > 0) display.print("+$");
        else if (diff < 0) display.print(""); 
        display.print(diff, 2);
      } else {
        display.print("Tracking...");
      }
      
      display.display();
      lastPrice = currentPrice;
      
    }
  } 
  http.end();
}

// ==================== SETUP ====================

void setup() {
  Serial.begin(115200);

  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  
  // Setup Button: INPUT_PULLUP means we don't need a physical resistor
  pinMode(PIN_BUTTON, INPUT_PULLUP); 

  // Init Display
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) { 
    for(;;);
  }
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("Connecting...");
  display.display();

  // WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  // Initial Fetch
  updateData();
  lastTime = millis();
}

// ==================== MAIN LOOP ====================

void loop() {
  // 1. BUTTON LOGIC (Check if pressed)
  // digitalRead is LOW when pressed because we used INPUT_PULLUP
  if (digitalRead(PIN_BUTTON) == LOW) {
    Serial.println("Button Pressed! Switching Asset...");
    
    // Move to next asset
    currentAssetIndex++;
    if (currentAssetIndex >= totalAssets) {
      currentAssetIndex = 0; // Loop back to start
    }
    
    // Show "Switching" on screen
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(10, 25);
    display.print(assetNames[currentAssetIndex]);
    display.display();
    
    // IMPORTANT: Reset lastPrice so we don't compare BTC price to ETH price
    lastPrice = 0.0;
    
    // Debounce: Wait a bit, then wait for release
    delay(200); 
    while(digitalRead(PIN_BUTTON) == LOW); // Wait until user lets go
    
    // Update immediately
    updateData();
    lastTime = millis(); // Reset timer
  }

  // 2. AUTOMATIC TIMER
  if ((millis() - lastTime) > timerDelay) {
    updateData();
    lastTime = millis();
  }
}
