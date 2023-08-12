
#include "MAX22200.h"
#include "MAX22200_registers.h"

#include <Arduino.h>
#include <SPI.h>

MAX22200::MAX22200(uint8_t en, uint8_t csb, uint8_t cmd) {
    
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
    
}

inline void MAX22200::beginTransaction() {
    SPI.beginTransaction(SPISettings(5e6/*5MHz*/, MSBFIRST, SPI_MODE0));
}

inline void MAX22200::endTransaction() {
    SPI.endTransaction();
}

inline void MAX22200::sendCmd(uint8_t cmd) {
    digitalWrite(pin_cmd, HIGH);
    digitalWrite(pin_csb, LOW);
    SPI.transfer(cmd);
    digitalWrite(pin_csb, HIGH); //TODO: check if this is necessary
    digitalWrite(pin_cmd, LOW);
}

inline void MAX22200::setEnable(bool en) { digitalWrite(pin_en, en); }
inline void MAX22200::enable() { setEnable(true); }
inline void MAX22200::disable() { setEnable(false); }

inline uint8_t MAX22200::transfer8(uint8_t addr, uint8_t out) {
    beginTransaction();

    sendCmd(MAX22200_COMMAND(true, addr, MAX22200_READ));    

    digitalWrite(pin_csb, LOW);
    uint8_t in = SPI.transfer(0);
    digitalWrite(pin_csb, HIGH);
    
    endTransaction();

    return in;
}

inline uint32_t MAX22200::transfer32(uint8_t addr, uint32_t out) {
    beginTransaction();

    sendCmd(MAX22200_COMMAND(false, addr, MAX22200_READ));    

    digitalWrite(pin_csb, LOW);
    uint32_t in = 0;
    in |= (uint32_t) SPI.transfer((uint8_t) (out>>24)) << 24;
    in |= (uint32_t) SPI.transfer((uint8_t) (out>>16)) << 16;
    in |= (uint32_t) SPI.transfer((uint8_t) (out>>8))  << 8;
    in |= (uint32_t) SPI.transfer((uint8_t) (out));
    digitalWrite(pin_csb, HIGH);
    
    endTransaction();

    return in;
}

inline uint8_t MAX22200::read8(uint8_t addr) {
    return transfer8(addr, 0);
}

inline uint32_t MAX22200::read32(uint8_t addr) {
    return transfer32(addr, 0);
}

inline void MAX22200::write8(uint8_t addr, uint8_t data) {
    transfer8(addr, data);
}

inline void MAX22200::write32(uint8_t addr, uint32_t data) {
    transfer32(addr, data);
}
