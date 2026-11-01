#include "core/wifi/webInterface.h"
#pragma once
#include <WiFi.h>
#include <Preferences.h>
#include <esp_system.h>
#include "core/wifi/webInterface.h"

// On first boot a random AP password is generated and stored in NVS.
// Read it via USB serial (115200 baud). Do not publish serial logs.
inline void skbStartHeadless() {
    Preferences p;
    if (!p.begin("skb-ap", false)) { Serial.println("[SKB] NVS unavailable"); return; }
    String pass = p.getString("password", "");
    if (pass.length() < 12 || pass.length() > 63) {
        static const char alphabet[] = "ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz23456789";
        pass.reserve(20);
        for (int i=0; i<20; ++i) pass += alphabet[esp_random() % (sizeof(alphabet)-1)];
        if (p.putString("password", pass) != pass.length()) {
            Serial.println("[SKB] Failed to store password"); p.end(); return;
        }
    }
    p.end();
    WiFi.mode(WIFI_AP);
    IPAddress gateway(192,168,4,1);
    if (!WiFi.softAPConfig(gateway,gateway,IPAddress(255,255,255,0))) {
        Serial.println("[SKB] AP IP configuration failed"); return;
    }
    if (!WiFi.softAP("SKB-Headless-Pro",pass.c_str(),6,false,4)) {
        Serial.println("[SKB] AP start failed"); return;
    }
    Serial.println("[SKB] SSID: SKB-Headless-Pro");
    Serial.println("[SKB] AP password (USB serial only): " + pass);
    Serial.println("[SKB] Browser: http://192.168.4.1/skb");
    // Existing Bruce web routes remain available, including authentication.
    startWebUi(true);
}
