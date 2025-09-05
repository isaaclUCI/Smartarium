/**
 * SmartArium - ESP32 Plant Monitoring System
 * 
 * A comprehensive plant monitoring system using TTGO T-Display ESP32.
 * Monitors temperature, humidity, soil moisture, and ambient light levels
 * with real-time display on TFT screen and serial output for logging.
 * 
 * Hardware:
 * - TTGO T-Display ESP32 (ST7789 135x240 TFT)
 * - DHT22 temperature/humidity sensor
 * - Capacitive soil moisture sensor  
 * - LDR (Light Dependent Resistor)
 * 
 * Features:
 * - Real-time sensor monitoring
 * - Automatic light sensor calibration
 * - Non-blocking sensor updates
 * - Robust error handling
 * - Serial logging for data analysis
 */

#include <Arduino.h>
#include "Config.h"
#include "Utils.h"
#include "Sensors.h"
#include "Display.h"

// =============================================================================
// Global Objects
// =============================================================================

Sensors sensors;    // Sensor management system
Display screen;     // TFT display controller

// Non-blocking timers for different update rates
Utils::Ticker serialTick{1000};  // Serial output every 1 second
Utils::Ticker renderTick{250};   // Display update every 250ms (smooth updates)

// =============================================================================
// Arduino Setup Function
// =============================================================================

/**
 * System initialization
 * 
 * Initializes serial communication, display, and sensors.
 * Shows startup splash screen while sensors initialize.
 */
void setup() {
  // Initialize serial communication for debugging and data logging
  Serial.begin(SERIAL_BAUD);
  delay(200);                         // Allow serial to stabilize

  // Initialize and show splash screen
  screen.begin();
  screen.showSplash("Sensors only");  // Indicate this is sensor-only version

  // Initialize all sensors
  sensors.begin();
  
  // Announce system startup
  Serial.println(F("SmartArium (monitor-only): DHT22 + Soil + LDR"));
}

// =============================================================================
// Arduino Main Loop
// =============================================================================

/**
 * Main system loop
 * 
 * Runs continuously to update sensors, display, and serial output.
 * Uses non-blocking timers to maintain responsive operation without
 * blocking delays. Each subsystem updates at its optimal rate.
 */
void loop() {
  const uint32_t now = millis();      // Get current time once per loop

  // Update all sensors (non-blocking, rate-limited internally)
  sensors.update(now);
  const Readings r = sensors.current(); // Get latest readings

  // Serial data logging (every 1 second)
  if (serialTick.due(now)) {
    // Output sensor data in comma-friendly format for logging/analysis
    Serial.print(F("Temp: "));
    if (isnan(r.tempC)) Serial.print(F("--.-")); else Serial.print(r.tempC, 1);
    
    Serial.print(F(" C, Humidity: "));
    if (isnan(r.humidity)) Serial.print(F("--.-")); else Serial.print(r.humidity, 1);
    
    Serial.print(F(" %, Soil: "));
    if (r.soilPct < 0) Serial.print(F("--")); else Serial.print(r.soilPct);
    
    Serial.print(F(" %, Light: "));
    if (r.lightPct < 0) Serial.print(F("--")); else Serial.print(r.lightPct);
    
    Serial.println(F(" %"));
  }

  // Display update (every 250ms for smooth visual updates)
  if (renderTick.due(now)) {
    // Pass calibration status to display appropriate messages
    screen.render(r, sensors.calibrating(now));
  }
}
