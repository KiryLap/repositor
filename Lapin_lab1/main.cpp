#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>

using namespace std;

struct Pipe {
    string name;
    double length = 0;
    double diameter = 0;
    bool underRepair;

    void read() {
        cout << "Введите название трубы: ";
        cin >> name;

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
            cout << "Введите диаметр трубы (в см): ";
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
            } else {
                cout << "Труба: " << name 
                    << ", Длина: " << length 
                    << " км, Диаметр: " << diameter 
                    << " см, В ремонте: " 
                    << (underRepair ? "Да" : "Нет") 
                    << endl;
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
        file << name << endl;
        file << length << endl;
        file << diameter << endl;
        file << underRepair << endl;
    }

    void loadFromFile(ifstream &file) {
        file >> name;
        file >> length;
        file >> diameter;
        file >> underRepair;
    }
};

struct CompressorStation {
    string name;
    int totalShops = 0;
    int operationalShops = 0;
    double efficiency;

    void read() {
        cout << "Введите название КС: ";
        cin >> name;

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
            cout << "Введите эффективность: ";
            cin >> efficiency;
            if (cin.fail() || efficiency < 0 || efficiency == 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ошибка! Эффективность должна быть неотрицательным числом. Попробуйте снова." << endl;
            } else {
                break; 
            }
        }
    }

    void display() {
         if (name.empty() && totalShops <= 0 && operationalShops <= 0) {
                cout << "КС нет" << endl;
        } else {
            cout << "КС: " << name << ", Всего цехов: " << totalShops
                << ", Цехов в работе: " << operationalShops
                << ", Эффективность: " << efficiency << endl;
                 }
        } 

    void toggleShop() {
        int choice;
        cout << "1. Запустить новый цех" << endl;
        cout << "2. Остановить работу цеха" << endl;
        cout << "Выберите действие: ";
        cin >> choice;
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

    void saveToFile(ofstream& file) {
        file << name << endl;
        file << totalShops << endl;
        file << operationalShops << endl;
        file << efficiency << endl;
    }

    void loadFromFile(ifstream& file) {
        file >> name;
        file >> totalShops;
        file >> operationalShops;
        file >> efficiency;
    }
};

void displayMenu() {
    cout << "Меню:" << endl;
    cout << "1. Добавить трубу" << endl;
    cout << "2. Добавить КС" << endl;
    cout << "3. Просмотр всех объектов" << endl;
    cout << "4. Редактировать трубу" << endl;
    cout << "5. Редактировать КС" << endl;
    cout << "6. Сохранить" << endl;
    cout << "7. Загрузить" << endl;
    cout << "0. Выход" << endl;
}
void saveDataToFile(Pipe & pipe, CompressorStation & cs) {
    ofstream outFile("data.txt");
    if (!outFile) {
        cout << "Ошибка открытия файла!" << endl;
        return;
    }
    pipe.saveToFile(outFile);
    cs.saveToFile(outFile);
    outFile.close();
    cout << "Данные сохранены." << endl;
}

void loadDataFromFile(Pipe & pipe, CompressorStation & cs) {
    ifstream inFile("data.txt");
    if (inFile.is_open()) {
        pipe.loadFromFile(inFile);
        cs.loadFromFile(inFile);
        inFile.close();
        cout << "Данные загружены." << endl;
    } else {
        cout << "Ошибка открытия файла." << endl;
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
            saveDataToFile(pipe, cs);
            break;
        }
        case 7: {
            loadDataFromFile(pipe, cs);
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