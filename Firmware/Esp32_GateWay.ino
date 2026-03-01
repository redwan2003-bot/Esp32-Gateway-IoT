#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "secrets.h"

// --- Configuration ---
const float TEMP_THRESHOLD = 40.0; // Day 07 Safety Logic
const int STATUS_LED = 2;          // Onboard LED

WiFiClientSecure espClient;
PubSubClient client(espClient);

// --- FreeRTOS Task Handles ---
TaskHandle_t TelemetryTask;

void setup() {
    Serial.begin(115200);
    pinMode(STATUS_LED, OUTPUT);

    // Day 03: Secure WiFi & SSL Setup
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    espClient.setCACert(NODE_RED_CA_CERT); 
    client.setServer(MQTT_BROKER, 8883);
    
    // Day 02: Multitasking Initialization
    xTaskCreatePinnedToCore(sendTelemetry, "Telemetry", 4096, NULL, 1, &TelemetryTask, 1);
}

void sendTelemetry(void * pvParameters) {
    for(;;) {
        if (client.connected()) {
            float coreTemp = temperatureRead(); // Internal ESP32 Temp
            
            // Day 07: Autonomous Edge Brain
            if (coreTemp > TEMP_THRESHOLD) {
                digitalWrite(STATUS_LED, HIGH); // Immediate local response
                client.publish("gateway/alerts", "{\"status\":\"CRITICAL\", \"msg\":\"Overheat\"}");
            } else {
                digitalWrite(STATUS_LED, LOW);
            }

            // Day 04: JSON Data Persistence
            String payload = "{\"cpu_temp\":" + String(coreTemp) + "}";
            client.publish("gateway/telemetry", payload.c_str());
        }
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

void loop() {
    if (!client.connected()) {
        // Reconnect Logic
    }
    client.loop();
}
