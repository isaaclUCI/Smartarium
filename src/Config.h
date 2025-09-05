/**
 * SmartArium Configuration File
 * 
 * This file contains all hardware-specific pin assignments, calibration values,
 * and timing constants for the ESP32-based plant monitoring system.
 * 
 * Hardware: TTGO T-Display v1.1 with ESP32
 * Sensors: DHT22 (temp/humidity), soil moisture sensor, LDR (light)
 */

#pragma once
#include <Arduino.h>

/* =============================================================================
 * GPIO Pin Assignments for TTGO T-Display v1.1
 * =============================================================================
 * These pins are carefully chosen to avoid conflicts with the built-in display
 * and to use appropriate ADC channels for analog sensors.
 */
static const int DHT_PIN       = 15;   // DHT22 temperature/humidity sensor data pin
static const int DHT_TYPE      = 22;   // DHT sensor type (DHT22 = AM2302)
static const int SOIL_ADC_PIN  = 36;   // Soil moisture sensor (ADC1_CH0, input-only pin)
static const int LDR_ADC_PIN   = 33;   // Light-dependent resistor (ADC1_CH5)

/* =============================================================================
 * Soil Moisture Sensor Calibration Values
 * =============================================================================
 * These values map the raw ADC readings to meaningful moisture percentages.
 * Calibrate by taking readings in completely dry air vs. submerged in water.
 * Higher raw values = drier soil (less conductivity)
 */
static const int SOIL_RAW_AIR   = 3000;  // Raw ADC value when sensor is in dry air
static const int SOIL_RAW_WATER = 1200;  // Raw ADC value when sensor is in water

/* =============================================================================
 * Light Sensor Auto-Calibration
 * =============================================================================
 * The LDR (Light Dependent Resistor) auto-calibrates during the first 10 seconds
 * of operation by recording min/max light levels to establish a baseline.
 */
static const uint32_t LDR_CALIBRATION_MS = 10000;  // 10 seconds calibration window

/* =============================================================================
 * System Timing Configuration
 * =============================================================================
 * Controls how frequently sensors are read and data is displayed/transmitted.
 */
static const uint32_t SENSOR_SAMPLE_MS = 1000;  // Read sensors every 1 second

/* =============================================================================
 * Serial Communication & Display Settings
 * =============================================================================
 */
#define SERIAL_BAUD 9600              // Serial monitor baud rate
#define SHOW_UPTIME_ON_TFT 1          // Show system uptime on display (1=enabled, 0=disabled)
