#ifndef COMPRESSOR_STATION_MANAGER_H
#define COMPRESSOR_STATION_MANAGER_H

#include <string>
#include <iostream>
#include <map>
#include "CompressorStation.h" 
#include "utils.h"            

class CompressorStationManager {
private:
    int nextId = 1;

public:
    std::string saveCS(const CompressorStation &station) const;
    void toggleShop(CompressorStation &station, int id);
    void addStation();
    void displayStations() const;
    void removeStation();
    void editStation();
    void searchStationName();
    void searchStationUnused();
    void searchStationMenu();
    void searchStationSwitch();
    void displayCompressorStationMenu();
    void CompressorStationSwitch();
};

#endif 