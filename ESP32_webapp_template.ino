#include "secrets.h"
#include <WiFi.h>
#include "web_assets.h"
#include "Resource.h" 
#include "FlashMemory.h"

// ====== WiFi config ======
const char* SSID     = WIFI_SSID;
const char* PASSWORD = WIFI_PASSWORD;

WebServer server(80);

#define LED_PIN 15

void webapp_begin() {

  // API REQUESTS
  server.on("/path", HTTP_POST, setFunction);
  server.on("/path", HTTP_GET, getFunction);

  // Root
  server.on("/", HTTP_GET, handleRoot);

  server.onNotFound(handleStaticOrNotFound);
  server.begin();
  Serial.println("HTTP server started (WebApp)");
}
static void handleStaticOrNotFound() {
  String path = server.uri();
  if (path.length() == 0) {
    path = "/";
  }

  // Já tratámos "/", aqui só tratamos ficheiros estáticos
  if (path == "/") {
    handleRoot();
    return;
  }

  const WebAsset* asset = find_web_asset(path);
  if (asset) {
    server.send(200, asset->contentType, asset->data);
  } else {
    server.send(404, "text/plain", "Not found");
  }
}
static void handleRoot() {
  const WebAsset* asset = find_web_asset("/index.html");
  if (!asset) {
    server.send(404, "text/plain", "index.html not found");
    return;
  }

  // Copiamos o HTML para uma String, substituímos o placeholder, e enviamos
  String html = asset->data;
  String ip = WiFi.localIP().toString();
  html.replace("{{ESP_IP}}", ip);
  server.send(200, asset->contentType, html);
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("ESP32-C5 LED Web Server");
  Serial.print("Connecting to WiFi: ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected!");
  String ipStr = WiFi.localIP().toString();
  Serial.print("IP address: ");
  Serial.println(ipStr);

  
  FlashMemory::begin();
  webapp_begin();
  pinMode(LED_PIN, OUTPUT);

}

long lastTempUpdate = 0;
bool ledState = false;
long lastLedTurnOn = 0;

void loop() {
  long now = millis();
  server.handleClient();
  readTemperature(now, lastTempUpdate, ledState);
  blinkLED(now, lastLedTurnOn, ledState);
}

// Read temperature every second and store in db
void readTemperature(long now, long &lastTempRead, bool &ledState) {
  
  if (now - lastTempUpdate >= 1000) {
    ledState = true;
    lastTempUpdate = now;
    float temp = temperatureRead();
    FlashMemory::setLastTemperature(temp);
    Serial.println("Stored temperature: " + String(temp, 2) + " °C");
  }
}

// Blink LED when reading temperature
void blinkLED(long now, long &lastLedTurnOn, bool &ledState) {
  
  if (ledState) {
    digitalWrite(LED_PIN, HIGH);
    ledState = false;
    lastLedTurnOn = now;
  }
  else if (now - lastLedTurnOn >= 200) {
    digitalWrite(LED_PIN, LOW);
  }
}




