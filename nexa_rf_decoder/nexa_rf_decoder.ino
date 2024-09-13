/*
   AM-HRR3 Receiver
   Decoding Homeeasy protocol using Manchester encoding.
   This code can be used for sniffing the device codes from Nexa remotes,
   this makes them possible to emulate with the Nexa Library (https://github.com/erix/NexaTransmitter).

   This code is based on the rf_reciever in
   https://github.com/TokyoBirdy/Arduino-Nexa (Authors: Barnaby Gray (12/2008), Peter Mead (09/2009))
   This code works is the same way, but the it is shortened/simplified.

   Message structure:
   - 32 bits of data (encoded as 64 wire bits):
     - bits 0-25: group code (26-bit number)
     - bit 26: group flag
     - bit 27: on/off flag
     - bits 28-31: device code (4-bit number)
*/

const int rxPin = 2;
const unsigned long LATCH1_LOWER = 9480, LATCH1_UPPER = 10350;
const unsigned long LATCH2_LOWER = 2550, LATCH2_UPPER = 2700;
const unsigned long BIT_ZERO_LOWER = 200, BIT_ZERO_UPPER = 365;
const unsigned long BIT_ONE_LOWER = 1000, BIT_ONE_UPPER = 1360;

void setup()
{
  pinMode(rxPin, INPUT);
  Serial.begin(9600);
}

void loop()
{
  unsigned long sender = 0;
  bool group = false, on = false;
  unsigned int recipient = 0;

  if (!validateLatch(LATCH1_LOWER, LATCH1_UPPER) || !validateLatch(LATCH2_LOWER, LATCH2_UPPER))
  {
    return;
  }

  if (parseData(sender, group, on, recipient))
  {
    printResult(sender, group, on, recipient);
  }
}

bool validateLatch(unsigned long lower, unsigned long upper)
{
  unsigned long t = pulseIn(rxPin, LOW, 1000000);
  return t >= lower && t <= upper;
}

bool parseData(unsigned long &sender, bool &group, bool &on, unsigned int &recipient)
{
  byte prevBit = 0;
  int i = 0;

  while (i < 64)
  {
    unsigned long t = pulseIn(rxPin, LOW, 1000000);
    byte bit = (t > BIT_ZERO_LOWER && t < BIT_ZERO_UPPER) ? 0 : (t > BIT_ONE_LOWER && t < BIT_ONE_UPPER) ? 1
                                                                                                         : 255;

    if (bit == 255 || (i % 2 == 1 && (prevBit ^ bit) == 0))
    {
      return false;
    }

    if (i % 2 == 1)
    {
      if (i < 53)
        sender = (sender << 1) | prevBit;
      else if (i == 53)
        group = prevBit;
      else if (i == 55)
        on = prevBit;
      else
        recipient = (recipient << 1) | prevBit;
    }

    prevBit = bit;
    ++i;
  }
  return true;
}

void printResult(unsigned long sender, bool group, bool on, unsigned int recipient)
{
  Serial.print("Sender: ");
  Serial.println(sender);
  Serial.println(group ? "Group command" : "No group");
  Serial.println(on ? "ON" : "OFF");
  Serial.print("Recipient: ");
  Serial.println(recipient);
  Serial.println();
}
