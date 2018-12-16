#include <Adafruit_NeoPixel.h>
#include "ESP8266WiFi.h"
const int led_pin = 13;
const int NUMPIXELS = 60;
int numberOfWiFi = 0;
int Last_numberOfWiFi = 0;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, led_pin, NEO_GRB + NEO_KHZ800);

boolean isWifiPowerMode = true;

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  Serial.begin(115200);
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  Serial.println("Setup done");
}

void loop() {
  Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int numberOfWiFi = WiFi.scanNetworks();
  if (numberOfWiFi == 0)
    Serial.println("no networks found");
  else
  {
    Serial.println(numberOfWiFi);
    Serial.println(Last_numberOfWiFi);
  }
  Serial.println("");

  if (numberOfWiFi < Last_numberOfWiFi) {
    for (int m = Last_numberOfWiFi; m > numberOfWiFi; m--) {
      strip.setPixelColor(m, strip.Color(0, 0, 0));
      strip.show();
      delay(500);
    }
  } else {
    if(isWifiPowerMode){
      int maxRSSI = -1;
      String maxRSSI_SSID = "";
      for(int i = 0;i < numberOfWiFi;++i){
        String ssid = WiFi.SSID(i);
        // unsigned
        int rssi = WiFi.RSSI(i) + 128;
        if(maxRSSI < rssi){
          maxRSSI = rssi;
          maxRSSI_SSID = ssid;
        }
      }
      // all of color is maxRSSI.
      for(int i = 0;i < numberOfWiFi;++i){
        strip.setPixelColor(i, strip.Color(maxRSSI % 255, 32, 0));
      }
      strip.show();
      delay(500);
      return;
    }
    for (int n = Last_numberOfWiFi; n < numberOfWiFi; n++) {
      if (n < 10) {
        strip.setPixelColor(n, strip.Color(0, 32, 0));//Green
      } else if (n < 25) {
        strip.setPixelColor(n, strip.Color(n * 12 - 42, 32, 0));
      } else {
        strip.setPixelColor(n, strip.Color(218, 0, 0));
      }
      strip.show(); // This sends the updated pixel color to the hardware.
      delay(500);
    }
  }
  Last_numberOfWiFi  = numberOfWiFi;
  delay(50);
}

void voidOutput(int num) {
  // Print SSID and RSSI for each network found
  Serial.print(num);
  Serial.print(": ");
  Serial.print(WiFi.SSID(num));
  Serial.print(" (");
  Serial.print(WiFi.RSSI(num));
  Serial.print(")");
  Serial.println((WiFi.encryptionType(num) == ENC_TYPE_NONE) ? " " : "*");
}
