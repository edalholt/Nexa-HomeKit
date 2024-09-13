#ifndef NEXADIMSOCKET_H
#define NEXADIMSOCKET_H

#include "HomeSpan.h"
#include <NexaTransmitter.h>

struct NexaDimSocket : Service::LightBulb {

  short txPin;
  short recipient;
  unsigned long remoteId;

  SpanCharacteristic *power;
  SpanCharacteristic *level;

  NexaDimSocket(short txPin, short recipient, unsigned long remoteId);

  bool update();
  
private:
  NexaTransmitter remote;
};

#endif // NEXADIMSOCKET_H
