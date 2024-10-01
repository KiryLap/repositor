#include "PipeManager.h"
#include "Pipe.h"
#include <vector>
#include <iostream>
#include <map>

extern std::map<int, Pipe> pipes; 
using namespace std;

    string PipeManager::savePipe(const Pipe &pipe) const {
            return pipe.full_name + ";" + to_string(pipe.length) + ";" + to_string(pipe.diameter) + ";" + to_string(pipe.underRepair);
        }

    void PipeManager::toggleRepair(Pipe &pipe, int id) {
        if (pipe.length > 0) {
            pipe.underRepair = !pipe.underRepair;
            cout << "ID: " << id << " - Признак \"в ремонте\" изменён на: " << (pipe.underRepair ? "Да" : "Нет") << endl;
            log("Пользователь установил состояние трубы ID " + to_string(id) + string(pipe.underRepair ? " в ремонте" : " в исправном состоянии"));
        }
        else {
            cout << "Трубы нет" << endl;
        }
    }

    void PipeManager::addPipe() {
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

    void PipeManager::editPipes() {
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

    void PipeManager::deletePipes() {
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

    void PipeManager::displayAllPipes() {
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

    void PipeManager::editPipesFound(const vector<int>& foundIds) {
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

    void PipeManager::deletePipesFound(const vector<int>& foundIds) {
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

    void PipeManager::displayPipeMenuFound() {
            cout << "-----------------------------" << endl;
            cout << "|         Меню списка:      |" << endl;
            cout << "| 1. Удалить трубы          |" << endl;
            cout << "| 2. Редактировать трубы    |" << endl;
            cout << "| 0. Назад                  |" << endl;
            cout << "-----------------------------" << endl;
        }

    void PipeManager::handleFoundPipeMenu(const vector<int>& foundPipeIds) {
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

    void PipeManager::searchPipesName() {
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

    void PipeManager::searchPipesRepair() {
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

    void PipeManager::searchPipeMenu() {
        cout << "-----------------------------" << endl;
        cout << "|            Меню:          |" << endl;
        cout << "| 1. Искать по имени        |" << endl;
        cout << "| 2. Искать по статусу      |" << endl;
        cout << "| 0. Назад                  |" << endl;
        cout << "-----------------------------" << endl;
    }

    void PipeManager::searchPipeSwitch() {
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

    void PipeManager::displayPipeMenu() {
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

    void PipeManager::PipeSwitch() {
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