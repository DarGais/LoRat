#include <SPI.h>
#include <LoRa.h>
#include "SSD1306.h" 


// LoRa connect config
#define NSS_PIN    5
#define NRESET_PIN 8
#define DIO0_PIN   9

String text = "Hello LoRa";

SSD1306  display(0x3c, 26, 27);

void setup() {
  
  Serial.begin(115200);
  while (!Serial);
  
  Serial.println("LoRa Node - Sender");

  display.init();
  //display.drawString(0, 26, "Hello world");
  //display.display();

  LoRa.setPins(NSS_PIN, NRESET_PIN, DIO0_PIN);
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while(1);
  }

  

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  //display.drawString(0, 26, "Hello world");
  //display.display();
  
}

void loop() {
  display.clear();
  display.drawString(10, 128, String(millis()));
  display.setFont(ArialMT_Plain_24);
  display.drawString(0, 26, "Hello world");
  display.display();
  
  LoRa.beginPacket();
  LoRa.print(text);
  LoRa.endPacket();

  Serial.print("Send '");
  Serial.print(text);
  Serial.println("'");
  delay(1000);
}
