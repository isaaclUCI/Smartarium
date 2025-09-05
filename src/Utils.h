/**
 * Utility Functions and Classes
 * 
 * This file provides reusable utility functions for mathematical operations
 * and timing control that are used throughout the SmartArium system.
 */

#pragma once
#include <Arduino.h>

namespace Utils {
  
  /**
   * Maps and constrains a value from one range to another
   * Similar to Arduino's map() function but with built-in constrain()
   * 
   * @param x Value to map
   * @param in_min Minimum of input range
   * @param in_max Maximum of input range  
   * @param out_min Minimum of output range
   * @param out_max Maximum of output range
   * @return Mapped and constrained value
   */
  int mapConstrain(int x, int in_min, int in_max, int out_min, int out_max);
  
  /**
   * Bidirectional mapping with automatic range detection
   * Handles cases where input range might be inverted (inA > inB)
   * Useful for sensors where higher values might mean lower measurements
   * 
   * @param x Value to map
   * @param inA First input range boundary
   * @param inB Second input range boundary
   * @param outMin Minimum output value
   * @param outMax Maximum output value
   * @return Mapped value with automatic range orientation
   */
  int mapConstrainBi(int x, int inA, int inB, int outMin, int outMax);

  /**
   * Clamps a floating-point value between minimum and maximum bounds
   * 
   * @param v Value to clamp
   * @param lo Lower bound
   * @param hi Upper bound
   * @return Clamped value
   */
  inline float fclamp(float v, float lo, float hi) {
    return (v < lo) ? lo : (v > hi) ? hi : v;
  }

  /**
   * Non-blocking timer class for periodic operations
   * 
   * Allows checking if a specified time period has elapsed without
   * using blocking delay() calls. Essential for multi-tasking in the main loop.
   */
  class Ticker {
  public:
    /**
     * Constructor
     * @param periodMs Time period in milliseconds (default: 1000ms)
     */
    explicit Ticker(uint32_t periodMs = 1000) : period(periodMs) {}
    
    /**
     * Change the timer period
     * @param ms New period in milliseconds
     */
    void set(uint32_t ms) { period = ms; }
    
    /**
     * Check if the timer period has elapsed
     * @param now Current time in milliseconds (from millis())
     * @return true if period has elapsed, false otherwise
     */
    bool due(uint32_t now) {
      if (now - last >= period) { 
        last = now; 
        return true; 
      }
      return false;
    }
    
  private:
    uint32_t period;     // Timer period in milliseconds
    uint32_t last{0};    // Last time the timer triggered
  };
}
