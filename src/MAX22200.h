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

    inline void beginTransaction();
    inline void endTransaction();

    inline void sendCmd(uint8_t);

    inline uint8_t transfer8(uint8_t addr, uint8_t data);
    inline uint32_t transfer32(uint8_t addr, uint32_t data);

public:

    MAX22200(uint8_t en, uint8_t csb, uint8_t cmd);

    inline void enable();
    inline void disable();
    inline void setEnable(bool);

    inline uint8_t  read8(uint8_t addr);
    inline uint32_t read32(uint8_t addr);

    inline void write8(uint8_t addr, uint8_t data);
    inline void write32(uint8_t addr, uint32_t data);

    inline void begin();
    inline void begin(ChannelMode ch10, ChannelMode ch32, ChannelMode ch54, ChannelMode ch76);

    inline void setChannelModes(ChannelMode cm10, ChannelMode cm32, ChannelMode cm54, ChannelMode cm76);
    inline void setChannelMode(uint8_t ch, ChannelMode mode);
    inline ChannelMode getChannelMode(uint8_t ch);

    inline void setChannels(uint8_t out);
    inline void setChannel(uint8_t ch, bool on);
    inline bool toggleChannel(uint8_t ch);
    inline uint8_t getChannels();
    inline bool getChannel(uint8_t ch);

    inline void configChannel(uint8_t ch, ChannelConfig cfg);
    inline ChannelConfig readChannelConfig(uint8_t ch);

};

#endif //MAX22200_H
