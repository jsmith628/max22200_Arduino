#ifndef MAX22200_H
#define MAX22200_H

#include "MAX22200_registers.h"

#include <stdint.h>

class MAX22200 {

private:

    uint8_t pin_en;
    uint8_t pin_csb;
    uint8_t pin_cmd;
    uint8_t pin_fault;
    
    uint8_t count;

    inline void beginTransaction();
    inline void endTransaction();

    inline void sendCmd(uint8_t);

public:

    MAX22200(uint8_t en, uint8_t csb, uint8_t cmd);
    MAX22200(uint8_t en, uint8_t csb, uint8_t cmd, uint8_t count);

    inline void enable();
    inline void disable();
    inline void setEnable(bool);

    inline void read(uint8_t addr, uint8_t* data);
    inline void read(uint8_t addr, uint32_t* data);

    inline void write(uint8_t addr, const uint8_t* data);
    inline void write(uint8_t addr, const uint32_t* data);

};

#endif //MAX22200_H
