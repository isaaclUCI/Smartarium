/**
 * Sensor Management System
 * 
 * This module handles all sensor operations for the SmartArium including:
 * - DHT22 temperature and humidity sensor
 * - Capacitive soil moisture sensor  
 * - LDR (Light Dependent Resistor) for ambient light
 * 
 * Features automatic calibration for the light sensor and robust
 * error handling for sensor communication failures.
 */

#pragma once
#include <Arduino.h>
#include "Config.h"
#include "Utils.h"

/**
 * Structure containing all sensor readings
 * 
 * Uses invalid sentinel values (-1, NAN) to indicate sensor failures
 * or readings that are not yet available. This allows the display
 * and logging systems to show appropriate error indicators.
 */
struct Readings {
  float tempC    = NAN;  // Temperature in Celsius (NAN = sensor error)
  float humidity = NAN;  // Relative humidity 0-100% (NAN = sensor error)
  int   soilPct  = -1;   // Soil moisture 0-100% (-1 = sensor error)
  int   lightPct = -1;   // Light level 0-100% (-1 = calibrating/error)
  int   soilRaw  = -1;   // Raw ADC value from soil sensor (for debugging)
  int   ldrRaw   = -1;   // Raw ADC value from light sensor (for debugging)
};

/**
 * Sensor management class
 * 
 * Handles initialization, periodic sampling, and calibration of all sensors.
 * Uses non-blocking timing to ensure the main loop remains responsive.
 */
class Sensors {
public:
  /**
   * Initialize all sensors and ADC configuration
   * Must be called once during system startup
   */
  void begin();
  
  /**
   * Update all sensor readings (non-blocking)
   * Should be called frequently from the main loop
   * 
   * @param nowMs Current time in milliseconds from millis()
   */
  void update(uint32_t nowMs);
  
  /**
   * Get the most recent sensor readings
   * @return Current sensor data structure
   */
  Readings current() const { return cur_; }
  
  /**
   * Check if light sensor is still in calibration mode
   * @param nowMs Current time in milliseconds
   * @return true if still calibrating, false if calibration complete
   */
  bool calibrating(uint32_t nowMs) const { 
    return (nowMs - bootMs_) < LDR_CALIBRATION_MS; 
  }
  
  /**
   * Get the minimum light value recorded during calibration
   * @return Minimum ADC value (darkest condition)
   */
  int ldrMin() const { return ldrMin_; }
  
  /**
   * Get the maximum light value recorded during calibration  
   * @return Maximum ADC value (brightest condition)
   */
  int ldrMax() const { return ldrMax_; }

private:
  Readings  cur_;                                    // Current sensor readings
  uint32_t  bootMs_{0};                             // System boot time for calibration
  int ldrMin_{4095};                                // Min light value (starts at ADC max)
  int ldrMax_{0};                                   // Max light value (starts at ADC min)
  Utils::Ticker sampleTick{SENSOR_SAMPLE_MS};      // Non-blocking sampling timer

  /**
   * Read temperature and humidity from DHT22 sensor
   * Updates cur_.tempC and cur_.humidity
   */
  void sampleDHT();
  
  /**
   * Read soil moisture from capacitive sensor
   * Updates cur_.soilRaw and cur_.soilPct
   */
  void sampleSoil();
  
  /**
   * Read light level from LDR and perform auto-calibration
   * Updates cur_.ldrRaw and cur_.lightPct
   * 
   * @param nowMs Current time for calibration timing
   */
  void sampleLDR(uint32_t nowMs);
};
