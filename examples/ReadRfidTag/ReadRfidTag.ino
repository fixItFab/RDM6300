#include "Arduino.h"
#include "RDM6300.h"
#include "SoftwareSerial.h"

SoftwareSerial rfid(D2, -1);

RDM6300 RDM6300(rfid);

void setup() { 
  Serial.begin(115200); 
}

void loop() {
  RDM6300.handleRfid();

  // check if valid tag has been read
  if (RDM6300.isReady) {
    Serial.print("RFIDTAG:");
    Serial.println(RDM6300.rfidTag);

    // ready for new tag
    RDM6300.isReady = false;
  }
}