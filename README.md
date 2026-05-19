# Automatic Aquarium Water Cooling and Monitoring System

**Course:** KQC7029 Embedded Systems
**Semester:** Semester 2, Session 2025/2026
**Faculty:** Faculty of Engineering, Universiti Malaya
**Lecturer:** Professor Ir. Dr. Jeevan A/L Kanesan
**Group:** 4

---

## Group Members

| No. | Name | Matric No. |
|----:|------|------------|
| 1 | Muhammad Amru Bin Mohamad Sharis | S2116804 |
| 2 | Md Fahim Faisal | 24076234 |
| 3 | Veronica Nurai anak Jeffery | 17107275/2 |
| 4 | Ahmad Afiq Bin Ahmad Aznan | 24078625 |
| 5 | Fu Jie | 25058239 |
| 6 | Jiang Xuyang | 25070714 |
| 7 | Li Shaobo | 25067068 |
| 8 | Guo Xuanzhuo | 24215841 |
| 9 | Avienash A/L Shankar | U2103136 |

---

## Project Overview

An IoT-enabled aquarium water cooling and monitoring system built around the ESP32. The system continuously measures water temperature with a waterproof DS18B20 probe and automatically switches a 12 V DC cooling fan on or off through a relay module to keep the tank within a safe thermal range. Live temperature and fan status are shown on a 20×4 I2C LCD and streamed to the Blynk IoT platform so the user can monitor the tank — and override the fan into manual mode — from a phone or web dashboard.

The prototype is powered through a solar charge controller and an 18650 Li-ion battery pack, with a 12 V DC adapter as an alternative input. This gives the system a small backup reservoir so monitoring continues across short mains interruptions.

### Key Features

- Real-time water temperature sensing with DS18B20 (1-Wire, ±0.5 °C)
- Automatic fan control with hysteresis (fan ON ≥ 27 °C, OFF ≤ 24 °C) to avoid relay chattering
- Relay-based ON/OFF fan switching (no PWM; full-speed cooling when active)
- 20×4 I2C LCD for local readout of temperature, fan state, and operating mode
- Tri-colour LED status indicators (green = normal, yellow = fan running, red = warning)
- Buzzer alert when the fan engages on high-temperature trip
- Blynk IoT dashboard for remote monitoring and AUTO/MANUAL mode switching
- Solar + 18650 battery backup powering the control electronics

---

## Hardware Components

See `pictures/Components.jpg` for the actual parts used in the build.

| Component | Role |
|---|---|
| ESP32 DevKit V1 | Main microcontroller, Wi-Fi to Blynk |
| DS18B20 waterproof probe | Water temperature sensor (1-Wire on GPIO 32) |
| Relay module (single-channel) | ON/OFF switching of the 12 V fan (GPIO 27) |
| 12 V DC cooling fan | Heat removal actuator |
| 20×4 I2C LCD (PCF8574, addr 0x27) | Local display |
| Red / Yellow / Green LEDs | Status indication (GPIO 33 / 25 / 26) |
| Buzzer | Audible high-temperature alert (GPIO 14) |
| Solar charge controller | Manages battery charging from solar input |
| 18650 Li-ion battery pack | Backup power source |
| 12 V DC adapter | Primary power input |
| Breadboard, jumper wires, 4.7 kΩ resistor | Prototyping and DS18B20 pull-up |

> Note: an L298N motor driver appears in the early draft proposal but was **replaced with a relay module** in the final build. Cooling demand is binary (fan on / fan off), so PWM speed control was not required.

---

## How It Works

1. The DS18B20 probe reads the water temperature every second.
2. The ESP32 compares the reading against the configured thresholds:
   - **≥ 27 °C** → energise relay, fan turns ON, yellow LED on, buzzer chirps once on trip.
   - **≤ 24 °C** → de-energise relay, fan turns OFF, green LED on.
3. Temperature and fan state are written to the 20×4 LCD and to Blynk virtual pins (`V0` temperature, `V1` fan state, `V4` text status).
4. From the Blynk app the user can switch to MANUAL mode (`V3`) and toggle the fan directly via `V2`, overriding the auto controller.

---

## Repository Layout

```
.
├── README.md
├── src/
│   └── KQC7029_IoT_Aquarium_Cooler_Project/
│       └── KQC7029_IoT_Aquarium_Cooler_Project.ino   # Arduino firmware for ESP32
├── pictures/
│   ├── Components.jpg          # Bill of materials photo
│   ├── Front View.jpg          # Front of assembled prototype
│   ├── Angle View.jpg          # Angled view
│   ├── Top View.jpg            # Top-down layout
│   ├── Fan View.jpg            # Fan + tank close-up
│   ├── Mobile Dashboard.jpg    # Blynk mobile dashboard
│   └── Web Dashboard.png       # Blynk web dashboard
└── report/                     # Final report (uploaded once completed)
```

A project demonstration video will be added to this repository once recording is finalised.

---

## Build and Flash

Firmware is a single Arduino sketch targeting the ESP32 DevKit V1.

### Required Arduino libraries
- `Blynk` by Volodymyr Shymanskyy (`BlynkSimpleEsp32.h`)
- `OneWire`
- `DallasTemperature`
- `LiquidCrystal_I2C`

### Configuration
Before flashing, edit the placeholders at the top of `KQC7029_IoT_Aquarium_Cooler_Project.ino`:

```cpp
#define BLYNK_TEMPLATE_ID    "INSERT_BLYNK_ID"
#define BLYNK_TEMPLATE_NAME  "INSERT_BLYNK_NAME"
#define BLYNK_AUTH_TOKEN     "INSERT_BLYNK_AUTH_TOKEN"
char ssid[] = "INSERT_WIFI_SSID";
char pass[] = "INSERT_WIFI_PASS";
```

### Pinout

| Signal | ESP32 GPIO |
|---|---|
| DS18B20 data (1-Wire) | 32 |
| Relay (fan) | 27 |
| Buzzer | 14 |
| Green LED | 26 |
| Yellow LED | 25 |
| Red LED | 33 |
| LCD SDA | 21 |
| LCD SCL | 22 |

### Flash
Open the sketch in the Arduino IDE, select the **ESP32 Dev Module** board, the correct serial port, and upload.

---

## Demo Dashboards

| Mobile (Blynk) | Web (Blynk) |
|---|---|
| ![Mobile dashboard](pictures/Mobile%20Dashboard.jpg) | ![Web dashboard](pictures/Web%20Dashboard.png) |

---

## Status

- [x] Hardware assembly
- [x] Firmware (auto + manual mode, LCD, LEDs, buzzer, Blynk integration)
- [x] Blynk mobile and web dashboards
- [ ] Final report
- [ ] Demonstration video
