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
//Register addresses
//

#define MAX22200_STATUS  0x00
#define MAX22200_CFG_CH1 0x01
#define MAX22200_CFG_CH2 0x02
#define MAX22200_CFG_CH3 0x03
#define MAX22200_CFG_CH4 0x04
#define MAX22200_CFG_CH5 0x05
#define MAX22200_CFG_CH6 0x06
#define MAX22200_CFG_CH7 0x07
#define MAX22200_CFG_CH8 0x08
#define MAX22200_FAULT   0x09
#define MAX22200_CFG_DPM 0x0A

//
//Channel config values
//

#define MAX22200_DEFAULT_CONFIG  0x00 //Each channel is separately controlled
#define MAX22200_PARALLEL_CONFIG 0x01 //The two channels act together for doubling current capacity
#define MAX22200_FULL_BRIDGE     0x10 //For driving loads (like motors), that can be reversed

//
//Status register bits
//

//0 for low-power more/off, 1 for normal operation
#define MAX22200_ACTIVE  0

//The statuses for each fault condition. 1 for error, 0 for normal operation
#define MAX22200_UVM     1 //Undervoltage Fault
#define MAX22200_COMER   2 //Communication Error Fault
#define MAX22200_DPM     3 //Detection of Plunger Movement Fault
#define MAX22200_HHF     4 //HIT Current Not Reached Fault
#define MAX22200_OLF     5 //Open-Load Fault
#define MAX22200_OCP     6 //Overcurrent Protection Fault
#define MAX22200_OVT     7 //Overtemperature Fault

//operation mode for each pin pair
#define MAX22200_CM10    8
#define MAX22200_CM32    10
#define MAX22200_CM54    12
#define MAX22200_CM76    14

#define MAX22200_FREQM   16 //1 for 100kHz, 0 for 80kHz internal oscillator

//0 if that fault should pull FAULT low, 1 if it shouldn't
#define MAX22200_M_UVM   17
#define MAX22200_M_COMER 18
#define MAX22200_M_DPM   19
#define MAX22200_M_HHF   20
#define MAX22200_M_OLF   21
#define MAX22200_M_OCP   22
#define MAX22200_M_OVT   23

//8-bits for turning each channel on/off
#define MAX22200_ONCH    24

//
//CFG_CH register bits
//

#define MAX22200_HHF_EN    0 //1 to enable HIT current reached detection
#define MAX22200_DPM_EN    1 //1 to enable detection of plunger movement
#define MAX22200_OL_EN     2 //1 to enable open-load diagnostic
#define MAX22200_SRC       3 //0 for fast OUT, 1 for slew-rate controlled in low-side mode
#define MAX22200_FREQ_CFG  4 //Sets f_chop. 0 for f_main/4, 1 for f_main/3, 2 for f_main/2, 3 for f_main 
#define MAX22200_HSnLS     6 //0 for low-side, 1 for high-side
#define MAX22200_VDRnCDR   7 //0 for Current-drive, 1 for Voltage-drive
#define MAX22200_HIT_T     8 //Hit time. T_hit = HIT_T*40/f_chop
#define MAX22200_HIT      16 //Hit current/voltage setting
#define MAX22200_TRIGnSPI 23 //0 if channel controlled by ONCH register, 1 if controlled by TRIGA / TRIGB pins
#define MAX22200_HOLD     24 //Hold current/voltage setting
#define MAX22200_HFS      31 //Half Full-scale Setting: 0x1 for half-current but double precision, 0x0 for normal

//
//FAULT register bits
//

//per-channel fault detection
#define MAX22200_FAULT_DPM  0
#define MAX22200_FAULT_OLF  8
#define MAX22200_FAULT_HHF 16
#define MAX22200_FAULT_OCP 24

//
//CFG_DPM register bits
//

#define MAX22200_DPM_IPTH    0
#define MAX22200_DPM_TDEB    4 //
#define MAX22200_DPM_ISTART  8 //DPM starting current

#endif //MAX22200_REGISTERS_H