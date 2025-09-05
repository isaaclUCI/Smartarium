/**
 * Display Management System
 * 
 * Handles all TFT display operations for the TTGO T-Display.
 * Provides a clean interface for showing sensor data, splash screens,
 * and system status information on the 135x240 pixel ST7789 display.
 */

#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Config.h"
#include "Sensors.h"

/**
 * Display controller class for TTGO T-Display
 * 
 * Manages the ST7789 TFT display including initialization, layout,
 * and rendering of sensor data. Uses a clean columnar layout with
 * error handling for sensor failures.
 */
class Display {
public:
  /**
   * Initialize the TFT display
   * Sets up display rotation, backlight, colors, and clears screen
   */
  void begin();
  
  /**
   * Show the startup splash screen
   * Displays the SmartArium title and optional subtitle
   * 
   * @param subtitle Optional subtitle text (can be nullptr)
   */
  void showSplash(const char* subtitle);
  
  /**
   * Render the main sensor data display
   * Shows all current sensor readings with appropriate formatting
   * and error indicators for failed sensors
   * 
   * @param r Current sensor readings structure
   * @param ldrCalibrating true if light sensor is still calibrating
   */
  void render(const Readings& r, bool ldrCalibrating);
  
private:
  TFT_eSPI tft{135, 240};  // TFT display object with screen dimensions
  
  /**
   * Display a single data row
   * Helper function to maintain consistent formatting across all data rows
   * 
   * @param y Reference to Y position (modified after drawing)
   * @param key Label for the data (e.g., "Temp:")
   * @param val Value string to display
   */
  void row(int& y, const char* key, const String& val);
  
  /**
   * Draw the display header
   * Clears screen and draws the SmartArium title at the top
   */
  void header();
};
