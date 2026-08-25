/**
 * =========================================================================================
 * 05_ESP32_Dual_Partition_Rollback.ino
 * Advanced Fail-Safe Dual-Partition OTA with Automated Rollback Engine
 * Features: Uses esp_ota_ops.h, Boot confirmation, Self-healing automatic rollback if crashed
 * =========================================================================================
 */

#include <WiFi.h>
#include <esp_ota_ops.h>
#include <esp_system.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

void inspectPartitionStatus() {
  const esp_partition_t *running = esp_ota_get_running_partition();
  const esp_partition_t *boot    = esp_ota_get_boot_partition();

  Serial.println("\n==================================================");
  Serial.printf("Running Partition: %s (Address 0x%08x)\n", running->label, running->address);
  Serial.printf("Boot Partition   : %s (Address 0x%08x)\n", boot->label, boot->address);

  esp_ota_img_states_t ota_state;
  if (esp_ota_get_state_partition(running, &ota_state) == ESP_OK) {
    if (ota_state == ESP_OTA_IMG_PENDING_VERIFY) {
      Serial.println("[OTA STATUS] First boot of new firmware pending validation!");
      // Simulate hardware self-test...
      delay(2000);
      bool selfTestPassed = true; // Set false to trigger auto rollback

      if (selfTestPassed) {
        Serial.println("[OTA VERIFIED] Firmware confirmed valid and healthy! Cancelling rollback flag.");
        esp_ota_mark_app_valid_cancel_rollback();
      } else {
        Serial.println("[FAIL-SAFE TRIGGERED] Hardware self-test failed! Rolling back to previous firmware partition...");
        esp_ota_mark_app_invalid_rollback_and_reboot();
      }
    } else if (ota_state == ESP_OTA_IMG_VALID) {
      Serial.println("[OTA STATUS] Firmware is verified and active (STABLE).");
    }
  }
  Serial.println("==================================================\n");
}

void setup() {
  Serial.begin(115200);
  inspectPartitionStatus();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("\n[WIFI] Connected! Dual-Partition Node IP: %s\n", WiFi.localIP().toString().c_str());
}

void loop() {
  delay(1000);
}
