#ifndef MAX22200_REGISTERS_H
#define MAX22200_REGISTERS_H

//
//Command read/write values
//

#define MAX22200_READ 0
#define MAX22200_WRITE 1

//
//Command register bits
//

#define MAX22200_N8BITS 0
#define MAX22200_A_BNK   1
#define MAX22200_RFU     5
#define MAX22200_RW      7

#define MAX22200_COMMAND(n8, addr, rw) (\
    (_BV(MAX22200_N8BITS)&n8) | (addr<<MAX22200_A_BNK) | (_BV(MAX22200_RW)&rw)\
)

//
//Channel config values
//

#define MAX22200_DEFAULT_CONFIG  0x00
#define MAX22200_PARALLEL_CONFIG 0x01
#define MAX22200_FULL_BRIDGE     0x10

//
//Status register bits
//

#define MAX22200_ACTIVE  0
#define MAX22200_UVM     1
#define MAX22200_COMER   2
#define MAX22200_DPM     3
#define MAX22200_HHF     4
#define MAX22200_OLF     5
#define MAX22200_OCP     6
#define MAX22200_OVT     7

#define MAX22200_CM10    8
#define MAX22200_CM32    10
#define MAX22200_CM54    12
#define MAX22200_CM76    14

#define MAX22200_FREQM   16
#define MAX22200_M_UVM   17
#define MAX22200_M_COMER 18
#define MAX22200_M_DPM   19
#define MAX22200_M_HHF   20
#define MAX22200_M_OLF   21
#define MAX22200_M_OCP   22
#define MAX22200_M_OVT   23

#define MAX22200_ONCH    24



#endif //MAX22200_REGISTERS_H