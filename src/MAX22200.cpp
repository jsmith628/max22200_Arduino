
#include "MAX22200.h"

#include <Arduino.h>
#include <SPI.h>

MAX22200::MAX22200(uint8_t en, uint8_t csb, uint8_t cmd): MAX22200(en, csb, cmd, 1) {}

MAX22200::MAX22200(uint8_t en, uint8_t csb, uint8_t cmd, uint8_t count) {
    
    //set pins
    pin_en = en;
    pin_csb = csb;
    pin_cmd = cmd;

    //set to outputs
    pinMode(pin_en, OUTPUT);
    pinMode(pin_csb, OUTPUT);
    pinMode(pin_cmd, OUTPUT);

    this->count = count;
    
}


inline void MAX22200::beginTransaction() const {
    SPI.beginTransaction(SPISettings(5e6/*5MHz*/, MSBFIRST, SPI_MODE0));
}

inline void MAX22200::endTransaction() const {
    SPI.endTransaction();
}

inline void MAX22200::enable() { digitalWrite(pin_en, true); }
inline void MAX22200::disable() { digitalWrite(pin_en, false); }
inline void MAX22200::setEnable(bool en) { digitalWrite(pin_en, en); }
