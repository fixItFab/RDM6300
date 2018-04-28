/*
  RFID RDM6300 library
  Created by Fabian Grebe
  Version 0.1

  Based on https://gist.github.com/maniacbug/1273970 by maniacbug
*/

#ifndef RDM6300_H
#define RDM6300_H

#include "Arduino.h"
#include "RDM6300.h"
#include "Stream.h"

class RDM6300 {
    public:
        RDM6300(Stream& stream);
        void handleRfid();
        bool isReady;
        uint32_t rfidTag;
    private:
        bool _checkAndConvert();
        uint8_t _rfidGetNext();
        Stream& _rfid;
        uint8_t _buffer[12];
        uint8_t * _bufferAt;
        uint8_t * _bufferEnd;
        int _seq;
};

#endif

