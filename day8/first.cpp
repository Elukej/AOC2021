#include <fstream>
#include "../include/luka_string.h"

bool elem(int num, std::initializer_list<int> l)
{
    for (auto it = l.begin(); it != l.end(); it++) 
        if (*it == num) return true;
    return false;
}

int main(int argc, char** argv)
{
    std::cout << "LED digit recognition:" << std::endl;
    std::ifstream input(argv[1]);
    if(!input) {
        std::cout << "Error opening file!" << std::endl;
        return EXIT_FAILURE;
    }

    std::string line;
    int count_digit = 0;
    while (std::getline(input, line)) {
        std::vector<std::string> vec =
        luka_string::split(luka_string::split(line, " | ").back(), " ");
        for (auto digit : vec)
            if (elem(digit.length(), {2,3,4,7})) count_digit++; 
    }
    std::cout << "Number of digits is: " << count_digit << std::endl;
}