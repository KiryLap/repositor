#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "Pipe.h"
#include "CompressorStation.h"
#include "utils.h"
#include "PipeManager.h"
#include "CompressorStationManager.h"
#include "GasTransportNetwork.h"
#include <locale>

using namespace std;

map<int, Pipe> pipes;
map<int, CompressorStation> stations;

void setupLocale() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
}

void saveToFile(const map<int, Pipe>& pipes, const map<int, CompressorStation>& stations, CompressorStationManager& cmanager, PipeManager& pmanager) {
    string filename;
    cout << "Введите имя файла для сохранения компрессорных станций и труб: ";
    cin >> filename;
    cin.ignore();
    ofstream ofs(filename + ".txt");
    log("Начата операция сохранения в файл: " + filename + ".txt");
    
    if (ofs) {
        for (const auto& station : stations) {
            ofs << "KS " << station.first << ": " << cmanager.saveCS(station.second) << endl;
        }

        for (const auto& pipe : pipes) {
            ofs << "pipe " << pipe.first << ": " << pmanager.savePipe(pipe.second) << endl;
        }

        ofs.close();
        cout << "Данные компрессорных станций и труб сохранены в " << filename << ".txt" << endl;
        log("Успешно сохранено в файл: " + filename + ".txt");
    } else {
        cerr << "Ошибка открытия файла для сохранения." << endl;
        log("Ошибка открытия файла для сохранения: " + filename + ".txt");
    }
}

void loadFromFile(map<int, Pipe>& pipes, map<int, CompressorStation>& stations) {
    string filename;
    cout << "Введите имя файла для загрузки компрессорных станций и труб: ";
    cin >> filename;
    cin.ignore();
    log("Попытка загрузки данных из файла: " + filename + ".txt");

    ifstream ifs(filename + ".txt");
    if (ifs) {
        string line;
        while (getline(ifs, line)) {
            try {
                if (line.substr(0, 2) == "KS") {
                    size_t pos = line.find(':');
                    if (pos == string::npos) throw runtime_error("Неверный формат строки для компрессорной станции");

                    int stationKey = stoi(line.substr(3, pos - 3)); 
                    CompressorStation station;
                    station.load(line.substr(pos + 1)); 
                    
                    if (stations.find(stationKey) == stations.end()) {
                        stations[stationKey] = station;
                        log("Загружена компрессорная станция с ID: " + to_string(stationKey));
                    } else {
                        cerr << "Компрессорная станция с ID " << stationKey << " уже существует." << endl;
                        log("Ошибка: Компрессорная станция с ID " + to_string(stationKey) + " уже существует.");
                    }
                } else if (line.substr(0, 4) == "pipe") {

                    size_t pos = line.find(':');
                    if (pos == string::npos) throw runtime_error("Неверный формат строки для трубы");

                    int pipeKey = stoi(line.substr(5, pos - 5)); 
                    Pipe pipe;
                    pipe.load(line.substr(pos + 1)); 

                    if (pipes.find(pipeKey) == pipes.end()) {
                        pipes[pipeKey] = pipe;
                        log("Загружена труба с ID: " + to_string(pipeKey));
                    } else {
                        cerr << "Труба с ID " << pipeKey << " уже существует." << endl;
                        log("Ошибка: Труба с ID " + to_string(pipeKey) + " уже существует.");
                    }
                }
            } catch (const exception& e) {
                cerr << "Ошибка при загрузке: " << e.what() << endl;
            }
        }
        ifs.close();
    } else {
        cerr << "Не удалось открыть файл: " << filename << ".txt" << endl;
    }
}

void runProgram() {
    PipeManager pManager; 
    CompressorStationManager sManager;
    GasTransportNetwork gasNetwork;
    int choice;

    while (true) {
        cout << "-----------------------------" << endl;
        cout << "|            Меню:          |" << endl;
        cout << "| 1. Трубы                  |" << endl;
        cout << "| 2. КС                     |" << endl;
        cout << "| 3. Просмотр всех объектов |" << endl;
        cout << "| 4. Сохранить              |" << endl;
        cout << "| 5. Загрузить              |" << endl;
        cout << "| 6. Соединение КС          |" << endl;
        cout << "| 0. Выход                  |" << endl;
        cout << "-----------------------------" << endl;
        choice = inputIntInRange("Выберете действие: ", 0, 6);
        log("Пользователь выбрал действие: " + to_string(choice));

        cout << "-----------------------------" << endl;

        switch (choice) {
            case 1:
                pManager.PipeSwitch();
                break;
            case 2:
                sManager.CompressorStationSwitch();
                break;
            case 3:
                pManager.displayAllPipes();
                sManager.displayStations();
                gasNetwork.viewConnections();
                log("Отображение всех труб и станций выполнено.");
                break;
            case 4:
                saveToFile(pipes,stations,sManager,pManager);
                break;
            case 5:
                loadFromFile(pipes, stations);
                break;
            case 6:{
                gasNetwork.connectStations(pipes, stations,pManager);}
                break;
            case 0:
                cout << "Выход из программы." << endl;
                log("Выход из программы.");
                exit( 1 );
            default:
                cout << "Неверный выбор, попробуйте снова." << endl;
                log("Некорректный выбор: " + choice);
                break;
        }
    }
}

int main() {
    setupLocale();
    log("Произведён успешный запуск программы!");
    runProgram();
    return 0;
}