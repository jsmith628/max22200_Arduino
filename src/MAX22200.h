#ifndef MAX22200_H
#define MAX22200_H

#include <stdint.h>

class MAX22200 {

public:

    enum ChannelMode {
        Default = 0, Parallel = 0b01, FullBridge = 0b10
    };

    struct ChannelConfig {
        uint32_t bits = 0;
    };


private:

    uint8_t pin_en;
    uint8_t pin_csb;
    uint8_t pin_cmd;
    uint8_t pin_fault;

    uint32_t status;

    void beginTransaction();
    void endTransaction();

    void sendCmd(uint8_t);

    uint8_t transfer8(uint8_t addr, uint8_t data);
    uint32_t transfer32(uint8_t addr, uint32_t data);

public:

    MAX22200(uint8_t en, uint8_t csb, uint8_t cmd);

    void enable();
    void disable();
    void setEnable(bool);

    uint8_t  read8(uint8_t addr);
    uint32_t read32(uint8_t addr);

    void write8(uint8_t addr, uint8_t data);
    void write32(uint8_t addr, uint32_t data);

    void begin();
    void begin(ChannelMode ch10, ChannelMode ch32, ChannelMode ch54, ChannelMode ch76);

    void setChannelModes(ChannelMode cm10, ChannelMode cm32, ChannelMode cm54, ChannelMode cm76);
    void setChannelMode(uint8_t ch, ChannelMode mode);
    ChannelMode getChannelMode(uint8_t ch);

    void setChannels(uint8_t out);
    void setChannel(uint8_t ch, bool on);
    bool toggleChannel(uint8_t ch);
    uint8_t getChannels();
    bool getChannel(uint8_t ch);

    void configChannel(uint8_t ch, ChannelConfig cfg);
    ChannelConfig readChannelConfig(uint8_t ch);

};

#endif //MAX22200_H
