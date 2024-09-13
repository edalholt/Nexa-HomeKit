#include "HomeSpan.h"
#include "NexaDimSocket.h"

#define TX_433_MHZ_PIN 14
#define NEXA_REMOTE_ID 12345678

#define DHTPIN 15
#define DHTTYPE DHT22

void setup() {
  Serial.begin(115200);

  homeSpan.setPairingCode("11122333");

  homeSpan.begin(Category::Bridges, "Nexa Bridge");

  new SpanAccessory();
    new Service::AccessoryInformation();
    new Characteristic::Identify();
    new Characteristic::Name("Ceiling light");
    new NexaDimSocket(TX_433_MHZ_PIN, 10, NEXA_REMOTE_ID);
}

void loop() {
  homeSpan.poll();
}
