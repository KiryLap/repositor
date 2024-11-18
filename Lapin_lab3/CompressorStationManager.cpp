#include "CompressorStationManager.h"
#include "CompressorStation.h"
#include <iostream>
#include <string>

extern std::map<int, CompressorStation> stations; 
using namespace std;

    string CompressorStationManager::saveCS(const CompressorStation &stations) const {
        return stations.full_name + ";" + to_string(stations.totalShops) + ";" + 
               to_string(stations.operationalShops) + ";" + 
               to_string(stations.unusedOperationalShops) + ";" + 
               to_string(stations.efficiency);
    }

    void CompressorStationManager::toggleShop(CompressorStation &stations, int id) {
        if (stations.full_name.empty() && stations.totalShops <= 0 && stations.operationalShops <= 0) {
            cout << "КС нет. Запуск/Остановка цеха невозможна." << endl;
        }
        else {
            int choice;
            cout << "-----------------------------" << endl;
            cout << "| 1. Запустить новый цех    |" << endl;
            cout << "| 2. Остановить работу цеха |" << endl;
            cout << "-----------------------------" << endl;
            choice = inputIntInRange("Выберете действие: ", 1, 3);
            cin.ignore();
            log("Пользователь выбрал действие: " + choice);

            cout << "-----------------------------" << endl;

            switch (choice) {
                case 1: 
                    log("Пользователь выбрал запуск цеха");
                    if (stations.operationalShops < stations.totalShops) {
                        stations.operationalShops++;
                        stations.unusedOperationalShops = ((stations.totalShops - stations.operationalShops) / static_cast<double>(stations.totalShops)) * 100;
                        cout << "Запустили новый цех. Теперь в работе: " << stations.operationalShops << endl;
                    } else {
                        cout << "Все цехи уже запущены." << endl;
                    }
                    break;
                case 2: 
                    log("Пользователь выбрал остановку цеха");
                    if (stations.operationalShops > 0) {
                        stations.operationalShops--;
                        stations.unusedOperationalShops = ((stations.totalShops - stations.operationalShops) / static_cast<double>(stations.totalShops)) * 100;
                        cout << "Остановили цех. Теперь в работе: " << stations.operationalShops << endl;
                    } else {
                        cout << "Нет работающих цехов для остановки." << endl;
                        
                    }
                    break;
                default:
                    cout << "Некорректный выбор. Попробуйте снова." << endl;
                    
                    break;
            }
            
        }
    }

    void CompressorStationManager::addStation() {
        CompressorStation station;

        while (stations.find(nextId) != stations.end()) {
            nextId++;
        }

        station.read();
        stations[nextId] = station;

        cout << "Компрессорная станция добавлена." << endl;
        log("Компрессорная станция добавлена в систему. ID: " + to_string(nextId));

        nextId++;
    }

    void CompressorStationManager::displayStations() const {
        if (stations.empty()) {
            cout << "Нет доступных компрессорных станций." << endl;
            return;
        }
        cout << "Список компрессорных станций:" << endl;
        cout << endl;
        for (const auto& station : stations) {
            cout << "ID " <<station.first;
            station.second.display();
        }
        cout << endl;
        log("Пользователь запросил список компрессорных станций.");
    }

    void CompressorStationManager::removeStation() {
        if (stations.empty()) {
            cout << "Список КС пуст. Удаление невозможно." << endl;
            return; 
        }
        cout << "Введите ID КС для удаления: ";
        string command;
        long id;
        getline(cin, command);
        id = numberOrDefault(command);
        if (id == -1){
            cout << "Неверный ввод. Повторите попытку!" << endl;
            return;
        }

        auto it = stations.find(id);
        if (it == stations.end()) {
            cout << "Ошибка! Станции с таким ID не существует." << endl;
            return;
        }

        stations.erase(it);
        cout << "Компрессорная станция была удалена." << endl;
    }

    void CompressorStationManager::editStation() {
        if (stations.empty()) {
            cout << "Список КС пуст. Редактирование невозможно." << endl;
            return; 
        }
        cout << "Введите ID КС для редактирования: ";
        string command;
        long id;
        getline(cin, command);
        id = numberOrDefault(command);
        if (id == -1){
            cout << "Неверный ввод. Повторите попытку!" << endl;
            return;
        }

        auto it = stations.find(id);
        if (it != stations.end()) {
            cout << "Редактирование КС с ID " << id << endl;
            toggleShop(it->second,it->first); 
            log("Пользователь обновил статус цеха на КС с ID: " + to_string(id));
        } else {
            cout << "КС с ID " << id << " не найдена." << endl;
        }
    }

    void CompressorStationManager::searchStationName(){
        string searchName;
        log("Пользователь запустил поиск КС по фильтру 'Имя'.");
        cout << "-----------------------------" << endl;
        cout << "Введите название КС для поиска: ";
        cin >> searchName;
        cin.ignore();
        log("Поиск КС 'Имя': Введено имя:" + searchName);

        if (stations.empty()) {
            cout << "-----------------------------" << endl;
            cout << "По данному запросу ничего не найдено" << endl;
            log("Пользователь ничего не нашёл по данному фильтру.");
        } else {
            cout << "-----------------------------" << endl;
            cout << "Результаты поиска КС:\n";
            bool found = false; 
            for (const auto& station : stations) {
                if (station.second.full_name.find(searchName) != string::npos) {
                    cout << "ID " << station.first;
                    station.second.display();
                    log("Пользователь нашёл по фильтру 'Имя' КС с ID: " + to_string(station.first));
                    found = true;
                }
            }

            if (!found) { 
                cout << "-----------------------------" << endl;
                cout << "По данному запросу ничего не найдено" << endl;
                log("Пользователь ничего не нашёл по данному фильтру.");
            }
        }
    }

    void CompressorStationManager::searchStationUnused() {
        double unusedOperationalShopsCheck;
        long aroundUnusedOperationalShops;
        string command;
        log("Пользователь запустил поиск КС по фильтру 'Незадействованные цехи'.");
        cout << "-----------------------------" << endl;
        cout << "Введите процент незадействованных цехов: ";
        getline(cin, command);
        aroundUnusedOperationalShops = numberOrDefault(command);
        if (aroundUnusedOperationalShops == -1){
            cout << "Неверный ввод. Повторите попытку!" << endl;
            return;
        }

        log("Поиск КС 'Незадействованные цехи': Введён процент: " + to_string(unusedOperationalShopsCheck));


        cout << "-----------------------------" << endl;
        cout << "|              Меню:            |" << endl;
        cout << "| 1. Равно введённому проценту  |" << endl;
        cout << "| 2. Больше введённого процента |" << endl;
        cout << "| 3. Меньше введённого процента |" << endl;
        cout << "-----------------------------" << endl;
        int choice;
        choice = inputIntInRange("Выберете действие: ", 1, 3);
        log("Пользователь выбрал действие: " + to_string(choice));

        if (stations.empty()) {
            cout << "-----------------------------" << endl;
            cout << "По данному запросу ничего не найдено" << endl;
            log("Пользователь ничего не нашёл по данному фильтру.");
            return;
        }

        cout << "-----------------------------" << endl;
        cout << "Результаты поиска КС:\n";

        for (const auto& station : stations) {
            aroundUnusedOperationalShops = station.second.unusedOperationalShops;
            
            bool conditionMet = false;
            switch (choice) {
                case 1: 
                    conditionMet = (aroundUnusedOperationalShops == unusedOperationalShopsCheck);
                    log("Пользователь выбрал параметр: Равно введённому проценту");
                    break;
                case 2: 
                    conditionMet = (aroundUnusedOperationalShops > unusedOperationalShopsCheck);
                    log("Пользователь выбрал параметр: Больше введённого процента");
                    break;
                case 3: 
                    conditionMet = (aroundUnusedOperationalShops < unusedOperationalShopsCheck);
                    log("Пользователь выбрал параметр: Меньше введённого процента");
                    break;
                default:
                    cout << "Некорректный выбор!" << endl;
                    return; 
            }

            if (conditionMet) {
                cout << "ID " << station.first;
                station.second.display(); 
                log("Пользователь нашёл по фильтру 'Незадействованные цехи' КС с ID: " + to_string(station.first));
            }
        }
    }

    void CompressorStationManager::searchStationMenu() {
        cout << "-----------------------------" << endl;
        cout << "|            Меню:          |" << endl;
        cout << "| 1. Искать по имени        |" << endl;
        cout << "| 2. Искать по проценту     |" << endl;
        cout << "|    незадействованных цехов|" << endl;
        cout << "| 0. Назад                  |" << endl;
        cout << "-----------------------------" << endl;
    }

    void CompressorStationManager::searchStationSwitch() {
        if (stations.empty()) {
            cout << "Список КС пуст. Поиск невозможен." << endl;
            return; 
        }
        int choice;
        while (true) {
            searchStationMenu();
            choice = inputIntInRange("Выберете действие: ", 0, 2);
            log("Пользователь выбрал действие: " + to_string(choice));
            cout << "-----------------------------" << endl;
            
            switch (choice) {
            case 1:
                searchStationName();
                break;
            case 2:
                searchStationUnused();
                break;
            case 0:
                CompressorStationSwitch();
                log("Пользователь перешёл в меню 'Управление КС'");
            default:
                cout << "Неверный выбор, попробуйте снова." << endl;
                log("Некорректный выбор: " + choice);
                break;
            }
        }
    }

    void CompressorStationManager::displayCompressorStationMenu() {
        cout << "-----------------------------" << endl;
        cout << "|            Меню:          |" << endl;
        cout << "| 1. Добавить КС            |" << endl;
        cout << "| 2. Редактировать КС       |" << endl;
        cout << "| 3. Удалить КС             |" << endl;
        cout << "| 4. Просмотреть список КС  |" << endl;
        cout << "| 5. Поиск по фильтру       |" << endl;
        cout << "| 0. Назад                  |" << endl;
        cout << "-----------------------------" << endl;
        
    }

    void CompressorStationManager::CompressorStationSwitch() {
        void runProgram();
        int choice;

        while (true) {
            displayCompressorStationMenu();
            choice = inputIntInRange("Выберете действие: ", 0, 5);
            log("Пользователь выбрал действие: " + to_string(choice));
            cout << "-----------------------------" << endl;

            switch (choice) {
            case 1:
                addStation();
                break;
            case 2:
                editStation();
                break;
            case 3:
                removeStation();
                break;
            case 4:
                displayStations();
                break;
            case 5:
                log("Пользователь перешёл в меню 'Фильрация КС'");
                searchStationSwitch();
                break;
            case 0:
                log("Пользователь перешёл в 'Основное меню'");
                runProgram();
            default:
                cout << "Неверный выбор, попробуйте снова." << endl;
                log("Некорректный выбор: " + choice);
                break;
            }
        }
    }
