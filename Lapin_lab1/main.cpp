#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>

using namespace std;

struct Pipe {
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
        underRepair = false;
    }

    void display() {
            if (length <= 0 && diameter <= 0) {
                cout << "Трубы нет" << endl;
                cout << "-----------------------------" << endl;
            } else {
                cout << "Труба: " << full_name 
                    << ", Длина: " << length 
                    << " км, Диаметр: " << diameter 
                    << " мм, В ремонте: " 
                    << (underRepair ? "Да" : "Нет") 
                    << endl;
                cout << "-----------------------------" << endl;
            }
        }

    void toggleRepair() {
        if (length > 0) {
            underRepair = !underRepair;
            cout << "Признак \"в ремонте\" изменён на: " << (underRepair ? "Да" : "Нет") << endl;
        }
        else {
            cout << "Трубы нет" << endl;
        }
    }

    void saveToFile(ofstream& file) {
        if (full_name.empty() && length <= 0 && diameter <= 0) {
            cout << "Трубы не существует. Невозможно сохранить данные по объекту." << endl;}
        else
        {
            file << full_name << endl;
            file << length << endl;
            file << diameter << endl;
            file << underRepair << endl;
            cout << "Данные трубы сохранены в pipe.txt." << endl;
        }
    }

    void loadFromFile(ifstream& file) {
        if (file.peek() == EOF) {
        cout << "Ошибка: Файл pipe.txt пуст." << endl;
        return;
    }
        if (file.is_open()) {
            if (!getline(file, full_name) || full_name.empty()) {
                cout << "Ошибка pipe.txt: Данные в файле заполнены некорректно (название)." << endl;
                return;
            }
            if (!(file >> length) || length <= 0) {
                cout << "Ошибка pipe.txt: Данные в файле заполнены некорректно (длина)." << endl;
                return;
            }
            if (!(file >> diameter) || diameter <= 0) {
                cout << "Ошибка pipe.txt: Данные в файле заполнены некорректно (диаметр)." << endl;
                return;
            }
            int repairStatus;
            if (!(file >> repairStatus) || (repairStatus != 0 && repairStatus != 1)) {
                cout << "Ошибка pipe.txt: Данные в файле заполнены некорректно (статус ремонта)." << endl;
                return;
            }
            else {
            cout << "Данные трубы загружены из pipe.txt." << endl;
            }
            
        } else {
            cout << "Ошибка: Не удалось открыть файл." << endl;
        }
    }
};

struct CompressorStation {
    string name;
    string name_line;
    string full_name;
    int totalShops = 0;
    int operationalShops = 0;
    double efficiency;

    void read() {
        cout << "Введите название КС: ";
        cin >> name;
        getline(cin, name_line);
        full_name = name + name_line;

        while (true) {
            cout << "Введите количество цехов: ";
            cin >> totalShops;
            if (cin.fail() || totalShops < 0 || totalShops == 0) { 
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                cout << "Ошибка! Количество цехов должно быть неотрицательным числом. Попробуйте снова." << endl;
            } else {
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
            } else if (operationalShops > totalShops) {
                cout << "Количество цехов в работе не должно превышать общее количество цехов. Попробуйте снова." << endl;
            } else {
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
            } else {
                break; 
            }
        }
    }

    void display() {
         if (full_name.empty() && totalShops <= 0 && operationalShops <= 0) {
                cout << "КС нет" << endl;
        } else {
            cout << "КС: " << full_name << ", Всего цехов: " << totalShops
                << ", Цехов в работе: " << operationalShops
                << ", Эффективность: " << efficiency << endl;
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
                    if (operationalShops < totalShops) {
                        operationalShops++;
                        cout << "Запустили новый цех. Теперь в работе: " << operationalShops << endl;
                    } else {
                        cout << "Все цехи уже запущены." << endl;
                    }
                    break;
                case 2: 
                    if (operationalShops > 0) {
                        operationalShops--;
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

    void saveToFile(ofstream& file) {
        if (full_name.empty() && totalShops <= 0 && operationalShops <= 0){
            cout << "Кс не существует. Невозможно сохранить данные по объекту." << endl;}
        else {
            file << full_name << endl;
            file << totalShops << endl;
            file << operationalShops << endl;
            file << efficiency << endl;
            cout << "Данные компрессорной станции сохранены в compressorstation.txt." << endl;
        }
        
    }

    void loadFromFile(ifstream& file) {
    if (file.peek() == EOF) {
        cout << "Ошибка: Файл compressorstation.txt пуст." << endl;
        return;
    }
    if (file.is_open()) {

        if (!getline(file, full_name) || full_name.empty()) {
            cout << "Ошибка compressorstation.txt: Данные в файле заполнены некорректно (имя)." << endl;
            return;
        }

        if (!(file >> totalShops) || totalShops < 0) {
            cout << "Ошибка compressorstation.txt: Данные в файле заполнены некорректно (количество магазинов)." << endl;
            return;
        }

        if (!(file >> operationalShops) || operationalShops < 0 || operationalShops > totalShops) {
            cout << "Ошибка compressorstation.txt: Данные в файле заполнены некорректно (количество работающих магазинов)." << endl;
            return;
        }

        if (!(file >> efficiency) || efficiency < 0.0) {
            cout << "Ошибка compressorstation.txt: Данные в файле заполнены некорректно (эффективность)." << endl;
            return;
        }
        else {
            cout << "Данные компрессорной станции загружены из compressorstation.txt." << endl;
        }

        
    } else {
        cout << "Ошибка: Не удалось открыть файл." << endl;
    }
}
};

void displayMenu() {
    cout << "-----------------------------" << endl;
    cout << "|            Меню:          |" << endl;
    cout << "| 1. Добавить трубу         |" << endl;
    cout << "| 2. Добавить КС            |" << endl;
    cout << "| 3. Просмотр всех объектов |" << endl;
    cout << "| 4. Редактировать трубу    |" << endl;
    cout << "| 5. Редактировать КС       |" << endl;
    cout << "| 6. Сохранить              |" << endl;
    cout << "| 7. Загрузить              |" << endl;
    cout << "| 0. Выход                  |" << endl;
    cout << "-----------------------------" << endl;
}
void savePipeDataToFile(Pipe & pipe) {
    ofstream outFile("pipe.txt");
    if (!outFile) {
        cout << "Ошибка открытия файла pipe.txt!" << endl;
        return;
    }
    pipe.saveToFile(outFile);
    outFile.close();
}

void saveCompressorStationDataToFile(CompressorStation & cs) {
    ofstream outFile("compressorstation.txt");
    if (!outFile) {
        cout << "Ошибка открытия файла compressorstation.txt!" << endl;
        return;
    }
    cs.saveToFile(outFile);
    outFile.close();
}

void loadPipeDataFromFile(Pipe & pipe) {
    ifstream inFile("pipe.txt");
    if (inFile.is_open()) {
        pipe.loadFromFile(inFile);
        inFile.close();
    } else {
        cout << "Ошибка открытия файла pipe.txt." << endl;
    }
}

void loadCompressorStationDataFromFile(CompressorStation & cs) {
    ifstream inFile("compressorstation.txt");
    if (inFile.is_open()) {
        cs.loadFromFile(inFile);
        inFile.close();
    } else {
        cout << "Ошибка открытия файла compressorstation.txt." << endl;
    }
}

int main() {
    Pipe pipe;
    CompressorStation cs;
    int choice;
    while (true) {
        displayMenu();
        cout << "Выберите действие: ";
        cin >> choice;
        cout << "-----------------------------" << endl;

        switch (choice) {
        case 1:
            pipe.read();
            break;
        case 2:
            cs.read();
            break;
        case 3:
            pipe.display();
            cs.display();
            break;
        case 4:
            pipe.toggleRepair();
            break;
        case 5:
            cs.toggleShop();
            break;
        case 6: {
            savePipeDataToFile(pipe);
            saveCompressorStationDataToFile(cs);
            break;
        }
        case 7: {
            loadPipeDataFromFile(pipe);
            loadCompressorStationDataFromFile(cs);
            break;
        }
        case 0:
            cout << "Выход из программы." << endl;
            return 0;
        default:
            cout << "Неверный выбор, попробуйте снова." << endl;
            break;
        }
    }
}