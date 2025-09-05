# SmartArium — Sensors Only (TTGO T-Display, ESP32)

> “Make it work, then make it right.” —Kent Beck

Simple, lab-style firmware that reads **DHT22 temperature & humidity**, **capacitive soil moisture (analog)**, and **LDR light (analog)** on a **TTGO T-Display**. Values are shown on the TFT and printed to the Serial Monitor. No relays, no cloud.

## Hardware
- TTGO T-Display v1.1 (ESP32, ST7789 135×240)
- DIYables **DHT22** module (3.3 V)
- diymore **Capacitive Soil Moisture v1.2** (AO, 3.3 V)
- LM393 **LDR** module (use **AO**, not DO; 3.3 V)
- Jumper wires / breadboard

## Wiring (3.3 V sensors; all grounds common)
| Sensor | Pin on sensor | TTGO pin |
|---|---|---|
| DHT22 | VCC | 3V3 |
|  | GND | GND |
|  | DATA | **GPIO15** |
| Soil (v1.2) | AO | **GPIO36** (ADC1_CH0) |
|  | VCC | 3V3 |
|  | GND | GND |
| LDR module | **AO** | **GPIO33** (ADC1_CH5) |
|  | VCC | 3V3 |
|  | GND | GND |

> Keep analog inputs on **ADC1** (GPIO36 & GPIO33) for best reliability.

## Build & Run (PlatformIO)
```bash
pio run -t upload
pio device monitor
