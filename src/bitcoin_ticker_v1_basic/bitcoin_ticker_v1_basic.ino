/*
 * Bitcoin Price Tracker - Binance Version (v2.0)
 * * WHY THIS VERSION?
 * - Uses Binance API (Faster, more reliable for ESP32 than CoinDesk)
 * - Adds "User-Agent" header to prevent Error -1 (Blocking)
 * - Simpler JSON parsing
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

// ==================== CONFIGURATION ====================

// --- WIFI SETTINGS ---
const char* ssid = "SSID";       
const char* password = "PASS"; 

// --- DISPLAY SETTINGS ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1 
#define SCREEN_ADDRESS 0x3C 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// --- API SETTINGS (BINANCE) ---
// Binance is much faster and rarely blocks ESP32s
const char* endpoint = "https://api.binance.com/api/v3/ticker/price?symbol=BTCUSDT";

// ==================== SETUP ====================

void setup() {
  Serial.begin(115200);
  delay(100);
  
  Serial.println("\n=================================");
  Serial.println("Bitcoin Tracker - v2.0 (Binance)");
  Serial.println("=================================\n");

  // 1. Initialize Display
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Connecting...");
  display.display();

  // 2. Connect to WiFi
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  // Wait loop
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    attempts++;
    if(attempts > 20) { // If it takes too long, retry
       Serial.println("\nStill connecting... check password?");
       attempts = 0;
    }
  }
  
  Serial.println("\nWiFi Connected!");
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("WiFi OK!");
  display.println("Fetching BTC...");
  display.display();
  
  // Small delay to let WiFi stack settle
  delay(1000);
}

// ==================== MAIN LOOP ====================

void loop() {
  if(WiFi.status() == WL_CONNECTED){
    
    WiFiClientSecure client;
    client.setInsecure(); // Ignore SSL certificate
    
    HTTPClient http;
    
    // START THE REQUEST
    // We add a User-Agent so Binance knows we are a legitimate device
    http.setUserAgent("Mozilla/5.0 (ESP32)"); 
    http.begin(client, endpoint);
    
    Serial.println("Ping Binance API...");
    int httpResponseCode = http.GET();
    
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response: ");
      Serial.println(httpResponseCode);
      
      String payload = http.getString();
      
      // Parse JSON
      // Binance returns simple JSON: {"symbol":"BTCUSDT","price":"95123.00"}
      StaticJsonDocument<768> doc;
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
        String priceString = doc["price"].as<String>();
        
        // Clean up price (remove extra decimals if needed)
        // Usually comes as "95123.45000000" -> We take substring for cleaner look
        int dotIndex = priceString.indexOf('.');
        if(dotIndex > 0) {
           priceString = priceString.substring(0, dotIndex + 3); // Keep 2 decimals
        }

        Serial.print("BTC: $");
        Serial.println(priceString);
        
        // ===== UPDATE SCREEN =====
        display.clearDisplay();
        
        // Header
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.println("BINANCE LIVE");
        
        display.drawLine(0, 10, 128, 10, WHITE);
        
        // Price Big
        display.setTextSize(2); 
        display.setCursor(0, 25);
        display.print("$");
        display.println(priceString); 
        
        // Footer
        display.setTextSize(1);
        display.setCursor(0, 55);
        display.print("Pair: BTC/USDT");
        
        display.display();
        
      } else {
        Serial.print("JSON Error: ");
        Serial.println(error.c_str());
      }
    }
    else {
      // PRINT THE EXACT ERROR REASON
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
      Serial.print("Reason: ");
      Serial.println(http.errorToString(httpResponseCode)); 
      
      display.clearDisplay();
      display.setCursor(0, 20);
      display.println("API Error:");
      display.setCursor(0, 35);
      display.print(httpResponseCode);
      display.display();
    }
    
    http.end();
  }
  else {
    Serial.println("WiFi Lost...");
    WiFi.reconnect();
  }

  // Update every 15 seconds (Binance allows faster updates than CoinDesk)
  delay(15000); 
}
