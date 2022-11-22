#pragma once
#include <ModbusDevice.h>
#include <ModbusRegister.h>
#include <PowerSink.h>

class KebaCharger: public mb::Device, public PowerSink {
public:
    explicit KebaCharger(const std::string& name, const std::string& ipAddress, int port = 502);
    KebaCharger(const Device &other) = delete;
    virtual ~KebaCharger();

    virtual float using_power() const;
    virtual bool allow_power(float power);

    float power_factor() const;

    enum ChargingState {
        ChargingStartUp = 0,
        ChargingAuthLocked = 1,
        ChargingAwaitConnection = 2,
        ChargingInProgress = 3,
        ChargingError = 4,
        CharingInterrupt = 5
    };
    ChargingState getChargingState() const;

    enum CableState{
        CableNoCable = 0,
        CableOnlyStation = 1,
        CableOnlyStationLockec = 3,
        CableConnected = 5,
        CableConnectedLocked = 7
    };
    CableState getCableState() const;

    std::array<float, 3> getChargingCurrentPerPhase() const;
    float getChargingCurrent() const;

    float maxChargingCurrent() const;
    float maxSupportedCurrent() const;
    uint32_t rfidCard() const;
    uint32_t chargedEnergy() const;

    /**
     * @brief Set the charging current
     * @param current Unit: Ampere, min: 6, max: 63
     */
    bool setChargingCurrent(float current);
    /**
     * @brief Stops charging after this energy has been charged
     * @param energy Unit: Wh (In 10 Wh steps)
     */
    bool setEnergy(float energy);
    bool setLockPlug(bool lock);
    bool setCharging(bool state);

protected:

private:
    mb::Register<unsigned int> mbReg_chargingState;
    mb::Register<unsigned int> mbReg_cableState;
    mb::Register<unsigned int> mbReg_current_L1;
    mb::Register<unsigned int> mbReg_current_L2;
    mb::Register<unsigned int> mbReg_current_L3;
    mb::Register<unsigned int> mbReg_power;
    mb::Register<unsigned int> mbReg_powerfactor;
    mb::Register<unsigned int> mbReg_maxChargingCurrent;
    mb::Register<unsigned int> mbReg_maxSupportedCurrent;
    mb::Register<unsigned int> mbReg_rfidCard;
    mb::Register<unsigned int> mbReg_chargedEnergy;
    mb::Register<unsigned short> mbReg_setChargingCurrent;
    mb::Register<unsigned short> mbReg_setEnergy;
    mb::Register<unsigned short> mbReg_setLockPlug;
    mb::Register<unsigned short> mbReg_setCharging;

};
