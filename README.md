# SmartRoom ESP32-C3 — Smart Lighting & Environment Monitoring System

A comprehensive IoT and Embedded System project based on the **ESP32-C3** microcontroller, featuring DHT22 temperature/humidity sensor, BH1750 ambient light sensor, potentiometer threshold ADC, **RTC DS1307 real-time clock**, physical push button, OLED SSD1306 display, and 3 status LEDs (Green, Red, White).

---

## 1. Hardware Pinout Mapping

| Component | ESP32-C3 Pin | Interface / Protocol | Function |
| :--- | :--- | :--- | :--- |
| **RTC DS1307** | `SDA: GPIO 4`, `SCL: GPIO 5` | I2C (`0x68`) | Real-time calendar & clock |
| **RTC SQW Tick** | `GPIO 0` | Hardware Interrupt | 1Hz square wave for periodic 1-second ticks |
| **Wall Push Button** | `GPIO 2` | Hardware Interrupt | Physical room button (Debounced, active LOW) |
| **DHT Sensor** | `GPIO 6` | 1-Wire Digital | Temperature & Relative Humidity |
| **Light Sensor** | `SDA: GPIO 4`, `SCL: GPIO 5` | I2C (`0x23`) | Ambient illuminance in Lux |
| **OLED SSD1306** | `SDA: GPIO 4`, `SCL: GPIO 5` | I2C (`0x3C`) | Displays TEMP, HUM, LUX, and RTC TIME |
| **Potentiometer** | `GPIO 3` (ADC1_CH3) | Analog ADC (12-bit) | Sensitivity/threshold tuning |
| **Green LED** | `GPIO 7` | Digital Output | **ON when environment is COMFORTABLE** |
| **Red LED** | `GPIO 10` | Digital Output | **ON when any threshold is VIOLATED (Alert)** |
| **White LED** | `GPIO 1` | Digital Output | **Main room lamp** (Automated & Manual control) |

---

## 2. Smart Lighting Algorithm (White Lamp)

### A. Automatic Mode (`MODE_AUTO`):
1. **Daytime (06:00 – 17:59):**
   - Default: **OFF** (utilizing natural sunlight).
   - **Low-light compensation:** If indoor illuminance drops below **$120\text{ lux}$** (rainy, stormy, curtains closed) $\rightarrow$ **Auto turn ON** white lamp. When daylight recovers above **$160\text{ lux}$** $\rightarrow$ **Auto turn OFF**.
   - **Excessive daylight shutoff:** If daytime ambient light becomes excessively bright ($\ge 800\text{ lux}$) $\rightarrow$ **The white lamp automatically turns OFF immediately** if it is currently on.
2. **Nighttime (18:00 – 05:59):**
   - Default: **ON** (evening illumination).
   - **Excessive brightness shutoff:** If nighttime ambient light exceeds **$350\text{ lux}$** $\rightarrow$ **Auto turn OFF** white lamp to save energy and prevent glare. Turns back on when light drops below **$280\text{ lux}$**.

### B. Manual Override:
- **Physical Wall Button (`GPIO 2`):** Pressing the button toggles the lamp and cycles through modes:
  $$\text{AUTO} \longrightarrow \text{MANUAL ON} \longrightarrow \text{MANUAL OFF} \longrightarrow \text{AUTO}$$
- **Web Dashboard & UART Serial:** Dedicated buttons for `AUTO`, `ON`, `OFF` (UART keys: `A`, `1`, `0`).

---

## 3. Environmental Comfort Criteria

1. **Comfortable Temperature:** $20.0^\circ\text{C} \le \text{Temp} \le 28.0^\circ\text{C}$.
2. **Comfortable Humidity:** $40.0\% \le \text{Humidity} \le 70.0\%$.
3. **Comfortable Light Level (Day vs. Night):**
   - **Daytime:** $150\text{ lux} \le \text{Lux} \le 1000\text{ lux}$.
   - **Nighttime:** $20\text{ lux} \le \text{Lux} \le 300\text{ lux}$.
4. **Indicator Logic:**
   - All 3 parameters satisfied $\rightarrow$ **Green LED ON, Red LED OFF**.
   - Any parameter out of range (or sensor error) $\rightarrow$ **Red LED ON, Green LED OFF**.

---

## 4. Wokwi Simulation Testing Guide

1. Open [diagram.json](file:///d:/Project/ESP32/Smart_Room/diagram.json).
2. Click **Play / Restart 🔄**.
3. **Interactive Scenarios:**
   - **Wall button toggle:** Click the blue pushbutton (`btn`) on canvas $\rightarrow$ Lamp toggles and mode changes.
   - **Daytime excessive brightness test:** Set daytime hour, turn white lamp ON, then slide ambient light up to $\ge 800\text{ lux}$ $\rightarrow$ Notice the white lamp automatically turns OFF immediately!
