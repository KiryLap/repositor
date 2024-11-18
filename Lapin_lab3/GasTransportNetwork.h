#ifndef GAS_TRANSPORT_NETWORK_H
#define GAS_TRANSPORT_NETWORK_H

#include "Pipe.h"
#include "CompressorStation.h"
#include <map>
#include "PipeManager.h"
#include "CompressorStationManager.h"

struct Connection {
    int entryStationId;
    int exitStationId;
    int pipeId;
    int diameter;

    Connection() = default;  // Конструктор по умолчанию
    Connection(int entry, int exit, int pipe, int dia)
        : entryStationId(entry), exitStationId(exit), pipeId(pipe), diameter(dia) {}
};

// Класс для газотранспортной сети
class GasTransportNetwork {
private:
    std::map<int, Connection> connections; // Хранилище соединений, где ключ - ID соединения
    int nextConnectionId = 1; // Счетчик ID соединений

public:
    int inputDiameter();
    void connectStations(std::map<int, Pipe>& pipes, std::map<int, CompressorStation>& stations, PipeManager& pipeManager);
    void viewConnections() const; // Метод для просмотра соединений
};

#endif // GAS_TRANSPORT_NETWORK_H