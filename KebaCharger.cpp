#include "KebaCharger.h"

KebaCharger::KebaCharger(const std::string& name, const std::string& ipAddress, int port /* = 502 */):
    mb::Device(ipAddress, port),
    mbReg_chargingState(this, 1000, 1),
    mbReg_cableState(this, 1004, 1),
    mbReg_current_L1(this, 1008, 1),
    mbReg_current_L2(this, 1010, 1),
    mbReg_current_L3(this, 1012, 1),
    mbReg_power(this, 1020, 1, " mW"),
    mbReg_total_energy(this, 1036, 1, " Wh"),
    mbReg_voltage_L1(this, 1040, 1, " V"),
    mbReg_voltage_L2(this, 1042, 1, " V"),
    mbReg_voltage_L3(this, 1044, 1, " V"),
    mbReg_powerfactor(this, 1046, 1),
    mbReg_maxChargingCurrent(this, 1100),
    mbReg_maxSupportedCurrent(this, 1110),
    mbReg_rfidCard(this, 1500),
    mbReg_chargedEnergy(this, 1502, 1, " Wh"),
    mbReg_setChargingCurrent(this, 5004),
    mbReg_setEnergy(this, 5010),
    mbReg_setLockPlug(this, 5012),
    mbReg_setCharging(this, 5014),
    PowerSink(name)
{
    std::runtime_error connection_exception("KebaCharger "+ipAddress+":"+std::to_string(port)+ " not reachable");
    if(modbus_set_slave(connection, 255) != 0){
        throw connection_exception;
    }
}

KebaCharger::~KebaCharger(){

}

float KebaCharger::using_power() const {
    float power = mbReg_power.getValue(true) * 0.001;
    return power;
}

bool KebaCharger::allow_power(float power){
    assert(false);
    return true;
}

float KebaCharger::power_factor() const {
    float power_factor = mbReg_powerfactor.getValue(true) * 0.1;
    return power_factor;
}


KebaCharger::ChargingState KebaCharger::getChargingState() const {
    bool res = true;
    int status = 0;
    std::vector<uint16_t> value = mbReg_chargingState.readRawData(true, &res, &status);
    return static_cast<ChargingState>(value.at(1));
}

KebaCharger::CableState KebaCharger::getCableState() const {
    bool res = true;
    int status = 0;
    std::vector<uint16_t> value = mbReg_cableState.readRawData(true, &res, &status);
    return static_cast<CableState>(value.at(1));
}

std::array<float, 3> KebaCharger::getChargingCurrentPerPhase() const {
    bool _res = true;
    std::array<float, 3> res = {0};
    res[0] = mbReg_current_L1.getValue(true,&_res)*0.001;
    res[1] = mbReg_current_L2.getValue(true,&_res)*0.001;
    res[2] = mbReg_current_L3.getValue(true,&_res)*0.001;
    return res;
}

float KebaCharger::getChargingCurrent() const {
    std::array<float, 3> cur = getChargingCurrentPerPhase();
    float res = 0;
    for(uint32_t i = 0; i < cur.size(); ++i){
        res += cur[i];
    }
    return res/3;
}

float KebaCharger::getTotalEnergy() const {
    bool _res = true;
    auto totaL_energy = mbReg_total_energy.getValue(true, &_res);
    return totaL_energy;
}

std::array<float, 3> KebaCharger::getVoltagePerPhase() const {
    bool _res = true;
    std::array<float, 3> res = {0};
    res[0] = mbReg_voltage_L1.getValue(true, &_res);
    res[1] = mbReg_voltage_L2.getValue(true, &_res);
    res[2] = mbReg_voltage_L3.getValue(true, &_res);
    return res;
}

float KebaCharger::getVoltage() const {
    auto voltages = getVoltagePerPhase();
    float res = 0;
    for(uint32_t i = 0; i < voltages.size(); ++i){
        res += voltages[i];
    }
    return res/3;
}

float KebaCharger::maxChargingCurrent() const {
    return mbReg_maxChargingCurrent.getValue(true) * 0.001;
}

float KebaCharger::maxSupportedCurrent() const {
    return mbReg_maxSupportedCurrent.getValue(true) * 0.001;
}

uint32_t KebaCharger::rfidCard() const {
    return mbReg_rfidCard.getValue(true);
}

uint32_t KebaCharger::chargedEnergy() const {
    return mbReg_chargedEnergy.getValue(true);
}

bool KebaCharger::setChargingCurrent(float current) {
    if(current < 6 || current > 63)
        return false;
    return mbReg_setChargingCurrent.setValue(static_cast<unsigned short>(current*1000));
}

bool KebaCharger::setEnergy(float energy) {
    return mbReg_setEnergy.setValue(static_cast<unsigned short>(energy*0.1));
}

bool KebaCharger::setLockPlug(bool lock) {
    return mbReg_setLockPlug.setValue(static_cast<unsigned short>(lock));
}

bool KebaCharger::setCharging(bool state) {
    return mbReg_setCharging.setValue(static_cast<unsigned short>(state));
}
