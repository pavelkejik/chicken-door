# 🐔 Automated Chicken Coop Ecosystem  
_A modular IoT platform for stress-free backyard chicken keeping_

## Contents
1. [Project Overview](#project-overview)  
2. [Core Features](#core-features)  
3. [System Architecture](#system-architecture)  
4. [Hardware](#hardware)  
   1. [Chicken Door (Gateway)](#chicken-door-gateway)  
   2. [Accessories](#accessories)  
5. [Communication & APIs](#communication--apis)  
6. [Safety & Reliability](#safety--reliability)  
7. [Getting Started](#getting-started)  
8. [MQTT Quick Reference](#mqtt-quick-reference)  
9. [Roadmap](#roadmap)  

---

## Project Overview
The project automates everyday coop duties by combining a **smart sliding door**, a set of battery-powered **accessories** (feeder, camera, …) and a modern web UI.  
The door acts as the **gateway**: it drives the motor, pairs with accessories over low-power **ESP-NOW**, exposes a local **Web UI + HTTP API**, and bridges everything to standard **MQTT** and optional **Modbus RTU** for seamless Home-Assistant integration.

---

## Core Features
| Area | Highlights |
|------|------------|
|Automation|• Auto-open/close by **sunrise/sunset** (astronomical) or **ambient light** plus configurable offsets  <br>• Full manual override via button, Web UI, MQTT, or Modbus|
|Safety|• Encoder-based position tracking, upper/lower limit calibration, soft-start/stop  <br>• Stall/over-current detection, timeout watchdog, emergency-stop state|
|Connectivity|• Wi-Fi STA/AP, **ESP-NOW** for accessories (1 km LR)  <br>• MQTT (via configurable `base_topic`), HTTP/REST, captive portal|
|Updates|• **OTA** firmware & file-system updates for door *and* every paired accessory through ESP-NOW|
|Accessories|• **Smart Feeder** – scheduled or weight-based servings  <br>• **Coop Camera** – still images on demand  <br>_Add your own!_  |
|Power|Door on 12 V DC; accessories battery-powered for months thanks to deep-sleep design|

---

## System Architecture
```
ESP32-S3 Door  ── Wi-Fi STA ⇆ Home Network ⇆ MQTT Broker / Home Assistant
      │
      ├─ Wi-Fi AP (for first-run config)
      └─ ESP-NOW P2P
             ├─ Feeder (ESP32-C3, battery)
             └─ Camera (ESP32-S3, battery)
```
Door firmware is C++/Arduino on FreeRTOS; the web interface is Vite + Vue 3 + Vuetify served directly from on-board **LittleFS**.

---

## Hardware
### Chicken Door (Gateway)
* **Drive**: 12 V DC gear-motor + rack-and-pinion (self-locking)  
* **MCU**: ESP32-S3-WROOM-1 (8 MB PSRAM)  
* **Sensors**: quadrature encoder, NTC temperature, photodiode (light)  
* **IO Expansion**: 2 GPIO inputs, open-collector output, RS-485 (Modbus RTU)  
* **Indicators**: bi-color status LED

### Accessories
| Name | Repo | Brief |
|------|------|-------|
|Smart Feeder|<https://github.com/pavelkejik/chicken-feeder>|Servo-lid + load-cell; daily schedule or “feed-to-weight” mode|
|Coop Camera|<https://github.com/pavelkejik/chicken-camera>|ESP32-S3 w/ OV2640; images send over ESP-NOW|

Each accessory ships its own OTA-updatable firmware and pairs with one click via the Web UI. Status appears alongside the door dashboard.

---

## Communication & APIs
| Layer | Tech | Notes |
|-------|------|-------|
|Local Web UI|`http://dvirka.local` (mDNS) or captive portal |Full config, live telemetry, manual commands|
|REST/JSON|`/api/door`, `/api/accessory/{id}`|Same parameter map as MQTT|
|MQTT|`<base_topic>/state/...` and `/set/...` |Door & all accessories share a unified schema|
|Modbus RTU|RS-485, 9600 8N1 |Minimal register map for industrial PLCs|
|ESP-NOW|1 Mbit/s (500 kbit/s LR) |door = server, accessories = clients|

---

## Safety & Reliability
* **Hard limits + soft position** ensure gentle landings and no unexpected slams.  
* Every motion path is guarded by current & timeout watchdogs; any fault triggers _Emergency Stop_ with MQTT/HTTP event.  
* Door refuses to auto-close if position => `UNKNOWN` until next recalibration pass.  
* Accessories inherit door time-base and watchdog pings to detect battery loss.

---

## Getting Started
### 1. Flash Firmware
```bash
# Door
cd firmware/door
pio run -t upload         # USB
pio run -t uploadfs       # Web assets
# Feeder / Camera similar...
```

### 2. First Boot & Wi-Fi Join
1. Hold the door’s button >3 s → it starts `Dvirka` AP (both LEDs solid).  
2. Connect with a phone, open any page → captive portal shows config.  
3. Enter your SSID/PSK; the door reboots and joins STA mode.  

### 3. Build the Web UI Locally (optional)
```bash
cd web-ui
npm i
npm run dev
```

### 4. Pair Accessories
Open **Menu → Accessories → “New” → Pair** and press the accessory button once. State flips from _New_ → _Connected_.

---

## MQTT Quick Reference
```text
<base_topic>/state/position        = "OPEN" | "CLOSED" | "OPENING" | ...
<base_topic>/state/light_lux       = 123
<base_topic>/set/command           ← "OPEN" | "CLOSE" | "STOP"

<base_topic>/devices/<id>/state/weight_g
```
Replace `<base_topic>` in the Web UI.

---

## Roadmap
- [ ] Add waterer accessory
- [ ] Implement chicken presence counting using RFID tags   
- [x] Publish 3D-print STLs & KiCad sources

---

### Author
**Pavel Kejík**

> Built for my family’s coop, released for every backyard chicken keeper 🤘