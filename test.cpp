#include <iostream>
#include <vector>

int main() {
    std::vector<int> codes = {70, 105, 122, 105, 107, 97, 32, 120, 121, 105, 110, 57};

    for (int code : codes) {
        std::cout << static_cast<char>(code);
    }

    std::cout << std::endl;
    return 0;
}