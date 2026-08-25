/**
 * =========================================================================================
 * 01_ArduinoOTA_Basic.ino
 * Production-ready ArduinoOTA for ESP32 & ESP8266
 * Features: MDNS discovery, Password authentication, Port selection, Progress percentage
 * =========================================================================================
 */

#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

#define PIN_STATUS_LED 2

void setup() {
  Serial.begin(115200);
  pinMode(PIN_STATUS_LED, OUTPUT);
  digitalWrite(PIN_STATUS_LED, LOW);

  Serial.println("\n[BOOT] Starting ArduinoOTA Node...");

  // 1. Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting in 5s...");
    delay(5000);
    ESP.restart();
  }
  Serial.printf("[WIFI] Connected! IP Address: %s\n", WiFi.localIP().toString().c_str());

  // 2. Configure ArduinoOTA
  ArduinoOTA.setPort(3232); // Default ESP32 OTA Port
  ArduinoOTA.setHostname("esp32-ota-node");
  ArduinoOTA.setPassword("admin123"); // Password Protection

  // 3. Register Event Callbacks
  ArduinoOTA
    .onStart([]() {
      String type = (ArduinoOTA.getCommand() == U_FLASH) ? "sketch" : "filesystem";
      Serial.println("[OTA START] Updating " + type + "...");
      digitalWrite(PIN_STATUS_LED, HIGH);
    })
    .onEnd([]() {
      Serial.println("\n[OTA END] Update Successful! Rebooting device...");
      digitalWrite(PIN_STATUS_LED, LOW);
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("[OTA PROGRESS] %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("[OTA ERROR][%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  // 4. Start OTA Engine
  ArduinoOTA.begin();
  Serial.println("[OTA READY] Device ready to receive wireless updates from Arduino IDE.");
}

void loop() {
  // MUST call ArduinoOTA.handle() in every loop cycle
  ArduinoOTA.handle();

  // User application code
  static unsigned long lastBlink = 0;
  if (millis() - lastBlink > 1000) {
    lastBlink = millis();
    digitalWrite(PIN_STATUS_LED, !digitalRead(PIN_STATUS_LED));
    Serial.printf("[HEARTBEAT] Free Heap: %d bytes | Uptime: %lu s\n", ESP.getFreeHeap(), millis() / 1000);
  }
}
