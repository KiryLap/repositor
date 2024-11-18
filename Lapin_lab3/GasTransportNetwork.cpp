#include "GasTransportNetwork.h"
#include "PipeManager.h"
#include <iostream>

using namespace std;

int GasTransportNetwork::inputDiameter() {

    while (true) {
        int diameter = readPositive<int>("Введите диаметр трубы (500, 700, 1000, 1400): ", "Ошибка! Диаметр должен быть неотрицательным числом. Попробуйте снова.");
        if (diameter == 500 || diameter == 700 || diameter == 1000 || diameter == 1400) {
            return diameter;
        }
        cout << "Ошибка! Диаметр должен быть равен 500, 700, 1000 или 1400." << endl;
    }
}

void GasTransportNetwork::connectStations(map<int, Pipe>& pipes, map<int, CompressorStation>& stations, PipeManager& pipeManager) {
    int entryId = readPositive<int>("Введите ID KС входа: ", "Ошибка! ID КС должно быть неотрицательным числом. Попробуйте снова.");
    int exitId = readPositive<int>("Введите ID KС выхода: ", "Ошибка! ID КС должно быть неотрицательным числом. Попробуйте снова.");
    if (entryId == exitId) {
        cout << "Ошибка! ID КС входа не может совпадать с ID КС выхода." << endl;
        return;
    }
    if (stations.find(entryId) == stations.end() || 
        stations.find(exitId) == stations.end()) {
        cout << "Одна или две из компрессорных станций не найдена." << endl;
        return;
    }

    for (const auto& connection : connections) {
        if (connection.second.entryStationId == entryId || connection.second.exitStationId == exitId) {
            cout << "Ошибка! Компрессорная станция ID: " << entryId 
                      << " или ID: " << exitId << " уже используется в другом соединении." << endl;
            return;
        }
    }

    int diameter = inputDiameter();

    int pipeId = -1;
    for (const auto& pair : pipes) {
        if (pair.second.getDiameter() == diameter && !pair.second.underRepair) {
            pipeId = pair.first; 
            break;
        }
    }
    if (pipeId == -1) {
        cout << "Не удалось найти подходящую трубу. Добавляем новую." << endl;
        pipeId = pipeManager.addPipeGTN();
    }

    for (const auto& connection : connections) {
        if (connection.second.pipeId == pipeId) {
            cout << "Ошибка! Труба с ID: " << pipeId << " уже используется в другом соединении. Давайте создадим новую!" << endl;
            pipeId = pipeManager.addPipeGTN();
        }
    }

    connections.emplace(nextConnectionId++, Connection(entryId, exitId, pipeId, diameter));
    cout << "Соединение компрессорной станции ID: " << entryId 
              << " с компрессорной станцией ID: " << exitId 
              << " через трубу с ID: " << pipeId << " и диаметром: " << diameter << endl;

    viewConnections();
    return;
}

void GasTransportNetwork::viewConnections() const {
    if (connections.empty()) {
        cout << "Соединений нет." << endl;
    }
    else {
        cout << "-----------------------------" << endl;
        cout << "Существующие соединения:" << endl;
        for (const auto& pair : connections) {
            const Connection& connection = pair.second;
            cout << "ID соединения: " << pair.first
                    << ", (вход) Станция ID: " << connection.entryStationId
                    << " ,(выход) соединена со станцией ID: " << connection.exitStationId
                    << " через трубу ID: " << connection.pipeId
                    << " с диаметром: " << connection.diameter << endl;
        }
    }
}