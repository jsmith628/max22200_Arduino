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

        //
        //Selects whether to use the full range of available current/voltage,
        //or to halve the current/voltage for increased precision in the HIT/HOLD current values.
        //The default is Full-scale.
        //
        void setScale(bool half_scale);
        inline void useFullScale() { setScale(false); }
        inline void useHalfScale() { setScale(true); }
        inline ChannelConfig withScale(bool half_scale) { setScale(half_scale); return *this; }
        inline ChannelConfig withFullScale() { useFullScale(); return *this; }
        inline ChannelConfig withHalfScale() { useHalfScale(); return *this; }

        bool usesHalfScale() const;
        inline bool usesFullScale() const { return !usesHalfScale(); }

        //
        //Choose whether this channel is controlled by writing to a register over SPI
        //or by pulling the TRIG_A (for even channels) or TRIG_B (for odd) pins high
        //on the IC. If the latter, the setChannel() functions from this
        //library will not work, and it's up to the user to manually control
        //the pins themself.
        //The default is SPI control.
        //
        void setControlMode(bool use_trigger_pin);
        inline void useTriggerPin() { setControlMode(true); }
        inline void useSPI() { setControlMode(false); }
        inline ChannelConfig withControlMode(bool use_trigger_pin) { setControlMode(use_trigger_pin); return *this; }
        inline ChannelConfig withTriggerPin() { useTriggerPin(); return *this; }
        inline ChannelConfig withSPI() { useSPI(); return *this; }

        bool usesTriggerPin() const;
        inline bool usesSPI() const { return !usesTriggerPin(); }

        //
        //Chooses whether to use voltage or current drive mode.
        //With voltage drive, the hit/hold levels control the duty cycle.
        //With current drive, the hit/hold levels set the target current to a proportion of the maximum.
        //Current drive is only available when using low-side switching
        //The default is Current Drive.
        //
        void setDriveMode(bool voltage_drive);
        inline void useVoltageDrive() { setDriveMode(true); }
        inline void useCurrentDrive() { setDriveMode(false); }
        inline ChannelConfig withDriveMode(bool voltage_drive) { setDriveMode(voltage_drive); return *this; }
        inline ChannelConfig withVoltageDrive() { useVoltageDrive(); return *this; }
        inline ChannelConfig withCurrentDrive() { useCurrentDrive(); return *this; } 

        bool usesVoltageDrive() const;
        inline bool usesCurrentDrive() const { return !usesVoltageDrive(); }

        //
        //Set the HIT current level. Ranges from 0-255 where 0 is off, and 255 is full power.
        //With voltage drive, this controls the duty cycle. With current drive
        //this controls the current. Note that only the upper 7 bits are used in practice.
        //The default value is 0.
        //
        void setHit(uint8_t level);
        inline ChannelConfig withHitLevel(uint8_t level) { setHit(level); return *this; }

        uint8_t hitLevel() const;

        //
        //Set the HOLD current level. Ranges from 0-255 where 0 is off, and 255 is full power.
        //With voltage drive, this controls the duty cycle. With current drive
        //this controls the current. Note that only the upper 7 bits are used in practice.
        //The default value is 0.
        //
        void setHold(uint8_t level);
        inline ChannelConfig withHold(uint8_t level) { setHold(level); return *this; }

        uint8_t holdLevel() const;

        //
        //Sets the duration of the HIT phase.
        //The default value is 0.
        //
        void setHitTime(uint8_t cycles);
        inline ChannelConfig withHitTime(uint8_t cycles) { setHitTime(cycles); return *this; }

        uint8_t hitTime() const;

        //
        //Chooses whether this channel will use low-side or high-side switching.
        //The default is low-side switching.
        //
        void setPolarity(bool high_side);
        inline void useLowSideSwitching() { setPolarity(true); }
        inline void useHighSideSwitching() { setPolarity(false); }
        inline ChannelConfig withLowSideSwitching() { useLowSideSwitching(); return *this; }
        inline ChannelConfig withHighSideSwitching() { useHighSideSwitching(); return *this; }

        bool usesHighSideSwitching() const;
        inline bool usesLowSideSwitching() const { return !usesHighSideSwitching(); }

        //
        //When SRC is on, the IC will slow down the rate of voltage transitions
        //in order to reduce EMF emissions.
        //SRC is only available at Chopping frequencies less than 50khz and with
        //low-side switching.
        //The default is for SRC to be off.
        //
        void setSlewRateControl(bool src);
        inline void enableSlewRateControl() { setSlewRateControl(true); }
        inline void disableSlewRateControl() { setSlewRateControl(false); }
        inline ChannelConfig withSlewRateControl() { enableSlewRateControl(); return *this; }
        inline ChannelConfig withoutSlewRateControl() { disableSlewRateControl(); return *this; }

        bool slewRateControlEnabled() const;

        //
        //If enabled, the channel will try to detect if the load has
        //been physically disconnected and raise a FAULT if so.
        //Default is disabled
        //
        void setOpenLoadDetectionEnable(bool en);
        inline void enableOpenLoadDection() { setOpenLoadDetectionEnable(true); }
        inline void disableOpenLoadDection() { setOpenLoadDetectionEnable(false); }
        inline ChannelConfig withOpenLoadDection() { enableOpenLoadDection(); return *this; }
        inline ChannelConfig withoutOpenLoadDection() { disableOpenLoadDection(); return *this; }

        bool openLoadDectionEnabled() const;

        //
        //If enabled, the channel will try to detect if the solenoid plunger
        //has failed to move and raise a FAULT if so.
        //Default is disabled
        //
        void setDetectionOfPlungerMovementEnable(bool en);
        inline void enableDetectionOfPlungerMovement() { setDetectionOfPlungerMovementEnable(true); }
        inline void disableDetectionOfPlungerMovement() { setDetectionOfPlungerMovementEnable(false); }
        inline ChannelConfig withDetectionOfPlungerMovement() { enableDetectionOfPlungerMovement(); return *this; }
        inline ChannelConfig withoutDetectionOfPlungerMovement() { disableDetectionOfPlungerMovement(); return *this; }

        bool detectionOfPlungerMovementEnabled() const;

        //
        //If enabled, the channel will try to detect if the hit current
        //fails to reach its target and raise a FAULT if so.
        //Default is disabled
        //
        void setHitCurrentCheckEnable(bool en);
        inline void enableHitCurrentCheck() { setHitCurrentCheckEnable(true); }
        inline void disableHitCurrentCheck() { setHitCurrentCheckEnable(false); }
        inline ChannelConfig withHitCurrentCheck() { enableHitCurrentCheck(); return *this; }
        inline ChannelConfig withoutHitCurrentCheck() { disableHitCurrentCheck(); return *this; }

        bool hitCurrentCheckEnabled() const;

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

    void writeChannels(uint8_t out);
    void writeChannel(uint8_t ch, bool on);
    bool toggleChannel(uint8_t ch);
    uint8_t getChannels();
    bool getChannel(uint8_t ch);

    void configChannel(uint8_t ch, ChannelConfig cfg);
    ChannelConfig readChannelConfig(uint8_t ch);

};

#endif //MAX22200_H
