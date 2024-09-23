#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <stdlib.h>
#include <ctime>

using namespace std;

void log(const string &action) {
        ofstream logFile("log.txt", ios::app);
        if (logFile.is_open()) {
            time_t now = time(0);
            char* dt = ctime(&now);
            logFile << dt << " - " << action << endl;
            logFile.close();
        } else {
            cerr << "Не удалось открыть файл логирования!" << endl;
        }
    }

struct Pipe {
    int id;
    string name;
    string name_line;
    string full_name;
    double length = 0;
    double diameter = 0;
    bool underRepair;
    

    void read() {
        cout << "Введите название трубы: ";
        cin >> name;
        getline(cin, name_line);
        full_name = name + name_line;
        log("Пользователь ввел название трубы: " + full_name);

        while (true) {
            cout << "Введите длину трубы (в км): ";
            cin >> length;
            if (cin.fail() || length < 0 || length == 0) {
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ошибка! Длина трубы должна быть неотрицательным числом. Попробуйте снова." << endl;
            } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                break; 
            }
        }
        log("Пользователь ввел длину трубы: " + to_string(length) + " км");

        while (true) {
            cout << "Введите диаметр трубы (в мм): ";
            cin >> diameter;
            if (cin.fail() || diameter < 0 || diameter == 0) {
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                cout << "Ошибка! Диаметр трубы должен быть неотрицательным числом. Попробуйте снова." << endl;
            } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                break; 
            }
        }
        log("Пользователь ввел диаметр трубы: " + to_string(diameter) + " мм");
        underRepair = false;
    }

    void display() const {
            if (length <= 0 && diameter <= 0) {
                cout << "Труб нет" << endl;
                log("Пользователь запросил данные о трубах, которые отсутствуют.");
                cout << "-----------------------------" << endl;
            } else {
                cout << "ID - " << id 
                    << " : Труба: " << full_name 
                    << ", Длина: " << length 
                    << " км, Диаметр: " << diameter 
                    << " мм, В ремонте: " 
                    << (underRepair ? "Да" : "Нет") 
                    << endl;
                log("Пользователь отобразил данные о трубе: " + full_name);
            }
        }

    void toggleRepair() {
        if (length > 0) {
            underRepair = !underRepair;
            cout << "ID: " << id << " - Признак \"в ремонте\" изменён на: " << (underRepair ? "Да" : "Нет") << endl;
            log("Пользователь установил состояние трубы ID " + to_string(id) + string(underRepair ? " в ремонте" : " в исправном состоянии"));
        }
        else {
            cout << "Трубы нет" << endl;
        }
    }

    string save() const {
        return to_string(id) + ";" + full_name + ";" + to_string(length) + ";" + to_string(diameter) + ";" + to_string(underRepair);
    }

    int getId() const {
        return id;
    }

    void load(const std::string &data) {
        size_t pos1 = data.find(';');
        size_t pos2 = data.find(';', pos1 + 1);
        size_t pos3 = data.find(';', pos2 + 1);
        size_t pos4 = data.find(';', pos3 + 1);

        id = std::stoi(data.substr(0, pos1)); 
        full_name = data.substr(pos1 + 1, pos2 - pos1 - 1);
        length = std::stod(data.substr(pos2 + 1, pos3 - pos2 - 1));
        diameter = std::stod(data.substr(pos3 + 1, pos4 - pos3 - 1));
        underRepair = data.substr(pos4 + 1) == "1";
    }
};

struct CompressorStation {
    int id;
    string name;
    string name_line;
    string full_name;
    int totalShops = 0;
    int operationalShops = 0;
    double unusedOperationalShops;
    double efficiency;

    void read() {
        cout << "Введите название КС: ";
        cin >> name;
        getline(cin, name_line);
        full_name = name + name_line;
        log("Введено название КС: " + full_name);

        while (true) {
            cout << "Введите количество цехов: ";
            cin >> totalShops;
            if (cin.fail() || totalShops < 0 || totalShops == 0) { 
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                cout << "Ошибка! Количество цехов должно быть неотрицательным числом. Попробуйте снова." << endl;
                log("Ошибка ввода количества цехов.");
            } else {
                log("Введено количество цехов: " + to_string(totalShops));
                break;
            }
        }

        while (true) {
            cout << "Введите количество цехов в работе: ";
            cin >> operationalShops;
            if (cin.fail() || operationalShops < 0 || operationalShops == 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ошибка! Количество цехов в работе должно быть неотрицательным числом. Попробуйте снова." << endl;
                log("Ошибка ввода количества цехов в работе.");
            } else if (operationalShops > totalShops) {
                cout << "Количество цехов в работе не должно превышать общее количество цехов. Попробуйте снова." << endl;
                log("Ошибка: количество цехов в работе превышает общее количество.");
            } else {
                log("Введено количество цехов в работе: " + to_string(operationalShops));
                break; 
            }
        }

        while (true) {
            cout << "Введите эффективность в % (1-100): ";
            cin >> efficiency;
            if (cin.fail() || efficiency < 0 || efficiency == 0 || efficiency > 100) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ошибка! Эффективность должна быть неотрицательным числом и меньше 100. Попробуйте снова." << endl;
                log("Ошибка: неверный ввод эффективности.");
            } else {
                log("Введена эффективность: " + to_string(efficiency) + "%");
                break; 
            }
        }
        unusedOperationalShops = ((totalShops - operationalShops) / static_cast<double>(totalShops)) * 100;
    }

    void display() const {
         if (full_name.empty() && totalShops <= 0 && operationalShops <= 0) {
                cout << "КС нет" << endl;
        } else {
            cout << "ID - " << id
                << " : КС: " << full_name 
                << ", Всего цехов: " << totalShops
                << ", Цехов в работе: " << operationalShops
                << setprecision(2) << fixed << ", Процент незадействованных цехов: " << unusedOperationalShops
                << "% , Эффективность: " << efficiency << "%"<< endl;
                 }
        } 

    void toggleShop() {
        if (full_name.empty() && totalShops <= 0 && operationalShops <= 0) {
            cout << "КС нет. Запуск/Остановка цеха невозможна." << endl;
        }
        else {
            int choice;
            cout << "-----------------------------" << endl;
            cout << "| 1. Запустить новый цех    |" << endl;
            cout << "| 2. Остановить работу цеха |" << endl;
            cout << "-----------------------------" << endl;
            cout << "Выберите действие: ";
            cin >> choice;
            cout << "-----------------------------" << endl;
            switch (choice) {
                case 1: 
                    log("Пользователь выбрал запуск цеха");
                    if (operationalShops < totalShops) {
                        operationalShops++;
                        unusedOperationalShops = ((totalShops - operationalShops) / static_cast<double>(totalShops)) * 100;
                        cout << "Запустили новый цех. Теперь в работе: " << operationalShops << endl;
                    } else {
                        cout << "Все цехи уже запущены." << endl;
                    }
                    break;
                case 2: 
                    log("Пользователь выбрал остановку цеха");
                    if (operationalShops > 0) {
                        operationalShops--;
                        unusedOperationalShops = ((totalShops - operationalShops) / static_cast<double>(totalShops)) * 100;
                        cout << "Остановили цех. Теперь в работе: " << operationalShops << endl;
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

    string save() const {
        return to_string(id) + ";" + full_name + ";" + to_string(totalShops) + ";" + 
               to_string(operationalShops) + ";" + 
               to_string(unusedOperationalShops) + ";" + 
               to_string(efficiency);
    }

    int getId() const {
        return id;
    }

    void load(const string &data) {
        size_t pos1 = data.find(';');  
        size_t pos2 = data.find(';', pos1 + 1);  
        size_t pos3 = data.find(';', pos2 + 1);  
        size_t pos4 = data.find(';', pos3 + 1);
        size_t pos5 = data.find(';', pos4 + 1);   

        id = stoi(data.substr(0, pos1)); 
        full_name = data.substr(pos1 + 1, pos2 - pos1 - 1);  
        totalShops = stoi(data.substr(pos2 + 1, pos3 - pos2 - 1));  
        operationalShops = stoi(data.substr(pos3 + 1, pos4 - pos3 - 1));  
        unusedOperationalShops = stoi(data.substr(pos4 + 1, pos5 - pos4 - 1));
        efficiency = efficiency = stod(data.substr(pos5 + 1, data.length() - pos4 - 1));
    }
};

vector<Pipe> pipes;

vector<CompressorStation> stations;

struct PipeManager {
    int nextId = 1;
    int id;

    void addPipe() {
        Pipe newPipe;
        while (true) {
            bool idExists = false;
            for (const auto& pipe : pipes) {
                if (pipe.id == nextId) {
                    idExists = true;
                    break;
                }
            }
            if (!idExists) {
                break; 
            }
            nextId++; 
        }
        newPipe.id = nextId++;
        newPipe.read();
        pipes.push_back(newPipe);
        cout << "Труба добавлена." << endl;
        log("Труба добавлена в систему. ID: " + to_string(newPipe.id));
    }

    void editPipes() {
        cout << "Введите ID труб для редактирования, разделяя запятыми (или * для редактирования всех): ";
        string input;
        getline(cin, input);
        stringstream ss(input);
        string idStr;
        vector<int> ids;
        vector<int> nonExistentIds;
        
        if (input == "*") {
            for (auto &pipe : pipes) {
                pipe.toggleRepair();
            }
            cout << "Все трубы обновлены." << endl;
            log("Пользователь обновил статус 'в ремонте' у всех труб.");
        } else {
             while (getline(ss, idStr, ',')) {
                try {
                    int id = stoi(idStr);
                    ids.push_back(id);
                } catch (const invalid_argument&) {
                    cout << "Ошибка: не является допустимым целым числом. Попробуйте снова." << endl;
                    return;
                } catch (const out_of_range&) {
                    cout << "Ошибка: Значение выходит за пределы допустимого диапазона." << endl;
                    return;
                }
            }

            bool edited = false;
            for (int id : ids) {
                auto it = find_if(pipes.begin(), pipes.end(), [&id](const Pipe& pipe) {
                    return pipe.id == id;
                });
                if (it != pipes.end()) {
                    it->toggleRepair();
                    log("Пользователь обновил статус трубы с ID: " + to_string(id));
                    edited = true;
                } else {
                    nonExistentIds.push_back(id);
                }
            }

            if (edited) {
                cout << "Статус труб обновлен." << endl;
            }

            if (!nonExistentIds.empty()) {
                cout << "Труб(/ы) с ID: ";
                for (size_t i = 0; i < nonExistentIds.size(); ++i) {
                    cout << nonExistentIds[i];
                    if (i < nonExistentIds.size() - 1) {
                        cout << ", ";
                    }
                }
                cout << " не существу(ет/ют)." << endl;
            }
        }
    }

    void deletePipes() {
        cout << "Введите ID труб для удаления, разделяя запятыми: ";
        string input;
        getline(cin, input);

        if (input == "*") {
            pipes.clear(); 
            cout << "Все трубы удалены." << endl;
            log("Пользователь удалил весь список труб.");
            return;
        }

        stringstream ss(input);
        string idStr;
        vector<int> ids;
        vector<int> nonExistentIds;

        while (getline(ss, idStr, ',')) {
            try {
                int id = stoi(idStr);
                ids.push_back(id);
            } catch (const invalid_argument&) {
                cout << "Ошибка: '" << idStr << "' не является допустимым целым числом. Попробуйте снова." << endl;
                log("Ошибка: '" + idStr + "' не является допустимым целым числом.");
                return;
            } catch (const out_of_range&) {
                cout << "Ошибка: Значение '" << idStr << "' выходит за пределы допустимого диапазона." << endl;
                log("Ошибка: Значение '" + idStr + "' выходит за пределы допустимого диапазона.");
                return;
            }
        }
        
        for (int id : ids) {
            auto it = find_if(pipes.begin(), pipes.end(), [&id](const Pipe& pipe) {
                return pipe.id == id;
            });

            if (it != pipes.end()) {
                pipes.erase(it); 
                cout << "Труба с ID " << id << " удалена." << endl;
                log("Удалена труба с ID: " + to_string(id));
            } else {
                nonExistentIds.push_back(id);
            }
        }

        if (!nonExistentIds.empty()) {
                cout << "Труб(/ы) с ID: ";
                for (size_t i = 0; i < nonExistentIds.size(); ++i) {
                    cout << nonExistentIds[i];
                    if (i < nonExistentIds.size() - 1) {
                        cout << ", ";
                    }
                }
                cout << " не существу(ет/ют)." << endl;
            }
    }

    void displayAllPipes() const {
        if (pipes.empty()) {
            cout << "Нет доступных труб." << endl;
        } else {
            cout << "Список всех труб:" << endl;
            cout << endl;
            for (const auto &pipe : pipes) {
                pipe.display();
            }
            cout << endl;
        }
        cout << "-----------------------------" << endl;
        log("Пользователь запросил список труб.");
    }

    void searchPipesTogether() {
        string searchName;
        bool repairStatus;
        log("Пользователь запустил поиск труб по фильтру 'Объеденённый'.");
        cout << "-----------------------------" << endl;
        cout << "Введите название трубы для поиска: ";
        cin >> searchName;
        log("Поиск труб 'Объеденённый': Введено имя:" + searchName);

        cout << "-----------------------------" << endl;
        cout << "Поиск труб 'в ремонте'? (1 - да, 0 - нет): ";
        cin >> repairStatus;
        log("Поиск труб 'Объеденённый': Введён статус (1 - да, 0 - нет):" + to_string(repairStatus));
        if (pipes.empty()) {
            cout << "-----------------------------" << endl;
            cout << "По данному запросу ничего не найдено" << endl;
            log("Пользователь ничего не нашёл по данному фильтру.");
        }
        else {
            cout << "-----------------------------" << endl;
            cout << "Результаты поиска труб:\n";
            for (const auto& pipe : pipes) {
                if (pipe.name == searchName && pipe.underRepair == repairStatus) {
                    pipe.display();
                    log("Пользователь нашёл по фильтру 'Объеденённый' трубу c ID: " + to_string(pipe.id));
                }
            }
            
        }
    }

    void searchPipesName(){
        string searchName;
        log("Пользователь запустил поиск труб по фильтру 'Имя'.");
        cout << "-----------------------------" << endl;
        cout << "Введите название трубы для поиска: ";
        cin >> searchName;
        log("Поиск труб 'Имя': Введено имя:" + searchName);

        if (pipes.empty()) {
            cout << "-----------------------------" << endl;
            cout << "По данному запросу ничего не найдено" << endl;
            log("Пользователь ничего не нашёл по данному фильтру.");
        }
        else {
            cout << "-----------------------------" << endl;
            cout << "Результаты поиска труб:\n";
            for (const auto& pipe : pipes) {
                if (pipe.name == searchName) {
                    pipe.display();
                    log("Пользователь нашёл по фильтру 'Имя' трубу с ID: " + to_string(pipe.id));
                }
            }
            
        }
    
    }

    void searchPipesRepair(){
        bool repairStatus;
        log("Пользователь запустил поиск труб по фильтру 'Статус ремонта'.");
        cout << "-----------------------------" << endl;
        cout << "Поиск труб 'в ремонте'? (1 - да, 0 - нет): ";
        cin >> repairStatus;
        log("Поиск труб 'Объеденённый': Введён статус (1 - да, 0 - нет):" + to_string(repairStatus));
        if (pipes.empty()) {
            cout << "-----------------------------" << endl;
            cout << "По данному запросу ничего не найдено" << endl;
            log("Пользователь ничего не нашёл по данному фильтру.");
        }
        else {
            cout << "-----------------------------" << endl;
            cout << "Результаты поиска труб:\n";
            for (const auto& pipe : pipes) {
                if (pipe.underRepair == repairStatus) {
                    pipe.display();
                    log("Пользователь нашёл по фильтру 'Статус ремонта' трубу с ID: " + to_string(pipe.id));
                }
            }
            
        }
    }

    void searchPipeMenu() {
        cout << "-----------------------------" << endl;
        cout << "|            Меню:          |" << endl;
        cout << "| 1. Искать по имени        |" << endl;
        cout << "| 2. Искать по статусу      |" << endl;
        cout << "| 3. Объеденённый поиск     |" << endl;
        cout << "| 0. Назад                  |" << endl;
        cout << "-----------------------------" << endl;
    }

    void searchPipeSwitch() {
        int choice; 
        string wrong;
        while (true) {
            searchPipeMenu();
            cout << "Выберите действие: ";
            cin >> choice;
            log("Пользователь в меню 'Фильрация труб' ввел действие : " + to_string(choice));
            getline(cin, wrong);

            if (cin.fail() || choice < 0 || choice > 3) {
                cout << "Неверный ввод. Пожалуйста, введите целое число от 0 до 3" << endl;
                log("Ошибка ввода пользовалетя");
                cin.clear(); 
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                continue; 
            }

            cout << "-----------------------------" << endl;

            switch (choice) {
            case 1:
                searchPipesName();
                break;
            case 2:
                searchPipesRepair();
                break;
            case 3:
                searchPipesTogether();
                break;
            case 0:
                PipeSwitch();
                log("Пользователь перешёл в меню 'Управление трубами'");
            }
        }
    }

    void displayPipeMenu() {
        cout << "-----------------------------" << endl;
        cout << "|            Меню:          |" << endl;
        cout << "| 1. Добавить трубу         |" << endl;
        cout << "| 2. Редактировать трубу    |" << endl;
        cout << "| 3. Удалить трубу          |" << endl;
        cout << "| 4. Список труб            |" << endl;
        cout << "| 5. Поиск по фильтру       |" << endl;
        cout << "| 0. Назад                  |" << endl;
        cout << "-----------------------------" << endl;
    }

    void PipeSwitch(){
        void runProgram();
        int choice; 
        string wrong;
        while (true) {
            displayPipeMenu();
            cout << "Выберите действие: ";
            cin >> choice;
            log("Пользователь в меню 'Управление трубами' ввел действие : " + to_string(choice));
            getline(cin, wrong);

            if (cin.fail() || choice < 0 || choice > 5) {
                cout << "Неверный ввод. Пожалуйста, введите целое число от 0 до 5." << endl;
                log("Ошибка ввода пользовалетя");
                cin.clear(); 
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                continue; 
            }

            cout << "-----------------------------" << endl;

            switch (choice) {
            case 1:
                addPipe();
                break;
            case 2:
                editPipes();
                break;
            case 3:
                deletePipes();
                break;
            case 4:
                displayAllPipes();
                break;
            case 5:
                searchPipeSwitch();
                log("Пользователь перешёл в меню 'Фильрация труб'");
                break;
            case 0:
                log("Пользователь перешёл в 'Основное меню'");
                runProgram();
            }
        }
    }
};

struct CompressorStationManager {
    int nextId = 1;
    int id;
    
    void addStation() {
        CompressorStation station;
        while (true) {
            bool idExists = false;
            for (const auto& station : stations) {
                if (station.id == nextId) {
                    idExists = true;
                    break;
                }
            }
            if (!idExists) {
                break; 
            }
            nextId++; 
        }
        station.id = nextId++; 
        station.read();
        stations.push_back(station);
        cout << "Компрессорная станция добавлена." << endl;
        log("Компрессорная станция добавлена в систему. ID: " + to_string(station.id));
    }

    void displayStations() const {
        if (stations.empty()) {
            cout << "Нет доступных компрессорных станций." << endl;
            return;
        }
        cout << "Список компрессорных станций:" << endl;
        cout << endl;
        for (const auto& station : stations) {
            station.display();
        }
        cout << endl;
        log("Пользователь запросил список компрессорных станций.");
    }

    void removeStation() {
        cout << "Введите ID КС для удаления: ";
        cin >> id;
        if (id <= 0 || id > stations.size()) {
            cout << "Ошибка! Станции с таким ID не существует." << endl;
            return;
        }
        stations.erase(stations.begin() + id - 1);
        cout << "Компрессорная станция была удалена." << endl;
    }

    void editStation() {
        cout << "Введите ID КС для редактирования: ";
        cin >> id;
        for (auto &station : stations) {
            if (station.id == id) {
                cout << "Редактирование КС с ID " << id << endl;
                station.toggleShop();
                log("Пользователь обновил статус трубы с ID: " + to_string(station.id));
                return;
            }
        }
        cout << "КС с ID " << id << " не найдена." << endl;
    }

    void searchStationTogether() {
        string searchName;
        double unusedOperationalShopsCheck;
        int aroundUnusedOperationalShops;
        log("Пользователь запустил поиск КС по фильтру 'Объеденённый'.");
        cout << "-----------------------------" << endl;
        cout << "Введите название КС для поиска: ";
        cin >> searchName;
        log("Поиск КС 'Объеденённый': Введено имя:" + searchName);

        cout << "-----------------------------" << endl;
        cout << "Введите процент незадействованных цехов: ";
        cin >> unusedOperationalShopsCheck;
        log("Поиск труб 'Объеденённый': Введён процент незадействованных цехов:" + to_string(unusedOperationalShopsCheck));
        if (stations.empty()) {
            cout << "-----------------------------" << endl;
            cout << "По данному запросу ничего не найдено" << endl;
            log("Пользователь ничего не нашёл по данному фильтру.");
        }
        else {
            cout << "-----------------------------" << endl;
            cout << "Результаты поиска труб:\n";
            for (const auto& station : stations) {
                aroundUnusedOperationalShops = station.unusedOperationalShops;
                if (station.name == searchName && (station.unusedOperationalShops == unusedOperationalShopsCheck || aroundUnusedOperationalShops == unusedOperationalShopsCheck)) {
                    station.display();
                    log("Пользователь нашёл по фильтру 'Объеденённый' КС с ID: " + to_string(station.id));
                }
            }
            
        }
    }

    void searchStationName(){
        string searchName;
        log("Пользователь запустил поиск КС по фильтру 'Имя'.");
        cout << "-----------------------------" << endl;
        cout << "Введите название КС для поиска: ";
        cin >> searchName;
        log("Поиск КС 'Имя': Введено имя:" + searchName);

        if (stations.empty()) {
            cout << "-----------------------------" << endl;
            cout << "По данному запросу ничего не найдено" << endl;
            log("Пользователь ничего не нашёл по данному фильтру.");
        }
        else {
            cout << "-----------------------------" << endl;
            cout << "Результаты поиска КС:\n";
            for (const auto& station : stations) {
                if (station.name == searchName) {
                    station.display();
                    log("Пользователь нашёл по фильтру 'Имя' КС с ID: " + to_string(station.id));
                }
            }
            
        }
    
    }

    void searchStationUnused(){
        double unusedOperationalShopsCheck;
        int aroundUnusedOperationalShops;
        log("Пользователь запустил поиск КС по фильтру 'Незадействованные цехи'.");
        cout << "-----------------------------" << endl;
        cout << "Введите процент незадействованных цехов: ";
        cin >> unusedOperationalShopsCheck;
        log("Поиск КС 'Незадействованные цехи': Введён процент:" + to_string(unusedOperationalShopsCheck));
        if (stations.empty()) {
            cout << "-----------------------------" << endl;
            cout << "По данному запросу ничего не найдено" << endl;
            log("Пользователь ничего не нашёл по данному фильтру.");
        }
        else {
            cout << "-----------------------------" << endl;
            cout << "Результаты поиска КС:\n";
            for (const auto& station : stations) {
                aroundUnusedOperationalShops = station.unusedOperationalShops;
                if (station.unusedOperationalShops == unusedOperationalShopsCheck || aroundUnusedOperationalShops == unusedOperationalShopsCheck) {
                    station.display();
                    log("Пользователь нашёл по фильтру 'Незадействованные цехи' КС с ID: " + to_string(station.id));
                }
            }
            
        }
    }

    void searchStationMenu() {
        cout << "-----------------------------" << endl;
        cout << "|            Меню:          |" << endl;
        cout << "| 1. Искать по имени        |" << endl;
        cout << "| 2. Искать по проценту     |" << endl;
        cout << "|    незадействованных цехов|" << endl;
        cout << "| 3. Объеденённый поиск     |" << endl;
        cout << "| 0. Назад                  |" << endl;
        cout << "-----------------------------" << endl;
    }

    void searchStationSwitch() {
        int choice; 
        string wrong;
        while (true) {
            searchStationMenu();
            cout << "Выберите действие: ";
            cin >> choice;
            log("Пользователь в меню 'Фильрация КС' ввел действие : " + to_string(choice));
            getline(cin, wrong);

            if (cin.fail() || choice < 0 || choice > 3) {
                cout << "Неверный ввод. Пожалуйста, введите целое число от 0 до 3" << endl;
                log("Ошибка ввода пользовалетя");
                cin.clear(); 
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                continue; 
            }

            cout << "-----------------------------" << endl;

            switch (choice) {
            case 1:
                searchStationName();
                break;
            case 2:
                searchStationUnused();
                break;
            case 3:
                searchStationTogether();
                break;
            case 0:
                CompressorStationSwitch();
                log("Пользователь перешёл в меню 'Управление КС'");
            }
        }
    }

    void displayCompressorStationMenu() {
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

    void CompressorStationSwitch(){
        void runProgram();
        int choice; 
        string wrong;
        while (true) {
            displayCompressorStationMenu();
            cout << "Выберите действие: ";
            cin >> choice;
            log("Пользователь в меню 'Управление КС' ввел действие : " + to_string(choice));
            getline(cin, wrong);

            if (cin.fail() || choice < 0 || choice > 5) {
                cout << "Неверный ввод. Пожалуйста, введите целое число от 0 до 5." << endl;
                log("Ошибка ввода пользовалетя");
                cin.clear(); 
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                continue; 
            }

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
            }
        }
    }
};

void saveToFile() {
    string filename;
    cout << "Введите имя файла для сохранения компрессорных станций и труб: ";
    cin >> filename;
    ofstream ofs(filename + ".txt");
    log("Начата операция сохранения в файл: " + filename + ".txt");
    if (ofs) {
        for (size_t i = 0; i < stations.size(); ++i) {
            ofs << "KS " << i + 1 << ": " << stations[i].save() << endl; 
        }

        for (size_t i = 0; i < pipes.size(); ++i) {
            ofs << "pipe " << i + 1 << ": " << pipes[i].save() << endl;
        }

        ofs.close();
        cout << "Данные компрессорных станций и труб сохранены в " << filename << ".txt" << endl;
        log("Успешно сохранено в файл: " + filename + ".txt");
    } else {
        cerr << "Ошибка открытия файла для сохранения." << endl;
        log("Ошибка открытия файла для сохранения: " + filename + ".txt");
    }
}

void loadFromFile() {
    string filename;
    cout << "Введите имя файла для загрузки компрессорных станций и труб: ";
    cin >> filename;
    log("Попытка загрузки данных из файла: " + filename + ".txt");
    
    ifstream ifs(filename + ".txt");
    if (ifs) {
        string line;
        while (getline(ifs, line)) {
            try {
                if (line.substr(0, 2) == "KS") {
                    CompressorStation station;
                    station.load(line.substr(3));

                    bool exists = false;
                    for (const auto& s : stations) {
                        if (s.getId() == station.getId()) {
                            exists = true;
                            break;
                        }
                    }
                    if (!exists) {
                        stations.push_back(station);
                        log("Загружена компрессорная станция с ID: " + to_string(station.id));
                    } else {
                        cerr << "Компрессорная станция с ID " << station.getId() << " уже существует." << endl;
                        log("Ошибка: Компрессорная станция с ID " + to_string(station.id) + " уже существует.");
                    }
                } else if (line.substr(0, 4) == "pipe") {
                    Pipe pipe;
                    pipe.load(line.substr(5));

                    bool exists = false;
                    for (const auto& p : pipes) {
                        if (p.getId() == pipe.getId()) {
                            exists = true;
                            break;
                        }
                    }
                    if (!exists) {
                        pipes.push_back(pipe);
                        log("Загружена труба с ID: " + to_string(pipe.id));
                    } else {
                        cerr << "Труба с ID " << pipe.getId() << " уже существует." << endl;
                        log("Ошибка: Труба с ID " + to_string(pipe.id) + " уже существует.");
                    }
                }
            } catch (const exception& e) {
                cerr << "Ошибка при загрузке строки: " << line << " -> " << e.what() << endl;
                log("Ошибка при загрузке строки '" + line + "': " + e.what());
            }
        }
        log("Загрузка данных завершена.");
    } else {
        cerr << "Не удалось открыть файл: " << filename << ".txt" << endl;
        log("Ошибка: Не удалось открыть файл " + filename + ".txt");
    }
}

void displayMenu() {
    cout << "-----------------------------" << endl;
    cout << "|            Меню:          |" << endl;
    cout << "| 1. Трубы                  |" << endl;
    cout << "| 2. КС                     |" << endl;
    cout << "| 3. Просмотр всех объектов |" << endl;
    cout << "| 4. Сохранить              |" << endl;
    cout << "| 5. Загрузить              |" << endl;
    cout << "| 0. Выход                  |" << endl;
    cout << "-----------------------------" << endl;
}

void runProgram() {
    PipeManager pmanager;
    CompressorStationManager cmanager;
    Pipe pipe;
    CompressorStation cs;
    int choice;
    string wrong;

    while (true) {
        displayMenu();
        cout << "Выберите действие: ";
        

        cin >> choice;
        log("Пользователь выбрал действие: " + to_string(choice));
        getline(cin, wrong);

        if (cin.fail() || choice < 0 || choice > 5) {
            cout << "Неверный ввод. Пожалуйста, введите целое число от 0 до 7." << endl;
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        cout << "-----------------------------" << endl;

        switch (choice) {
            case 1:
                pmanager.PipeSwitch();
                break;
            case 2:
                cmanager.CompressorStationSwitch();
                break;
            case 3:
                pmanager.displayAllPipes();
                cmanager.displayStations();
                log("Отображение всех труб и станций выполнено.");
                break;
            case 4:
                saveToFile();
                break;
            case 5:
                loadFromFile();
                break;
            case 0:
                cout << "Выход из программы." << endl;
                log("Выход из программы.");
                exit( 1 );
            default:
                cout << "Неверный выбор, попробуйте снова." << endl;
                log("Некорректный выбор: " + to_string(choice));
                break;
        }
    }
}

int main() {
    log("Произведён успешный запуск программы!");
    runProgram();
    return 0;
}