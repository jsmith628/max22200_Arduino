#ifndef MAX22200_H
#define MAX22200_H

#include <stdint.h>

#define MAX22200_READ 0
#define MAX22200_WRITE 1

typedef struct {
    bool n8: 1;
    unsigned int addr: 4;
    unsigned int rfu: 2;
    bool rw: 1;
} MAX22200Cmd;

class MAX22200 {

private:

    uint8_t pin_en;
    uint8_t pin_csb;
    uint8_t pin_cmd;
    uint8_t pin_fault;
    
    uint8_t count;

    inline void beginTransaction();
    inline void endTransaction();

    inline void sendCmd(MAX22200Cmd);

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
