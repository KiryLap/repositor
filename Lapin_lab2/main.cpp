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
#include <map>

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

double readPositiveDouble(const string& prompt) {
        double value;
        while (true) {
            cout << prompt;
            cin >> value;

            if (cin.fail() || value <= 0) {
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ошибка! Значение должно быть положительным числом. Попробуйте снова." << endl;
            } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                return value;
            }
        }
    }

int readPositiveInt(const string& prompt, const string& errorMsg, int max = numeric_limits<int>::max()) {
        int value;
        while (true) {
            cout << prompt;
            cin >> value;
            if (cin.fail() || value < 0) {
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                cout << errorMsg << endl;
                log("Ошибка ввода: " + errorMsg);
            } else if (value > max) {
                cout << "Ошибка! Значение не должно превышать " << max << ". Попробуйте снова." << endl;
                log("Ошибка: значение превышает допустимое.");
            } else {
                log("Введено значение: " + to_string(value));
                return value;
            }
        }
    }

bool isNumber(const std::string& s)
{
    return  s.length() < 2 and !s.empty() && std::find_if(s.begin(),
        s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

long numberOrDefault(const string& command){
    if (isNumber(command)) {
        return strtol(command.c_str(), NULL, 10);
    }
    return -1;
}

int inputIntInRange(const string& prompt, int minValue, int maxValue) {
    int value;

    while (true) {
        cout << prompt;
        string input;
        getline(cin, input);
        stringstream ss(input);
        if (ss >> value) {
            if (ss.eof()) {
                if (value >= minValue && value <= maxValue) {
                    return value;
                } else {
                    cout << "----------------------------------------" << endl;
                    cout << "Ошибка: введите число от " << minValue << " до " << maxValue << "." << endl;
                    cout << "----------------------------------------" << endl;
                }
            } else {
                cout << "---------------------------------------" << endl;
                cout << "Ошибка: введите корректное целое число." << endl;
                cout << "----------------------------------------" << endl;
            }
        } else {
            cout << "----------------------------------------" << endl;
            cout << "Ошибка: введите корректное целое число." << endl;
            cout << "----------------------------------------" << endl;
        }
    }
}

class Pipe {
public:
    string full_name;
    double length = 0;
    double diameter = 0;
    bool underRepair;

    void read() {
        cout << "Введите название трубы: ";
        getline(cin, full_name);
        log("Пользователь ввел название трубы: " + full_name);

        length = readPositiveDouble("Введите длину трубы (в км): ");
        log("Пользователь ввел длину трубы: " + to_string(length) + " км");

        diameter = readPositiveDouble("Введите диаметр трубы (в мм): ");
        log("Пользователь ввел диаметр трубы: " + to_string(diameter) + " мм");
        underRepair = false;
    }

    void display() const {
            if (length <= 0 && diameter <= 0) {
                cout << "Труб нет" << endl;
                log("Пользователь запросил данные о трубах, которые отсутствуют.");
                cout << "-----------------------------" << endl;
            } else {
                cout << " : Труба: " << full_name 
                    << ", Длина: " << length 
                    << " км, Диаметр: " << diameter 
                    << " мм, В ремонте: " 
                    << (underRepair ? "Да" : "Нет") 
                    << endl;
                log("Пользователь отобразил данные о трубе: " + full_name);
            }
        }

    void load(const string &data) {
        size_t pos1 = data.find(';'); 
        size_t pos2 = data.find(';', pos1 + 1);
        size_t pos3 = data.find(';', pos2 + 1); 

        full_name = data.substr(0, pos1); 
        length = stod(data.substr(pos1 + 1, pos2 - pos1 - 1));
        diameter = stod(data.substr(pos2 + 1, pos3 - pos2 - 1)); 
        underRepair = data.substr(pos3 + 1) == "1"; 
    }

};

class CompressorStation {
public:
    string full_name;
    int totalShops = 0;
    int operationalShops = 0;
    double unusedOperationalShops;
    double efficiency;

    void read() {
        cout << "Введите название КС: ";
        getline(cin, full_name);
        log("Введено название КС: " + full_name);
        totalShops = readPositiveInt("Введите количество цехов: ", "Ошибка! Количество цехов должно быть неотрицательным числом. Попробуйте снова.");
        operationalShops = readPositiveInt("Введите количество цехов в работе: ", "Ошибка! Количество цехов в работе должно быть неотрицательным числом. Попробуйте снова.", totalShops);
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
        cin.ignore();
    }

    void display() const {
         if (full_name.empty() && totalShops <= 0 && operationalShops <= 0) {
                cout << "КС нет" << endl;
        } else {
            cout << " : КС: " << full_name 
                << ", Всего цехов: " << totalShops
                << ", Цехов в работе: " << operationalShops
                << setprecision(2) << fixed << ", Процент незадействованных цехов: " << unusedOperationalShops
                << "% , Эффективность: " << efficiency << "%"<< endl;
                 }
        } 

    void load(const string &data) {
        size_t pos1 = data.find(';');  
        size_t pos2 = data.find(';', pos1 + 1);  
        size_t pos3 = data.find(';', pos2 + 1);  
        size_t pos4 = data.find(';', pos3 + 1);  

        full_name = data.substr(0, pos1);  
        totalShops = stoi(data.substr(pos1 + 1, pos2 - pos1 - 1));  
        operationalShops = stoi(data.substr(pos2 + 1, pos3 - pos2 - 1));  
        unusedOperationalShops = stoi(data.substr(pos3 + 1, pos4 - pos3 - 1));
        efficiency = efficiency = stod(data.substr(pos4 + 1, data.length() - pos3 - 1));
    }
};

map<int, Pipe> pipes;
map<int, CompressorStation> stations;

class PipeManager {
private:
    int nextId = 1;

public:
    
    string savePipe(const Pipe &pipe) const {
            return pipe.full_name + ";" + to_string(pipe.length) + ";" + to_string(pipe.diameter) + ";" + to_string(pipe.underRepair);
        }

    void toggleRepair(Pipe &pipe, int id) {
        if (pipe.length > 0) {
            pipe.underRepair = !pipe.underRepair;
            cout << "ID: " << id << " - Признак \"в ремонте\" изменён на: " << (pipe.underRepair ? "Да" : "Нет") << endl;
            log("Пользователь установил состояние трубы ID " + to_string(id) + string(pipe.underRepair ? " в ремонте" : " в исправном состоянии"));
        }
        else {
            cout << "Трубы нет" << endl;
        }
    }

    void addPipe() {
        Pipe newPipe; 
        newPipe.read(); 

        while (pipes.find(nextId) != pipes.end()) {
            nextId++;
        }

        pipes[nextId] = newPipe;

        cout << "Труба добавлена. ID: " << nextId << endl;
        log("Труба добавлена в систему. ID: " + to_string(nextId));

        nextId++; 
    }

    void editPipes() {
        if (pipes.empty()) {
            cout << "Список труб пуст. Редактирование невозможно." << endl;
            return; 
        }
        cout << "Введите ID труб для редактирования, разделяя запятыми (или * для редактирования всех): ";
        string input;
        getline(cin, input);
        stringstream ss(input);
        string idStr;
        vector<int> ids;
        vector<int> nonExistentIds;

        if (input == "*") {
            for (auto &pipePair : pipes) { 
                toggleRepair(pipePair.second, pipePair.first); 
            }
            cout << "Все трубы обновлены." << endl;
            log("Пользователь обновил статус 'в ремонте' у всех труб.");
        } else {
            while (getline(ss, idStr, ',')) {
                try {
                    int id = stoi(idStr); 
                    ids.push_back(id);
                } catch (const invalid_argument &) {
                    cout << "Ошибка: не является допустимым целым числом. Попробуйте снова." << endl;
                    return;
                } catch (const out_of_range &) {
                    cout << "Ошибка: значение выходит за пределы допустимого диапазона." << endl;
                    return;
                }
            }

            bool edited = false;
            for (int id : ids) {
                auto it = pipes.find(id); 
                if (it != pipes.end()) { 
                    toggleRepair(it->second, id); 
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
        if (pipes.empty()) {
            cout << "Список труб пуст. Удаление невозможно." << endl;
            return; 
        }
        cout << "Введите ID труб для удаления, разделяя запятыми (или * для удаления всех): ";
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
            auto it = pipes.find(id); 

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
                cout << "ID " <<pipe.first;
                pipe.second.display();
            }
            cout << endl;
        }
        cout << "-----------------------------" << endl;
        log("Пользователь запросил список труб.");
    }

    void editPipesFound(const vector<int>& foundIds) {
    cout << "Введите ID труб для редактирования, разделяя запятыми (или * для редактирования всех): ";
    string input;
    getline(cin, input);
    stringstream ss(input);
    string idStr;
    vector<int> ids;
    vector<int> nonExistentIds;

    if (input == "*") {
        // Обновляем только найденные трубы
        for (int id : foundIds) {
            auto it = pipes.find(id); 
            if (it != pipes.end()) {
                toggleRepair(it->second, id);
            }
        }
        cout << "Все найденные трубы обновлены." << endl;
        log("Пользователь обновил статус 'в ремонте' у всех найденных труб.");
    } else {
        while (getline(ss, idStr, ',')) {
            try {
                int id = stoi(idStr); 
                ids.push_back(id);
            } catch (const invalid_argument &) {
                cout << "Ошибка: не является допустимым целым числом. Попробуйте снова." << endl;
                return;
            } catch (const out_of_range &) {
                cout << "Ошибка: значение выходит за пределы допустимого диапазона." << endl;
                return;
            }
        }

        bool edited = false;
        for (int id : ids) {
            if (find(foundIds.begin(), foundIds.end(), id) != foundIds.end()) {
                auto it = pipes.find(id); 
                if (it != pipes.end()) { 
                    toggleRepair(it->second, id); 
                    edited = true;
                }
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

    void deletePipesFound(const vector<int>& foundIds) {
        cout << "Введите ID труб для удаления, разделяя запятыми (или * для удаления всех найденных труб): ";
        string input;
        getline(cin, input);

        if (input == "*") {
            for (int id : foundIds) {
                pipes.erase(id);
                cout << "Труба с ID " << id << " удалена." << endl;
                log("Удалена труба с ID: " + to_string(id));
            }
            cout << "Все найденные трубы удалены." << endl;
            log("Пользователь удалил все найденные трубы.");
            return;
        }

        stringstream ss(input);
        string idStr;
        vector<int> ids;
        vector<int> nonExistentIds;

        while (getline(ss, idStr, ',')) {
            try {
                int id = stoi(idStr);
                if (find(foundIds.begin(), foundIds.end(), id) != foundIds.end()) {
                    ids.push_back(id);
                } else {
                    nonExistentIds.push_back(id);
                }
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
            auto it = pipes.find(id); 
            if (it != pipes.end()) {
                pipes.erase(it); 
                cout << "Труба с ID " << id << " удалена." << endl;
                log("Удалена труба с ID: " + to_string(id));
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

    void displayPipeMenuFound() {
            cout << "-----------------------------" << endl;
            cout << "|         Меню списка:      |" << endl;
            cout << "| 1. Удалить трубы          |" << endl;
            cout << "| 2. Редактировать трубы    |" << endl;
            cout << "| 0. Назад                  |" << endl;
            cout << "-----------------------------" << endl;
        }

    void handleFoundPipeMenu(const vector<int>& foundPipeIds) {
        int choice;
        while (true) {
            displayPipeMenuFound();
            choice = inputIntInRange("Выберете действие: ", 0, 2);
            log("Пользователь выбрал действие: " + to_string(choice));

            cout << "-----------------------------" << endl;

            switch (choice) {
                case 1:
                    deletePipesFound(foundPipeIds); 
                    break;
                case 2:
                    editPipesFound(foundPipeIds); 
                    break;
                case 0:
                    log("Пользователь перешёл в 'Фильтрация труб'");
                    return;
                default:
                    cout << "Неверный выбор, попробуйте снова." << endl;
                    log("Некорректный выбор: " + choice);
                    break;
            }
        }
    }

    void searchPipesName() {
        string searchName;
        log("Пользователь запустил поиск труб по фильтру 'Имя'.");
        cout << "-----------------------------" << endl;
        cout << "Введите название трубы для поиска: ";
        cin >> searchName;
        cin.ignore();
        log("Поиск труб 'Имя': Введено имя:" + searchName);

        if (pipes.empty()) {
            cout << "-----------------------------" << endl;
            cout << "По данному запросу ничего не найдено" << endl;
            log("Пользователь ничего не нашёл по данному фильтру.");
        } else {
            cout << "-----------------------------" << endl;
            cout << "Результаты поиска труб:\n";
            vector<int> foundPipeIds; 
            bool found = false;

            for (const auto& pipe : pipes) {
                if (pipe.second.full_name.find(searchName) != string::npos) {
                    cout << "ID " << pipe.first;
                    pipe.second.display();
                    log("Пользователь нашёл по фильтру 'Имя' трубу с ID: " + to_string(pipe.first));
                    foundPipeIds.push_back(pipe.first); 
                    found = true;
                }
            }
            if (!found) {
                cout << "-----------------------------" << endl;
                cout << "По данному запросу ничего не найдено" << endl;
                log("Пользователь ничего не нашёл по данному фильтру.");
                return;
            }
            else{
                handleFoundPipeMenu(foundPipeIds);
            }
        }
    }

    void searchPipesRepair() {
    bool repairStatus;
    log("Пользователь запустил поиск труб по фильтру 'Статус ремонта'.");
    cout << "-----------------------------" << endl;
    cout << "Поиск труб 'в ремонте'? (1 - да, 0 - нет):";
    repairStatus = inputIntInRange(" ", 0, 1);
    log("Поиск труб 'Статус ремонта': Введён статус (1 - да, 0 - нет):" + to_string(repairStatus));

    if (pipes.empty()) {
        cout << "-----------------------------" << endl;
        cout << "По данному запросу ничего не найдено" << endl;
        log("Пользователь ничего не нашёл по данному фильтру.");
        return;
    }

    cout << "-----------------------------" << endl;
    cout << "Результаты поиска труб:\n";
    
    vector<int> foundPipeIds;
    
    for (const auto& pipe : pipes) {
        if (pipe.second.underRepair == repairStatus) {
            cout << "ID " << pipe.first;
            pipe.second.display();
            log("Пользователь нашёл по фильтру 'Статус ремонта' трубу с ID: " + to_string(pipe.first));
            foundPipeIds.push_back(pipe.first); 
            
        }
    }
    handleFoundPipeMenu(foundPipeIds);
    if (foundPipeIds.empty()) {
        cout << "-----------------------------" << endl;
        cout << "По данному запросу ничего не найдено" << endl;
        log("Пользователь ничего не нашёл по данному фильтру.");
        return;
    }
}

    void searchPipeMenu() {
        cout << "-----------------------------" << endl;
        cout << "|            Меню:          |" << endl;
        cout << "| 1. Искать по имени        |" << endl;
        cout << "| 2. Искать по статусу      |" << endl;
        cout << "| 0. Назад                  |" << endl;
        cout << "-----------------------------" << endl;
    }

    void searchPipeSwitch() {
        if (pipes.empty()) {
            cout << "Список труб пуст. Поиск невозможен." << endl;
            return; 
        }
        int choice;
        while (true) {
            searchPipeMenu();
            choice = inputIntInRange("Выберете действие: ", 0, 5);
            log("Пользователь выбрал действие: " + to_string(choice));

            cout << "-----------------------------" << endl;

            switch (choice) {
            case 1:
                searchPipesName();
                break;
            case 2:
                searchPipesRepair();
                break;
            case 0:
                PipeSwitch();
                log("Пользователь перешёл в меню 'Управление трубами'");
            default:
                cout << "Неверный выбор, попробуйте снова." << endl;
                log("Некорректный выбор: " + choice);
                break;
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

    void PipeSwitch() {
        void runProgram();
        int choice;
        while (true) {
            displayPipeMenu();
            choice = inputIntInRange("Выберете действие: ", 0, 5);
            log("Пользователь выбрал действие: " + to_string(choice));

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
            default:
                cout << "Неверный выбор, попробуйте снова." << endl;
                log("Некорректный выбор: " + choice);
                break;
            }
        }
    }
};

class CompressorStationManager {
private:
    int nextId = 1;

public:
    
    string saveCS(const CompressorStation &stations) const {
        return stations.full_name + ";" + to_string(stations.totalShops) + ";" + 
               to_string(stations.operationalShops) + ";" + 
               to_string(stations.unusedOperationalShops) + ";" + 
               to_string(stations.efficiency);
    }

    void toggleShop(CompressorStation &stations, int id) {
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

    void addStation() {
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

    void displayStations() const {
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

    void removeStation() {
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

    void editStation() {
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

    void searchStationName(){
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

    void searchStationUnused() {
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

    void searchStationMenu() {
        cout << "-----------------------------" << endl;
        cout << "|            Меню:          |" << endl;
        cout << "| 1. Искать по имени        |" << endl;
        cout << "| 2. Искать по проценту     |" << endl;
        cout << "|    незадействованных цехов|" << endl;
        cout << "| 0. Назад                  |" << endl;
        cout << "-----------------------------" << endl;
    }

    void searchStationSwitch() {
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

    void CompressorStationSwitch() {
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
};

void saveToFile(const map<int, Pipe>& pipes, const map<int, CompressorStation>& stations, CompressorStationManager& cmanager, PipeManager& pmanager) {
    string filename;
    cout << "Введите имя файла для сохранения компрессорных станций и труб: ";
    cin >> filename;
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
    PipeManager pManager; 
    CompressorStationManager sManager;
    int choice;

    while (true) {
        displayMenu();
        choice = inputIntInRange("Выберете действие: ", 0, 5);
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
                log("Отображение всех труб и станций выполнено.");
                break;
            case 4:
                saveToFile(pipes,stations,sManager,pManager);
                break;
            case 5:
                loadFromFile(pipes, stations);
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
    log("Произведён успешный запуск программы!");
    runProgram();
    return 0;
}