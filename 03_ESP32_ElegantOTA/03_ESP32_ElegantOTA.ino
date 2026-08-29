/**
 * =========================================================================================
 * 03_ESP32_ElegantOTA.ino
 * ElegantOTA Implementation for Firmware & LittleFS Web Updating
 * Library: ElegantOTA by Ayush Sharma
 * =========================================================================================
 */

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

AsyncWebServer server(80);

void onOTAStart() {
 Serial.println("[ELEGANT OTA] Update process initiated...");
}

void onOTAProgress(size_t current, size_t final) {
 Serial.printf("[ELEGANT OTA] Progress: %u / %u bytes (%.1f%%)\r", current, final, (float)current / (float)final * 100.0);
}

void onOTAEnd(bool success) {
 if (success) {
 Serial.println("\n[ELEGANT OTA] Update completed successfully! Rebooting...");
 } else {
 Serial.println("\n[ELEGANT OTA] Update failed!");
 }
}

void setup() {
 Serial.begin(115200);

 WiFi.mode(WIFI_STA);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 }
 Serial.printf("\n[WIFI] Connected! ElegantOTA URL: http://%s/update\n", WiFi.localIP().toString().c_str());

 server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
 request->send(200, "text/plain", "Welcome to Smart Node. Go to /update for OTA.");
 });

 // Initialize ElegantOTA with callbacks and credentials
 ElegantOTA.begin(&server, "admin", "admin123");
 ElegantOTA.onStart(onOTAStart);
 ElegantOTA.onProgress(onOTAProgress);
 ElegantOTA.onEnd(onOTAEnd);

 server.begin();
}

void loop() {
 ElegantOTA.loop();
 delay(10);
}
