#ifndef MAX22200_H
#define MAX22200_H

#include <stdint.h>

class MAX22200 {

private:

    uint8_t pin_en;
    uint8_t pin_csb;
    uint8_t pin_cmd;
    uint8_t pin_fault;

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

};

#endif //MAX22200_H
