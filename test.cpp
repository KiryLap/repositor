#include <iostream>
#include <string>

int main() {
    std::string name; 
    std::string name_line; 

    std::cout << "Введите название трубы: ";
    std::cin >> name;

    std::getline(std::cin, name_line); 

    std::cout << "Первое слово: " << name << std::endl; 
    std::cout << "Полное название трубы: " << name + name_line << std::endl; 

    return 0;
}