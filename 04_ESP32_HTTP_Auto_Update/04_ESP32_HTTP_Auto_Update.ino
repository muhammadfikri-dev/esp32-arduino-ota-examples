/**
 * =========================================================================================
 * 04_ESP32_HTTP_Auto_Update.ino
 * Cloud Auto-Update Client (Checks Remote Server for Firmware Manifest & Downloads .bin)
 * Features: Version comparison, Automatic download, HTTPS/HTTPStream OTA, Zero-User-Action
 * =========================================================================================
 */

#include <WiFi.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <ArduinoJson.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

const char* CURRENT_FIRMWARE_VERSION = "1.0.0";
const char* MANIFEST_URL = "https://raw.githubusercontent.com/muhammadfikri-dev/esp32-firmware-releases/main/version.json";

void checkForUpdates() {
  Serial.printf("[AUTO-UPDATE] Checking for new firmware manifest at: %s\n", MANIFEST_URL);
  
  HTTPClient http;
  http.begin(MANIFEST_URL);
  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    StaticJsonDocument<256> doc;
    deserializeJson(doc, payload);

    const char* latestVersion = doc["version"];
    const char* binaryUrl     = doc["url"];

    Serial.printf("[AUTO-UPDATE] Current Version: %s | Remote Version: %s\n", CURRENT_FIRMWARE_VERSION, latestVersion);

    if (String(latestVersion) != String(CURRENT_FIRMWARE_VERSION)) {
      Serial.printf("[AUTO-UPDATE] New version found! Downloading from: %s\n", binaryUrl);
      
      WiFiClient client;
      t_httpUpdate_return ret = httpUpdate.update(client, binaryUrl);

      switch (ret) {
        case HTTP_UPDATE_FAILED:
          Serial.printf("[AUTO-UPDATE ERROR] Failed (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
          break;
        case HTTP_UPDATE_NO_UPDATES:
          Serial.println("[AUTO-UPDATE] No updates needed.");
          break;
        case HTTP_UPDATE_OK:
          Serial.println("[AUTO-UPDATE] Update Successful! Rebooting...");
          break;
      }
    } else {
      Serial.println("[AUTO-UPDATE] Firmware is up to date.");
    }
  } else {
    Serial.printf("[AUTO-UPDATE] Manifest check failed (HTTP %d)\n", httpCode);
  }
  http.end();
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("\n[WIFI] Connected! Running Firmware v%s\n", CURRENT_FIRMWARE_VERSION);

  // Check update on boot
  checkForUpdates();
}

void loop() {
  // Check for updates every 6 hours
  static unsigned long lastCheck = 0;
  if (millis() - lastCheck > (6UL * 3600UL * 1000UL)) {
    lastCheck = millis();
    checkForUpdates();
  }
  delay(1000);
}
