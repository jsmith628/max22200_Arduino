
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

    digitalWrite(pin_csb, HIGH);
    digitalWrite(pin_cmd, LOW);

    this->count = count;
    
}


inline void MAX22200::beginTransaction() {
    SPI.beginTransaction(SPISettings(5e6/*5MHz*/, MSBFIRST, SPI_MODE0));
    digitalWrite(pin_csb, LOW);
}

inline void MAX22200::endTransaction() {
    digitalWrite(pin_csb, HIGH);
    SPI.endTransaction();
}

inline void MAX22200::enable() { digitalWrite(pin_en, true); }
inline void MAX22200::disable() { digitalWrite(pin_en, false); }
inline void MAX22200::setEnable(bool en) { digitalWrite(pin_en, en); }

inline void MAX22200::sendCmd(uint8_t cmd) {

    digitalWrite(pin_cmd, HIGH);
    beginTransaction();

    for(int i=0; i<count; i++) {
        SPI.transfer(cmd);
    }

    endTransaction();
    digitalWrite(pin_cmd, LOW);

}

inline void MAX22200::read(uint8_t addr, uint8_t* data) {

    uint8_t cmd = MAX22200_COMMAND(true, addr, MAX22200_READ);
    sendCmd(cmd);

    beginTransaction();
    for(int i=count-1; i>=0; i--) {
        data[i] = SPI.transfer(0);
    }
    endTransaction();

}

inline void MAX22200::read(uint8_t addr, uint32_t* data) {

    uint8_t cmd = MAX22200_COMMAND(false, addr, MAX22200_READ);
    sendCmd(cmd);

    beginTransaction();
    for(int i=count-1; i>=0; i--) {
        data[i] = 0;
        data[i] |= ((uint32_t) SPI.transfer16(0)) << 16;
        data[i] |= ((uint32_t) SPI.transfer16(0)) & 0xFFFF;
    }
    endTransaction();

}

inline void MAX22200::write(uint8_t addr, const uint8_t* data) {
    uint8_t cmd = MAX22200_COMMAND(true, addr, MAX22200_WRITE);
    sendCmd(cmd);

    beginTransaction();
    for(int i=count-1; i>=0; i--) {
        SPI.transfer(data[i]);
    }
    endTransaction();
}

inline void MAX22200::write(uint8_t addr, const uint32_t* data) {
    uint8_t cmd = MAX22200_COMMAND(false, addr, MAX22200_WRITE);
    sendCmd(cmd);

    beginTransaction();
    for(int i=count-1; i>=0; i--) {
        SPI.transfer16((uint16_t) (data[i] >> 16));
        SPI.transfer16((uint16_t) (data[i] & 0xFFFF));
    }
    endTransaction();
}
