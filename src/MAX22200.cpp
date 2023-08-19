
#include "MAX22200.h"
#include "MAX22200_registers.h"

#include <Arduino.h>
#include <SPI.h>

#define SET_BITS(x, mask, val) \
if(val) {\
    x |= mask;\
} else {\
    x &= ~mask;\
}

void MAX22200::ChannelConfig::setScale(bool half) {
    SET_BITS(bits, _BV(MAX22200_HFS), half);
}
bool MAX22200::ChannelConfig::usesHalfScale() const {
    return (bits & _BV(MAX22200_HFS)) != 0;
}

void MAX22200::ChannelConfig::setControlMode(bool trig) {
    SET_BITS(bits, _BV(MAX22200_TRIGnSPI), trig);
}
bool MAX22200::ChannelConfig::usesTriggerPin() const {
    return (bits & _BV(MAX22200_TRIGnSPI)) != 0;
}

void MAX22200::ChannelConfig::setDriveMode(bool vdr) {
    SET_BITS(bits, _BV(MAX22200_VDRnCDR), vdr);
}
bool MAX22200::ChannelConfig::usesVoltageDrive() const {
    return (bits & _BV(MAX22200_VDRnCDR)) != 0;
}

void MAX22200::ChannelConfig::setHit(uint8_t level) {
    level >>= 1; //reduce to 7-bits
    bits &= ~((uint32_t) 0x7Fu << MAX22200_HIT); //reset the hold current
    bits |= (uint32_t) level << MAX22200_HIT; //set the bits
}

uint8_t MAX22200::ChannelConfig::hitLevel() const {
    return ((bits >> MAX22200_HIT) & 0x7Fu) << 1;
}

void MAX22200::ChannelConfig::setHold(uint8_t level) {
    level >>= 1; //reduce to 7-bits
    bits &= ~((uint32_t) 0x7Fu << MAX22200_HOLD); //reset the hold current
    bits |= (uint32_t) level << MAX22200_HOLD;  //set the bits
}

uint8_t MAX22200::ChannelConfig::holdLevel() const {
    return ((bits >> MAX22200_HOLD) & 0x7) << 1;
}

void MAX22200::ChannelConfig::setHitTime(uint8_t cycles) {
    bits &= ~((uint32_t) 0x7Fu << MAX22200_HIT_T); //reset the hold current
    bits |= (uint32_t) cycles << MAX22200_HIT_T;  //set the bits
}

uint8_t MAX22200::ChannelConfig::hitTime() const {
    return ((bits >> MAX22200_HIT_T) & 0xFF) << 1;
}

void MAX22200::ChannelConfig::setPolarity(bool high_side) {
    SET_BITS(bits, _BV(MAX22200_HSnLS), high_side);
}
bool MAX22200::ChannelConfig::usesHighSideSwitching() const {
    return (bits & _BV(MAX22200_HSnLS)) != 0;
}

void MAX22200::ChannelConfig::setSlewRateControl(bool slc) {
    SET_BITS(bits, _BV(MAX22200_SRC), slc);
}
bool MAX22200::ChannelConfig::slewRateControlEnabled() const {
    return (bits & _BV(MAX22200_SRC)) != 0;
}

void MAX22200::ChannelConfig::setOpenLoadDetectionEnable(bool en) {
    SET_BITS(bits, _BV(MAX22200_OL_EN), en);
}
bool MAX22200::ChannelConfig::openLoadDectionEnabled() const {
    return (bits & _BV(MAX22200_OL_EN)) != 0;
}

void MAX22200::ChannelConfig::setDetectionOfPlungerMovementEnable(bool en) {
    SET_BITS(bits, _BV(MAX22200_DPM_EN), en);
}
bool MAX22200::ChannelConfig::detectionOfPlungerMovementEnabled() const {
    return (bits & _BV(MAX22200_DPM_EN)) != 0;
}

void MAX22200::ChannelConfig::setHitCurrentCheckEnable(bool en) {
    SET_BITS(bits, _BV(MAX22200_HHF_EN), en);
}
bool MAX22200::ChannelConfig::hitCurrentCheckEnabled() const {
    return (bits & _BV(MAX22200_HHF_EN)) != 0;
}

MAX22200::MAX22200(uint8_t en, uint8_t csb, uint8_t cmd) {
    //set pins
    pin_en = en;
    pin_csb = csb;
    pin_cmd = cmd;    
}

void MAX22200::beginTransaction() {
    SPI.beginTransaction(SPISettings(5e6/*5MHz*/, MSBFIRST, SPI_MODE0));
}

void MAX22200::endTransaction() {
    SPI.endTransaction();
}

void MAX22200::sendCmd(uint8_t cmd) {
    digitalWrite(pin_cmd, HIGH);
    digitalWrite(pin_csb, LOW);
    SPI.transfer(cmd);
    digitalWrite(pin_csb, HIGH); //TODO: check if this is necessary
    digitalWrite(pin_cmd, LOW);
}

void MAX22200::setEnable(bool en) { digitalWrite(pin_en, en); }
void MAX22200::enable() { setEnable(true); }
void MAX22200::disable() { setEnable(false); }

uint8_t MAX22200::transfer8(uint8_t addr, uint8_t out) {
    beginTransaction();

    sendCmd(MAX22200_COMMAND(true, addr, MAX22200_READ));    

    digitalWrite(pin_csb, LOW);
    uint8_t in = SPI.transfer(0);
    digitalWrite(pin_csb, HIGH);
    
    endTransaction();

    return in;
}

uint32_t MAX22200::transfer32(uint8_t addr, uint32_t out) {
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

uint8_t MAX22200::read8(uint8_t addr) {
    return transfer8(addr, 0);
}

uint32_t MAX22200::read32(uint8_t addr) {
    return transfer32(addr, 0);
}

void MAX22200::write8(uint8_t addr, uint8_t data) {
    transfer8(addr, data);
}

void MAX22200::write32(uint8_t addr, uint32_t data) {
    transfer32(addr, data);
}

void MAX22200::begin() {
    begin(MAX22200::Default, MAX22200::Default, MAX22200::Default, MAX22200::Default);
}

void MAX22200::begin(
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

    status = read32(MAX22200_STATUS);
    status &= 0x00FFFFFF;          //set all channels low

    //TODO: set fault masks instead of overwriting??
    status = _BV(MAX22200_ACTIVE); //set active
    setChannelModes(cm10, cm32, cm54, cm76);

}

void MAX22200::setChannelModes(
    MAX22200::ChannelMode cm10, MAX22200::ChannelMode cm32,
    MAX22200::ChannelMode cm54, MAX22200::ChannelMode cm76
) {

    //clear prev config
    const uint32_t mask = 0xFFu << MAX22200_CM10;
    status &= ~mask;

    status |= (uint32_t) cm10 << MAX22200_CM10;
    status |= (uint32_t) cm32 << MAX22200_CM32;
    status |= (uint32_t) cm54 << MAX22200_CM54;
    status |= (uint32_t) cm76 << MAX22200_CM76;

    write32(MAX22200_STATUS, status);
}

//TODO: lol
void MAX22200::setChannelMode(uint8_t ch, ChannelMode mode) {
    //round down to the nearest even number
    ch &= ~1u;

    //delete the prev mode
    uint32_t mask = 0b11u << (ch+MAX22200_CM10);
    status &= ~mask;

    //update the mode
    status |= (uint32_t) mode << (ch+MAX22200_CM10);
    write32(MAX22200_STATUS, status);
}

MAX22200::ChannelMode MAX22200::getChannelMode(uint8_t ch) {
    //round down to nearest even number
    ch &= ~1u;
    return 0b11 & (status >> (MAX22200_CM10 + ch));
}

uint8_t MAX22200::getChannels() {
    return (uint8_t) (status >> MAX22200_ONCH);
}

void MAX22200::setChannels(uint8_t out) {
    status &= 0x00FFFFFFu;
    status |= (uint32_t) out << MAX22200_ONCH;
    write8(MAX22200_STATUS, out);
}

bool MAX22200::getChannel(uint8_t ch) {
    return (status & _BV(ch+MAX22200_ONCH)) != 0;
}

void MAX22200::setChannel(uint8_t ch, bool on) {
    uint8_t channels = getChannels();
    if(on) {
        channels |= 1<<ch;
    } else {
        channels &= ~(1<<ch);
    }
    setChannels(channels);
}

bool MAX22200::toggleChannel(uint8_t ch) {
    setChannels(getChannels() ^ _BV(ch));
}

void MAX22200::configChannel(uint8_t ch, MAX22200::ChannelConfig cfg) {
    write32(MAX22200_CFG_CH1+ch, cfg.bits);
}

MAX22200::ChannelConfig MAX22200::readChannelConfig(uint8_t ch) {
    return { read32(MAX22200_CFG_CH1+ch) };
}

