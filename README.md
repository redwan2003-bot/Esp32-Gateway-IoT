# ESP32-Gateway: Secure IoT Telemetry Gateway (Phase 1)

An ESP32-S3 gateway that publishes sensor telemetry to a cloud MQTT broker over TLS, with a local over-temperature alarm that works without cloud connectivity. Part of my #30DaysOfEngineering series (write-ups: https://medium.com/@reahs302444).

## Architecture

1. **Edge:** ESP32-S3 firmware (C++). A FreeRTOS telemetry task, pinned to one core, reads the internal temperature and publishes JSON.
2. **Transport:** MQTT over TLS (port 8883) to HiveMQ Cloud, with the server verified against a CA certificate.
3. **Orchestration:** Node-RED (Docker) routes messages.
4. **Storage and dashboards:** InfluxDB v2 and Grafana.

## What it does

- Publishes temperature telemetry as JSON over MQTT with TLS.
- Local over-temperature alarm (for example above 40 C) that runs on the device itself.
- Backend stack (Node-RED, InfluxDB, Grafana) for logging and visualisation.

## Status and limitations

- Reconnect handling is a stub and needs to be completed.
- The alarm currently publishes its event only while connected.
- Timing and uptime have not been measured yet. I will add real numbers here after testing.
