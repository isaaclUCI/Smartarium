/**
 * Sensor Implementation
 * 
 * Implements all sensor reading and calibration logic for the SmartArium.
 * Handles DHT22, soil moisture, and light sensors with robust error handling.
 */

#include "Sensors.h"
#include <DHT.h>

// Global DHT sensor instance (required by the DHT library)
static DHT dht(DHT_PIN, DHT_TYPE);

/**
 * Initialize all sensors and configure ADC settings
 * 
 * Sets up the DHT22 sensor, configures ADC resolution and attenuation
 * for optimal analog sensor readings, and records boot time for calibration.
 */
void Sensors::begin() {
  bootMs_ = millis();  // Record boot time for LDR calibration timing
  
  // Initialize DHT22 temperature/humidity sensor
  dht.begin();
  
  // Configure ADC for 12-bit resolution (0-4095 range)
  analogReadResolution(12);
  
  // Set ADC attenuation for 3.3V range on soil moisture pin
  // ADC_11db allows reading up to ~3.3V input voltage
  analogSetPinAttenuation(SOIL_ADC_PIN, ADC_11db);
  analogSetPinAttenuation(LDR_ADC_PIN,  ADC_11db);
}

/**
 * Non-blocking sensor update function
 * 
 * Checks if it's time to sample sensors based on the configured interval.
 * Only performs sensor readings when the sample timer expires to avoid
 * overwhelming the DHT22 sensor (which needs time between readings).
 */
void Sensors::update(uint32_t nowMs) {
  // Check if it's time for the next sensor sample
  if (!sampleTick.due(nowMs)) return;
  
  // Sample all sensors
  sampleDHT();      // Temperature and humidity
  sampleSoil();     // Soil moisture  
  sampleLDR(nowMs); // Light level with auto-calibration
}

/**
 * Read temperature and humidity from DHT22 sensor
 * 
 * The DHT22 is a digital sensor that communicates over a single wire.
 * Readings can occasionally fail, resulting in NAN values which are
 * handled gracefully by the display system.
 */
void Sensors::sampleDHT() {
  cur_.tempC    = dht.readTemperature();  // Celsius by default
  cur_.humidity = dht.readHumidity();     // Relative humidity percentage
}

/**
 * Read soil moisture from capacitive sensor
 * 
 * Capacitive soil sensors work by measuring the dielectric constant of soil,
 * which changes with moisture content. Higher water content = lower resistance = lower ADC reading.
 * The raw ADC value is mapped to a 0-100% scale using calibration constants.
 */
void Sensors::sampleSoil() {
  cur_.soilRaw = analogRead(SOIL_ADC_PIN);
  
  // Map raw ADC to percentage using bidirectional mapping
  // SOIL_RAW_WATER (low ADC) = 100% moisture
  // SOIL_RAW_AIR (high ADC) = 0% moisture  
  cur_.soilPct = Utils::mapConstrainBi(cur_.soilRaw, SOIL_RAW_WATER, SOIL_RAW_AIR, 100, 0);
}

/**
 * Read light level from LDR with automatic calibration
 * 
 * Light Dependent Resistors (LDR) vary their resistance based on ambient light.
 * Since lighting conditions vary greatly, the sensor auto-calibrates during the
 * first 10 seconds of operation by recording min/max values encountered.
 */
void Sensors::sampleLDR(uint32_t nowMs) {
  cur_.ldrRaw = analogRead(LDR_ADC_PIN);
  
  // Auto-calibration during the first 10 seconds of operation
  if (calibrating(nowMs)) {
    // Track minimum and maximum light levels encountered
    if (cur_.ldrRaw < ldrMin_) ldrMin_ = cur_.ldrRaw;
    if (cur_.ldrRaw > ldrMax_) ldrMax_ = cur_.ldrRaw;
  }
  
  // Handle edge case where min equals max (no variation during calibration)
  // Create a small artificial range to prevent division by zero
  if (ldrMin_ == ldrMax_) { 
    ldrMin_ = max(0, cur_.ldrRaw - 50);       // Ensure we don't go below 0
    ldrMax_ = min(4095, cur_.ldrRaw + 50);    // Ensure we don't exceed ADC max
  }
  
  // Map calibrated range to 0-100% scale
  cur_.lightPct = Utils::mapConstrainBi(cur_.ldrRaw, ldrMin_, ldrMax_, 0, 100);
}
