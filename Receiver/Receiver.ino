#include <SPI.h>
#include <LoRa.h>

// LoRa connect config
#define NSS_PIN    5
#define NRESET_PIN 8
#define DIO0_PIN   9

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Node - Receiver");

  LoRa.setPins(NSS_PIN, NRESET_PIN, DIO0_PIN);
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while(1);
  }
  LoRa.setTimeout(100);
}

void loop() {
  if (LoRa.parsePacket() > 0) {
    String text = LoRa.readString();
    Serial.print("Receiver '");
    Serial.print(text);
    Serial.print("' RSSI is ");
    Serial.println(LoRa.packetRssi());
  }
  delay(1);
}
