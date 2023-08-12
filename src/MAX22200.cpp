
#include "MAX22200.h"
#include "MAX22200_registers.h"

#include <Arduino.h>
#include <SPI.h>

MAX22200::MAX22200(uint8_t en, uint8_t csb, uint8_t cmd) {
    //set pins
    pin_en = en;
    pin_csb = csb;
    pin_cmd = cmd;    
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

inline void MAX22200::begin() {
    begin(MAX22200::Default, MAX22200::Default, MAX22200::Default, MAX22200::Default);
}

inline void MAX22200::begin(
    MAX22200::ChannelMode cm10, MAX22200::ChannelMode cm32,
    MAX22200::ChannelMode cm54, MAX22200::ChannelMode cm76
) {
    //set to outputs
    pinMode(pin_en, OUTPUT);
    pinMode(pin_csb, OUTPUT);
    pinMode(pin_cmd, OUTPUT);

    //resting state
    digitalWrite(pin_csb, HIGH);
    digitalWrite(pin_cmd, LOW);
    enable();

    //TODO: set fault masks??
    status = read32(MAX22200_STATUS);
    status &= 0x00FFFFFF;          //set all channels low
    status = _BV(MAX22200_ACTIVE); //set active
    setChannelModes(cm10, cm32, cm54, cm76);

}

inline void MAX22200::setChannelModes(
    MAX22200::ChannelMode cm10, MAX22200::ChannelMode cm32,
    MAX22200::ChannelMode cm54, MAX22200::ChannelMode cm76
) {

    //clear prev config
    status &= 0xFFFF00FF;

    status |= (uint32_t) cm10 << MAX22200_CM10;
    status |= (uint32_t) cm32 << MAX22200_CM32;
    status |= (uint32_t) cm54 << MAX22200_CM54;
    status |= (uint32_t) cm76 << MAX22200_CM76;

    write32(MAX22200_STATUS, status);
}

//TODO: lol
inline void MAX22200::setChannelMode(uint8_t ch, ChannelMode mode);

inline MAX22200::ChannelMode MAX22200::getChannelMode(uint8_t ch) {
    //round down to nearest even number
    ch >>= 1;
    ch <<= 1;
    return 0b11 & (status >> (MAX22200_CM10 + ch));
}

inline uint8_t MAX22200::getChannels() {
    return (uint8_t) (status >> MAX22200_ONCH);
}

inline void MAX22200::setChannels(uint8_t out) {
    status &= 0x00FFFFFF;
    status |= (uint32_t) out << MAX22200_ONCH;
    write8(MAX22200_STATUS, out);
}

inline bool MAX22200::getChannel(uint8_t ch) {
    return (status & _BV(ch+MAX22200_ONCH)) != 0;
}

inline void MAX22200::setChannel(uint8_t ch, bool on) {
    uint8_t channels = getChannels();
    if(on) {
        channels |= 1<<ch;
    } else {
        channels &= ~(1<<ch);
    }
    setChannels(channels);
}

inline bool MAX22200::toggleChannel(uint8_t ch) {
    setChannels(getChannels() ^ _BV(ch));
}

inline void MAX22200::configChannel(uint8_t ch, MAX22200::ChannelConfig cfg) {
    write32(MAX22200_CFG_CH1+ch, cfg.bits);
}

inline MAX22200::ChannelConfig MAX22200::readChannelConfig(uint8_t ch) {
    return { read32(MAX22200_CFG_CH1+ch) };
}

