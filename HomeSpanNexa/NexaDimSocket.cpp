#include "NexaDimSocket.h"

NexaDimSocket::NexaDimSocket(short txPin, short recipient, unsigned long remoteId) 
  : Service::LightBulb(), txPin(txPin), recipient(recipient), remoteId(remoteId), remote(txPin, remoteId) {

  power = new Characteristic::On();
  level = new Characteristic::Brightness(100);
  level->setRange(5, 100, 1);
}

bool NexaDimSocket::update() {
  int brightness = power->getNewVal() * level->getNewVal();
  int mappedBrightness = map(brightness, 0, 100, 0, 15);

  Serial.print("Power: ");
  Serial.println(power->getNewVal());
  Serial.print("Level: ");
  Serial.println(level->getNewVal());
  Serial.print("Mapped Brightness: ");
  Serial.println(mappedBrightness);
  
  remote.setSwitch(power->getNewVal(), recipient, mappedBrightness);

  return true;
}
