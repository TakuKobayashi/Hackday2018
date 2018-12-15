/*
 *  This sketch demonstrates how to scan WiFi networks. 
 *  The API is almost the same as with the WiFi Shield library, 
 *  the most obvious difference being the different file you need to include:
 */

#include <Adafruit_NeoPixel.h>
#include "ESP8266WiFi.h"

#define LEDPIN        13
#define NUMPIXELS     16

Adafruit_NeoPixel pix = Adafruit_NeoPixel(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);
uint8_t LEDr = 0, LEDg = 0, LEDb = 0, fBlink = 0;
#define BLINK_INTERVAL 500 // [ms], 1000->500

void set_led(int n, int r, int g, int b, uint8_t f_blink){
  LEDr = r; LEDg = g; LEDb = b;
  fBlink = f_blink;
  pix.setPixelColor(n, pix.Color(LEDr, LEDg, LEDb));
  pix.show();
}

void LEDblink()
{
  if (fBlink != 0){
    if (fBlink == 1){
      pix.setPixelColor(0, pix.Color(LEDr, LEDg, LEDb));
      fBlink = 2;
    }
    else if (fBlink == 2){
      pix.setPixelColor(0, pix.Color(0, 0, 0));
      fBlink = 1;
    }
    pix.show();
  }
}

void setup() {
  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  pix.begin(); // This initializes the NeoPixel library.

  Serial.println("Setup done");
}

int i = 0;
void loop() {
  Serial.println("scan start");

 if(i >= NUMPIXELS){
    for(int j = 0; j < NUMPIXELS; j++){
        set_led(j, 0, 0, 0, 0);
    }
    i = 0;
  }

  set_led(i, 128, 128, 128, 0);

  i++;

  Serial.println(i);
 
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      delay(10);
    }
  }
  Serial.println("");

  // Wait a bit before scanning again
  delay(1000);
}
