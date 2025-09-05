/**
 * Display Implementation
 * 
 * Implements the TFT display interface for the SmartArium system.
 * Handles screen initialization, layout management, and sensor data presentation
 * with robust error handling and clean formatting.
 */

#include "Display.h"

/**
 * Initialize the TFT display hardware
 * 
 * Configures the ST7789 display controller, sets orientation for landscape mode,
 * enables the backlight, and prepares the screen for drawing operations.
 */
void Display::begin() {
  tft.init();                          // Initialize the ST7789 display controller
  tft.setRotation(1);                  // Landscape orientation (135x240 becomes 240x135)
  
  // Configure backlight control (pin 4 controls TFT_BL)
  pinMode(4, OUTPUT);
  digitalWrite(4, 1);                  // Turn on backlight (active high)
  
  // Set up initial display state
  tft.fillScreen(TFT_BLACK);           // Clear screen to black
  tft.setTextColor(TFT_WHITE, TFT_BLACK); // White text on black background
}

/**
 * Display the startup splash screen
 * 
 * Shows the SmartArium branding and optional subtitle during system initialization.
 * Uses larger text for the main title to make it prominent.
 */
void Display::showSplash(const char* subtitle) {
  tft.fillScreen(TFT_BLACK);           // Clear the entire screen
  
  // Draw main title in large text
  tft.setTextSize(2);                  // Double-size text for prominence
  tft.setCursor(8, 10);                // Position near top-left with margin
  tft.println("SmartArium");
  
  // Draw subtitle in normal text if provided
  tft.setTextSize(1);                  // Standard text size
  tft.setCursor(8, 34);                // Position below main title
  tft.println(subtitle ? subtitle : ""); // Handle null subtitle gracefully
}

/**
 * Draw the standard display header
 * 
 * Clears the screen and draws the SmartArium title at the top.
 * This is called before rendering the main sensor data display.
 */
void Display::header() {
  tft.fillScreen(TFT_BLACK);           // Clear previous content
  
  // Draw title
  tft.setTextSize(2);                  // Large text for header
  tft.setCursor(6, 4);                 // Top margin
  tft.println("SmartArium");
  
  tft.setTextSize(1);                  // Reset to normal text size for data rows
}

/**
 * Display a single data row with consistent formatting
 * 
 * Helper function that maintains consistent spacing and alignment across
 * all sensor data rows. Uses printf-style formatting for clean columns.
 */
void Display::row(int& y, const char* key, const String& val) {
  tft.setCursor(6, y);                 // Left margin for alignment
  
  // Use printf for consistent column formatting
  // %-8s creates an 8-character left-aligned field for the key
  tft.printf("%-8s %s\n", key, val.c_str());
  
  y += 16;                             // Move to next row (16 pixels down)
}

/**
 * Render the complete sensor data display
 * 
 * Shows all current sensor readings with appropriate error handling.
 * Displays "calibrating" status for light sensor during startup period.
 * Uses consistent formatting and handles sensor failure gracefully.
 */
void Display::render(const Readings& r, bool ldrCalibrating) {
  header();                            // Draw title and clear screen

#if SHOW_UPTIME_ON_TFT
  // Calculate and display system uptime if enabled in config
  static uint32_t t0 = millis();       // Remember first call time
  uint32_t up = (millis() - t0) / 1000; // Convert to seconds
#endif

  int y = 26;                          // Starting Y position (below header)

#if SHOW_UPTIME_ON_TFT
  // Show system uptime for debugging/monitoring
  row(y, "Uptime:", String(up) + " s");
#endif

  // Show calibration status for light sensor
  if (ldrCalibrating) {
    row(y, "Status:", "Calibrating LDR...");
  }

  // Display temperature with error handling
  // DHT22 returns NAN when communication fails
  row(y, "Temp:",  isnan(r.tempC) ? "--.- C" : String(r.tempC, 1) + " C");
  
  // Display humidity with error handling
  row(y, "Humid:", isnan(r.humidity) ? "-- %" : String(r.humidity, 1) + " %");
  
  // Display soil moisture (negative values indicate sensor errors)
  row(y, "Soil:",  (r.soilPct < 0) ? "-- %" : String(r.soilPct) + " %");
  
  // Display light level (negative values indicate calibration/errors)
  row(y, "Light:", (r.lightPct < 0) ? "-- %" : String(r.lightPct) + " %");
}
