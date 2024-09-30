#include <iostream>
#include <string>

int main() {
    std::string name; 

    std::cout << "Введите название трубы: ";
    std::getline(std::cin, name); 
    std::cout << "Первое слово: " << name << std::endl; 

    return 0;
}