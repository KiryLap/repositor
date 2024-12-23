#include "Pipe.h"
#include "utils.h"

using namespace std;

void Pipe::read() {
    cout << "Введите название трубы: ";
    getline(cin, full_name);
    log("Пользователь ввел название трубы: " + full_name);

    length = readPositive<double>("Введите длину трубы (в км): ", "Введён недопустимый параметр длинны.");
    log("Пользователь ввел длину трубы: " + to_string(length) + " км");

    diameter = readPositive<double>("Введите диаметр трубы (в мм): ", "Введён недопустимый диаметр.");
    log("Пользователь ввел диаметр трубы: " + to_string(diameter) + " мм");
    underRepair = false;
}

void Pipe::display() const {
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

void Pipe::load(const string &data) {
    size_t pos1 = data.find(';'); 
    size_t pos2 = data.find(';', pos1 + 1);
    size_t pos3 = data.find(';', pos2 + 1); 

    full_name = data.substr(0, pos1); 
    length = stod(data.substr(pos1 + 1, pos2 - pos1 - 1));
    diameter = stod(data.substr(pos2 + 1, pos3 - pos2 - 1)); 
    underRepair = data.substr(pos3 + 1) == "1"; 
}