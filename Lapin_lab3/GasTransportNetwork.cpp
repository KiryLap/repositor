#include "GasTransportNetwork.h"
#include "PipeManager.h"
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <queue>

using namespace std;

extern map<int, Connection> connections;
int nextConnectionId = 1;

string GasTransportNetwork::saveConnections() const {
    ostringstream savedConnections; 
    for (const auto& pair : connections) {
        const Connection& connection = pair.second;
        savedConnections <<"connection "<< pair.first << ": " << connection.entryStationId << ";" 
                         << connection.exitStationId << ";" 
                         << connection.pipeId << ";" 
                         << connection.diameter << ";" 
                         << connection.length << "\n"; 
    }
    return savedConnections.str(); 
}

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
    int entryId = readPositive<int>("Введите ID КС входа: ", "Ошибка! ID КС должно быть неотрицательным числом. Попробуйте снова.");
    int exitId = readPositive<int>("Введите ID КС выхода: ", "Ошибка! ID КС должно быть неотрицательным числом. Попробуйте снова.");
    
    if (entryId == exitId) {
        cout << "Ошибка! ID КС входа не может совпадать с ID КС выхода." << endl;
        return;
    }
    
    if (stations.find(entryId) == stations.end() || stations.find(exitId) == stations.end()) {
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
    vector<pair<int, Pipe>> foundPipes;

    for (const auto& pair : pipes) {
        if (pair.second.getDiameter() == diameter && !pair.second.underRepair) {
            foundPipes.push_back(pair);
        }
    }
    
    int pipeId = -1;
    int length = 0;

    if (foundPipes.empty()) {
        cout << "---------------------------------------------------" << endl;
        cout << "Не удалось найти подходящую трубу. Добавляем новую." << endl;
        auto result = pipeManager.addPipeGTN();
        pipeId = get<0>(result);
        diameter = get<1>(result); 
        length = get<2>(result);
        addConnection(entryId, exitId, pipeId, diameter, length);
        viewConnections();
        return;
    }

    for (const auto& foundPipe : foundPipes) {
    pipeId = foundPipe.first;

        bool notUsed = true; 
        for (const auto& connection : connections) {
            if (connection.second.pipeId == pipeId) {
                notUsed = false; 
                break;
            }
        }

        if (notUsed) {
            diameter = foundPipe.second.diameter;
            length = foundPipe.second.length;
            addConnection(entryId, exitId, pipeId, diameter, length);
            viewConnections();
            return;
            break; 
        }
        else {
            cout << "---------------------------------------------------" << endl;
            cout << "Не удалось найти подходящую трубу. Добавляем новую." << endl;
            auto result = pipeManager.addPipeGTN();
            pipeId = get<0>(result);
            diameter = get<1>(result); 
            length = get<2>(result);
            addConnection(entryId, exitId, pipeId, diameter, length);
            viewConnections();
            return;
        }
    }
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
                    << " с диаметром: " << connection.diameter << " и длинной: " << connection.length <<endl;
        }
    }
}

void GasTransportNetwork::addConnection(int entryId, int exitId, int pipeId, int diameter, int length) {
    connections.emplace(nextConnectionId++, Connection(entryId, exitId, pipeId, diameter, length));
    cout << "-----------------------------" << endl;
    cout << "Соединение компрессорной станции ID: " << entryId 
        << " с компрессорной станцией ID: " << exitId 
        << " через трубу с ID: " << pipeId << " с диаметром: " << diameter << " и длинной: " << length << endl;
}

void GasTransportNetwork::removeConnection() {
    int connectionId = readPositive<int>("Введите ID соединения для удаления: ", 
                                          "Ошибка! ID должно быть неотрицательным числом. Попробуйте снова.");

    auto it = connections.find(connectionId);
    if (it != connections.end()) {
        connections.erase(it);
        cout << "Соединение с ID: " << connectionId << " успешно удалено." << endl;
    } else {
        cout << "Ошибка! Соединение с ID: " << connectionId << " не найдено." << endl;
    }
}

void GasTransportNetwork::dfs(int node, map<int, vector<int>>& graph, set<int>& visited, set<int>& recursionStack, stack<int>& Stack, bool& hasCycle) {
    visited.insert(node);
    recursionStack.insert(node);

    for (int neighbor : graph[node]) {
        if (recursionStack.find(neighbor) != recursionStack.end()) {
            hasCycle = true; 
            return;
        }
        if (visited.find(neighbor) == visited.end()) {
            dfs(neighbor, graph, visited, recursionStack, Stack, hasCycle);
        }
    }

    recursionStack.erase(node);
    Stack.push(node);
}

void GasTransportNetwork::findConnectedComponents(map<int, Connection>& connections, vector<vector<int>>& components) {
    map<int, vector<int>> graph;
    set<int> visited;
    set<int> recursionStack; 

    for (const auto& pair : connections) {
        int from = pair.second.entryStationId;
        int to = pair.second.exitStationId;
        graph[from].push_back(to);
        graph[to]; 
    }

    for (const auto& pair : graph) {
        int node = pair.first;
        if (visited.find(node) == visited.end()) {
            vector<int> component;
            stack<int> Stack;
            bool hasCycle = false; 
            dfs(node, graph, visited, recursionStack, Stack, hasCycle);
            if (hasCycle) {
                cout << "Граф с источником/корнем " << node << " содержит циклы, топологическая сортировка невозможна." << endl;
                cout << "---------------------------------------------------------------------------------------------" << endl;
            } else {
                while (!Stack.empty()) {
                    component.push_back(Stack.top());
                    Stack.pop();
                }
                components.push_back(component);
            }
        }
    }
}

void GasTransportNetwork::topologicalSort(const map<int, Connection>& connections) {
    vector<vector<int>> components;
    findConnectedComponents(const_cast<map<int, Connection>&>(connections), components);   

    for (const auto& component : components) {
        map<int, vector<int>> graph;
        map<int, map<int, int>> weights; 
        map<int, int> inDegree; 

        for (const auto& compNode : component) {
            for (const auto& pair : connections) {
                if (pair.second.entryStationId == compNode) {
                    int to = pair.second.exitStationId;
                    int length = pair.second.length;
                    graph[compNode].push_back(to);
                    weights[compNode][to] = length;
                    inDegree[to]++;
                    if (inDegree.find(compNode) == inDegree.end()) {
                        inDegree[compNode] = 0; 
                    }
                }
            }
        }

        stack<int> Stack;
        set<int> visited;
        set<int> recursionStack;

        bool hasCycle = false; 
        for (int node : component) {
            if (visited.find(node) == visited.end()) {
                dfs(node, graph, visited, recursionStack, Stack, hasCycle);
            }
        }

        if (!hasCycle) {
            vector<int> topologicalOrder;
            while (!Stack.empty()) {
                topologicalOrder.push_back(Stack.top());
                Stack.pop();
            }

            cout << "Топологическая сортировка графа: ";
            cout << "---------------------------------------------------" << endl;
            for (int i : topologicalOrder) {
                cout << i << " ";
            }
            cout << endl;

            cout << "Матрица смежности (с весами): " << endl;
            cout << "    ";
            for (int j : topologicalOrder) {
                cout << setw(3) << j << "|"; 
            }
            cout << endl;

            for (int i : topologicalOrder) {
                cout << setw(3) << i << "|"; 
                for (int j : topologicalOrder) {
                    if (weights[i].find(j) != weights[i].end()) {
                        cout << setw(3) << weights[i][j] << " "; 
                    } else {
                        cout << setw(3) << 0 << " "; 
                    }
                }
                cout << endl;
            }
        }
    }
}