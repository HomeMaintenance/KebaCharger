#include <KebaCharger.h>
#include <thread>

int main(int argc, char **argv){
    KebaCharger charger("KebaCharger0", "192.168.178.88");
    KebaCharger::ChargingState chargingstate = charger.getChargingState();
    KebaCharger::CableState cableState = charger.getCableState();
    float power = charger.using_power();
    float powerfactor = charger.power_factor();
    float chargingCurrent = charger.getChargingCurrent();
    float maxChargingCurrent = charger.maxChargingCurrent();
    float maxSupportedCurrent = charger.maxSupportedCurrent();
    uint32_t rfidCard = charger.rfidCard();
    uint32_t chargedEnergy = charger.chargedEnergy();


    bool res = true;
    res &= charger.setChargingCurrent(63);
    res &= charger.setCharging(true);

    while(charger.getChargingCurrent() < 6){
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    res &= charger.setChargingCurrent(charger.getChargingCurrent()-1);
    // res &= charger.setCharging( chargingstate == KebaCharger::ChargingState::ChargingInProgress );
    float chargingCurrent2 = charger.getChargingCurrent();
    assert(res);
    return 0;
}