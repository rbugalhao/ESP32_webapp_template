#pragma once
#include <Arduino.h>
#include <Preferences.h>

// Simple wrapper around ESP32 Preferences to act like a tiny "DB".
class Database {
public:
    // Must be called once in setup()
    static void begin();

    // Example: store & read last temperature
    static void setLastTemperature(float tempC);
    static float getLastTemperature();

private:
    static Preferences prefs;
};
