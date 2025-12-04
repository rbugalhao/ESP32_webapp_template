#include "secrets.h"
#include <WiFi.h>
#include "web_assets.h"
#include "Resource.h" 
#include "Database.h"

// ====== WiFi config ======
const char* SSID     = WIFI_SSID;
const char* PASSWORD = WIFI_PASSWORD;

WebServer server(80);

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

  
  Database::begin();
  webapp_begin();

}

long lastTempUpdate = 0;

void loop() {
  server.handleClient();
  readTemperature(lastTempUpdate);
}

// Read temperature every second and store in db
void readTemperature(long &lastTempRead) {
  long now = millis();
  if (now - lastTempUpdate >= 1000) {
      lastTempUpdate = now;
      float temp = temperatureRead();
      Database::setLastTemperature(temp);
      Serial.println("Stored temperature: " + String(temp, 2) + " °C");
  }
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
