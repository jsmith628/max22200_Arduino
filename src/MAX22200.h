#ifndef MAX22200_H
#define MAX22200_H

#include <stdint.h>

class MAX22200 {

private:

    uint8_t pin_en;
    uint8_t pin_csb;
    uint8_t pin_cmd;
    uint8_t pin_fault;
    
    uint8_t count;

    inline void beginTransaction() const;
    inline void endTransaction() const;

public:

    MAX22200(uint8_t en, uint8_t csb, uint8_t cmd);
    MAX22200(uint8_t en, uint8_t csb, uint8_t cmd, uint8_t count);

    inline void enable();
    inline void disable();
    inline void setEnable(bool);

};

#endif //MAX22200_H
