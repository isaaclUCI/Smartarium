/**
 * Utility Functions Implementation
 * 
 * Implementation of mathematical mapping and constraining functions
 * used for sensor data processing throughout the SmartArium system.
 */

#include "Utils.h"

/**
 * Maps a value from one range to another with automatic constraining
 * 
 * This function safely handles edge cases like division by zero and
 * automatically constrains the input to the valid range before mapping.
 * Uses long arithmetic to prevent integer overflow during calculation.
 */
int Utils::mapConstrain(int x, int in_min, int in_max, int out_min, int out_max) {
  // Handle degenerate case where input range is zero
  if (in_min == in_max) return out_min;
  
  // Constrain input to valid range (handles both normal and inverted ranges)
  x = constrain(x, min(in_min, in_max), max(in_min, in_max));
  
  // Perform mapping using long arithmetic to prevent overflow
  // Formula: output = (input - in_min) * (out_max - out_min) / (in_max - in_min) + out_min
  long val = (long)(x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  
  return (int)val;
}

/**
 * Bidirectional mapping that automatically handles inverted ranges
 * 
 * This function determines the orientation of the input range and
 * maps accordingly. Useful for sensors where the relationship between
 * raw values and meaningful measurements might be inverted.
 * 
 * Example: Soil sensor where higher ADC values = drier soil (inverted relationship)
 */
int Utils::mapConstrainBi(int x, int inA, int inB, int outMin, int outMax) {
  // Handle degenerate case
  if (inA == inB) return outMin;
  
  // Determine range orientation and map accordingly
  if (inA < inB) {
    // Normal orientation: inA to inB maps to outMin to outMax
    return mapConstrain(x, inA, inB, outMin, outMax);
  } else {
    // Inverted orientation: inA to inB maps to outMax to outMin
    return mapConstrain(x, inB, inA, outMax, outMin);
  }
}
