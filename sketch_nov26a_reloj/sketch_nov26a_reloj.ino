#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "time.h"

#define SDA_PIN 8
#define SCL_PIN 9

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Tu WiFi:
const char* ssid     = "Sebas";
const char* password = "24840729";

// Ajustes de NTP (hora / fecha)
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -10800;  // Uruguay UTC-3
const int daylightOffset_sec = 0;

// Coordenadas de Montevideo
const float LAT = -34.90;
const float LON = -56.17;

// URL de Open-Meteo para tiempo actual
String weatherURL = String(
  "https://api.open-meteo.com/v1/forecast?"
  "latitude=") + LAT +
  "&longitude=" + LON +
  "&current_weather=true"
;

float temperatura = 0;
unsigned long lastRequest = 0;

void setup() {
  Wire.begin(SDA_PIN, SCL_PIN);
  lcd.init();
  lcd.backlight();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
  }

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  obtenerTemperatura();
}

void obtenerTemperatura() {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  http.begin(weatherURL);
  int code = http.GET();
  if (code == 200) {
    String payload = http.getString();
    int idx = payload.indexOf("\"temperature\":");
    if (idx != -1) {
      // extraer valor de temperatura
      String t = payload.substring(idx + 14);
      temperatura = t.toFloat();
    }
  }
  http.end();
}

void loop() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return;
  }

  // Cada 10 minutos actualiza la temperatura
  if (millis() - lastRequest > 600000) {
    obtenerTemperatura();
    lastRequest = millis();
  }

  // Línea 1: fecha + año + temperatura
  lcd.setCursor(0,0);
  lcd.printf("%02d/%02d/%04d %2.0f", 
    timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900,
    temperatura);
  lcd.write(223);    // símbolo °
  lcd.print("C   "); // la C de Celsius + espacios libres

  // Línea 2: hora
  lcd.setCursor(0,1);
  lcd.printf("Hora: %02d:%02d:%02d",
    timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

  delay(1000);
}


