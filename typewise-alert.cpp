#include "typewise-alert.h"
#include <stdio.h>
#include <map>

// Define constants for cooling type limits
const std::map<CoolingType, std::pair<int, int>> coolingLimits = {
    {PASSIVE_COOLING, {0, 35}},
    {HI_ACTIVE_COOLING, {0, 45}},
    {MED_ACTIVE_COOLING, {0, 40}}
};

// Breach Detection Logic
BreachType inferBreach(double value, double lowerLimit, double upperLimit) { 
    if (value < lowerLimit) {
        return TOO_LOW;
    }
    if (value > upperLimit) {
        return TOO_HIGH;
    }
    return NORMAL;
}

BreachType classifyTemperatureBreach(CoolingType coolingType, double temperatureInC) {
    auto limits = coolingLimits.at(coolingType);
    return inferBreach(temperatureInC, limits.first, limits.second);
}

// Alert Logic
void sendAlert(AlertTarget alertTarget, BreachType breachType) {
    switch (alertTarget) {
        case TO_CONTROLLER:
            sendToController(breachType);
            break;
        case TO_EMAIL:
            sendToEmail(breachType);
            break;
    }
}

void checkAndAlert(AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) {
    BreachType breachType = classifyTemperatureBreach(batteryChar.coolingType, temperatureInC);
    sendAlert(alertTarget, breachType);
}

void sendToController(BreachType breachType) {
    const unsigned short header = 0xfeed;
    printf("%x : %x\n", header, breachType);
}

void sendToEmail(BreachType breachType) {
    const char* recipient = "a.b@c.com";
    if (breachType == TOO_LOW) {
        printf("To: %s\n", recipient);
        printf("Hi, the temperature is too low\n");
    } else if (breachType == TOO_HIGH) {
        printf("To: %s\n", recipient);
        printf("Hi, the temperature is too high\n");
    }
}
