#include "FlashMemory.h"

const char* LAST_TEMP = "lastTemp";


// Define the static Preferences instance
Preferences FlashMemory::prefs;

void FlashMemory::begin() {
    // "appdb" is the NVS namespace; you can name it whatever you like
    // second argument = readOnly? false because we'll write to it
    prefs.begin("appdb", false);
}

void FlashMemory::setLastTemperature(float tempC) {
    // Store as a float value with key "lastTemp"
    prefs.putFloat(LAST_TEMP, tempC);
}

float FlashMemory::getLastTemperature() {
    // If key doesn't exist yet, return some default (e.g. NAN or 0.0)
    if (!prefs.isKey(LAST_TEMP)) {
        return NAN;  // you can return 0.0 if you prefer
    }
    return prefs.getFloat(LAST_TEMP);
}
