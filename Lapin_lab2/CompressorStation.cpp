#include "CompressorStation.h"
#include "utils.h"

using namespace std;

void CompressorStation::read() {
    cout << "Введите название КС: ";
    getline(cin, full_name);
    log("Введено название КС: " + full_name); 
    totalShops = readPositive<int>("Введите количество цехов: ", "Ошибка! Количество цехов должно быть неотрицательным числом. Попробуйте снова.");
    log("Пользователь ввел общее количество цехов: " + to_string(totalShops));
    operationalShops = readPositive<int>("Введите количество цехов в работе: ", "Ошибка! Количество цехов в работе должно быть неотрицательным числом и не привышать общее количество цехов. Попробуйте снова.",totalShops+1);
    log("Пользователь ввел количество рабочих цехов: " + to_string(totalShops));
    efficiency = readPositive<double>("Введите эффективность в % (1-100): ","Ошибка! Эффективность должна быть неотрицательным числом и меньше 100. Попробуйте снова.",101 );
    log("Пользователь ввел процент эффективности: " + to_string(totalShops) + "%");
    unusedOperationalShops = ((totalShops - operationalShops) / static_cast<double>(totalShops)) * 100;
}

void CompressorStation::display() const {
    if (full_name.empty() && totalShops <= 0 && operationalShops <= 0) {
        cout << "КС нет" << endl;
    } else {
        cout << " : КС: " << full_name 
             << ", Всего цехов: " << totalShops
             << ", Цехов в работе: " << operationalShops
             << setprecision(2) << fixed 
             << ", Процент незадействованных цехов: " << unusedOperationalShops << "%"
             << ", Эффективность: " << efficiency << "%" << endl;
    }
}

void CompressorStation::load(const string &data) {
    size_t pos1 = data.find(';');  
    size_t pos2 = data.find(';', pos1 + 1);  
    size_t pos3 = data.find(';', pos2 + 1);  
    size_t pos4 = data.find(';', pos3 + 1);  

    full_name = data.substr(0, pos1);  
    totalShops = stoi(data.substr(pos1 + 1, pos2 - pos1 - 1));  
    operationalShops = stoi(data.substr(pos2 + 1, pos3 - pos2 - 1));  
    unusedOperationalShops = stoi(data.substr(pos3 + 1, pos4 - pos3 - 1));
    efficiency = stod(data.substr(pos4 + 1, data.length() - pos4 - 1));
}