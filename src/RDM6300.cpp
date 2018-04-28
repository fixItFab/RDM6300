/*
  RFID RDM6300 library
  Created by Fabian Grebe
  Version 0.1

  Based on https://gist.github.com/maniacbug/1273970 by maniacbug
*/

#include "Arduino.h"
#include "RDM6300.h"
#include "Stream.h"

RDM6300::RDM6300(Stream& stream) : _rfid(stream) {
  _bufferAt = _buffer;
  _bufferEnd = _buffer + sizeof(_buffer);
  _seq = 0;
  isReady = false;
}

void RDM6300::handleRfid() {
  // Only read in values if there is not already a value waiting to be processed
  if (_rfid.available() <= 0 || isReady) {
    return;
  }

  // read next byte
  char c = _rfid.read();

  // check start byte (0x02)
  if (_seq == 0 && c == 0x02) {
    _seq++;
    return;
  }

  // Read characters into the buffer until it is full
  if (_seq == 1) {
    if (_bufferAt < _bufferEnd) {
      *_bufferAt++ = c;
    } else {
      _seq++;
    }
  }

  if (_seq == 2) {
    // check end byte (0x03)
    if (c == 0x03) {
      isReady = _checkAndConvert();
    }

    // reset sequence and buffer
    _seq = 0;
    _bufferAt = _buffer;
  }
}

bool RDM6300::_checkAndConvert() {
  uint32_t result = 0;

  // reset buffer position 
  _bufferAt = _buffer;

  // Accumulate the checksum, starting with the first value
  uint8_t checksum = _rfidGetNext();

  // We are looking for 4 more values
  int i = 4;
  while (i--) {
    // Grab the next value
    uint8_t value = _rfidGetNext();

    // Add it into the result
    result <<= 8;
    result |= value;

    // Xor it into the checksum
    checksum ^= value;
  }

  // Pull out the checksum from the data
  uint8_t data_checksum = _rfidGetNext();

  if (checksum == data_checksum) {
    rfidTag = result;
    return true;
  }

  return false;
}

// Convert the next two chars in the stream into a byte and
// return that
uint8_t RDM6300::_rfidGetNext() {
  // sscanf needs a 2-byte space to put the result but we
  // only need one byte.
  uint16_t result;

  // Working space to assemble each byte
  static char byte_chars[3];

  // Pull out one byte from this position in the stream
  snprintf(byte_chars, 3, "%c%c", _bufferAt[0], _bufferAt[1]);
  sscanf(byte_chars, "%x", &result);
  _bufferAt += 2;

  return static_cast<uint8_t>(result);
}
