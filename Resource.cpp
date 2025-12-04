#include "Resource.h"
#include <Arduino.h>
#include "FlashMemory.h" 

// @GET("/path")
void getFunction() {
    float temp = FlashMemory::getLastTemperature();

    // Convert to JSON for easy parsing on the web app
    String json = "{ \"temperature\": " + String(temp, 2) + " }";
    Serial.println("GET /path -> " + json);

    server.send(200, "application/json", json);
}


// @POST("/path")
void setFunction() {
  
    if (!server.hasArg("param")) {
        server.send(400, "text/plain", "Missing param parameter");
        return;
    }
    
    String state = server.arg("param");
    Serial.println("POST /path -> " + state);

    if (state.equalsIgnoreCase("HELLO")) {
        server.send(200, "text/plain", "WORLD");
    } else {
        server.send(400, "text/plain", "Unknown param");
    }
}
