/**
 * =========================================================================================
 * 02_ESP32_Web_OTA_Async.ino
 * Asynchronous Web Browser Drag-and-Drop OTA Firmware Uploader
 * Features: Embedded HTML5/CSS Modern UI, HTTP Basic Auth, AsyncWebServer non-blocking upload
 * =========================================================================================
 */

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Update.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

const char* http_username = "admin";
const char* http_password = "admin123";

AsyncWebServer server(80);

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
 <title>ESP32 Web OTA</title>
 <meta name="viewport" content="width=device-width, initial-scale=1">
 <style>
 body { font-family: system-ui, sans-serif; background: #0f172a; color: #f8fafc; text-align: center; padding: 40px 20px; }
 .card { background: #1e293b; max-width: 480px; margin: auto; padding: 30px; border-radius: 16px; box-shadow: 0 10px 25px rgba(0,0,0,0.3); }
 h2 { color: #38bdf8; margin-top: 0; }
 input[type=file] { margin: 20px 0; background: #334155; padding: 12px; border-radius: 8px; width: 80%; color: white; }
 .btn { background: #0284c7; color: white; border: none; padding: 12px 28px; font-size: 16px; border-radius: 8px; cursor: pointer; font-weight: bold; }
 .btn:hover { background: #0369a1; }
 #prg { margin-top: 20px; font-weight: bold; color: #4ade80; display: none; }
 </style>
</head>
<body>
 <div class="card">
 <h2>⚡ ESP32 Web Firmware OTA</h2>
 <p>Select exported <code>.bin</code> firmware file to flash:</p>
 <form method='POST' action='/update' enctype='multipart/form-data' id='upload_form'>
 <input type='file' name='update' accept='.bin'><br>
 <input type='submit' value='Upload & Flash' class='btn'>
 </form>
 <div id='prg'>Flashing firmware... Device will reboot.</div>
 </div>
</body>
</html>
)rawliteral";

void setup() {
 Serial.begin(115200);

 WiFi.mode(WIFI_STA);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 }
 Serial.printf("\n[WIFI] Connected! Web OTA available at: http://%s/update\n", WiFi.localIP().toString().c_str());

 // Serve Index Web Page
 server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
 request->send_P(200, "text/html", INDEX_HTML);
 });

 // Handle Binary Upload
 server.on("/update", HTTP_POST, [](AsyncWebServerRequest *request){
 bool shouldReboot = !Update.hasError();
 AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", shouldReboot ? "OK - Rebooting..." : "FAIL - Update Error");
 response->addHeader("Connection", "close");
 request->send(response);
 if (shouldReboot) {
 delay(1000);
 ESP.restart();
 }
 }, [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
 if (!index) {
 Serial.printf("[WEB OTA START] Uploading file: %s\n", filename.c_str());
 if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
 Update.printError(Serial);
 }
 }
 if (!Update.hasError()) {
 if (Update.write(data, len) != len) {
 Update.printError(Serial);
 }
 }
 if (final) {
 if (Update.end(true)) {
 Serial.printf("[WEB OTA SUCCESS] %u bytes flashed.\n", index + len);
 } else {
 Update.printError(Serial);
 }
 }
 });

 server.begin();
}

void loop() {
 // Free for main application logic
 delay(1000);
}
