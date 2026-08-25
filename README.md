# 🚀 ESP32 & Arduino Over-The-Air (OTA) Mastery Guide & Examples

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Platform: ESP32 / Arduino](https://img.shields.io/badge/Platform-ESP32%20%7C%20Arduino-blue.svg)](https://espressif.com/)
[![Framework: Arduino IDE](https://img.shields.io/badge/Framework-Arduino%20IDE%202.0%2B-teal.svg)](https://www.arduino.cc/)
[![OTA Ready](https://img.shields.io/badge/OTA-Production%20Ready-green.svg)](#)
[![Portfolio: Laksanasoft](https://img.shields.io/badge/Portfolio-Laksanasoft-purple.svg)](#)

A comprehensive, production-ready reference repository containing **5 distinct Over-The-Air (OTA) firmware update implementations** for **ESP32** and **Arduino** microcontrollers. Includes network port flashing, web browser file uploaders, ElegantOTA filesystem sync, cloud GitHub auto-updates, and dual-partition fail-safe rollbacks.

---

## 📑 Included OTA Implementations

| Folder | OTA Method | Description | Best Used For |
|:---|:---|:---|:---|
| **[`01_ArduinoOTA_Basic`](./01_ArduinoOTA_Basic/)** | **Native ArduinoOTA** | Flash directly from Arduino IDE network port via mDNS & password authentication. | Development & Local Network flashing. |
| **[`02_ESP32_Web_OTA_Async`](./02_ESP32_Web_OTA_Async/)** | **Asynchronous Web OTA** | Upload `.bin` firmware file through any web browser (`/update`) with authentication. | Standalone devices without IDE access. |
| **[`03_ESP32_ElegantOTA`](./03_ESP32_ElegantOTA/)** | **ElegantOTA UI** | Modern HTML5 interface supporting both Firmware (`.bin`) and Filesystem (`LittleFS`/`SPIFFS`) updates. | Smart Home & Commercial IoT devices. |
| **[`04_ESP32_HTTP_Auto_Update`](./04_ESP32_HTTP_Auto_Update/)** | **Cloud HTTPS Auto-Update** | ESP32 checks a remote cloud JSON manifest and automatically downloads & flashes new releases. | Large fleet remote deployments. |
| **[`05_ESP32_Dual_Partition_Rollback`](./05_ESP32_Dual_Partition_Rollback/)** | **Dual Partition & Fail-Safe** | Two active OTA slots (`ota_0` and `ota_1`) with automated boot verification & rollback if firmware fails. | Mission-critical industrial devices. |

---

## 🧠 Memory Partition Layout (Flash Partition Table)

For OTA to function on ESP32, the flash memory must be divided into at least two app slots (`app0` and `app1`). 

### Default 4MB Flash Partition Scheme (`default_ota_4mb.csv`):
```
# Name,   Type, SubType, Offset,  Size, Flags
nvs,      data, nvs,     0x9000,  0x4000,
otadata,  data, ota,     0xd000,  0x2000,
phy_init, data, phy,     0xf000,  0x1000,
app0,     app,  ota_0,   0x10000, 0x140000, (1.25 MB)
app1,     app,  ota_1,   0x150000,0x140000, (1.25 MB)
spiffs,   data, spiffs,  0x290000,0x170000, (1.43 MB)
```

In **Arduino IDE**, select: **Tools > Partition Scheme > "Default 4MB with ffat (1.2MB APP/1.5MB FATFS)"** or **"Minimal SPIFFS (1.9MB APP with OTA)"**.

---

## 🚀 Quick Setup & Usage Guide

### Method 1: Flashing via Arduino IDE Network Port (ArduinoOTA)
1. Open [`01_ArduinoOTA_Basic/01_ArduinoOTA_Basic.ino`](./01_ArduinoOTA_Basic/01_ArduinoOTA_Basic.ino).
2. Flash the sketch initially using a standard USB Cable.
3. Once booted and connected to your Wi-Fi, open **Tools > Port** in Arduino IDE.
4. You will see a new network port under **Network Ports**: `esp32-ota-node at 192.168.x.x`.
5. Select the Network Port, enter password (`admin123`), and click **Upload**!

### Method 2: Web Browser Drag-and-Drop Flashing
1. In Arduino IDE, click **Sketch > Export Compiled Binary** (`Ctrl+Alt+S`).
2. Open your browser and navigate to: **`http://IP_ESP32_ANDA/update`**.
3. Login (User: `admin` | Pass: `admin123`).
4. Select the exported `.bin` file and click **Update**.

---

## 📄 License
Distributed under the **MIT License**. Created by **Muhammad Fikri** for **Laksanasoft Portfolio**.
