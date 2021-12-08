#include <fstream>
#include <algorithm>
#include "../include/luka_string.h"

int main(int argc, char** argv)
{
    std::cout << "Crab submarines!" << std::endl;
    std::ifstream input(argv[1]);
    if(!input) {
        std::cout << "Error opening file!" << std::endl;
        return EXIT_FAILURE;
    }
    std::string line;
    std::getline(input, line);
    std::vector<int> vec = luka_string::num_parser(line);
    std::sort(vec.begin(), vec.end()); 

    int median = vec[vec.size() / 2];
    std::cout << "Median is: " << median << std::endl;
    int fuel = 0;
    for (auto mem : vec) fuel += (abs(mem - median));
    std::cout << "Min fuel spent by crabs: " << fuel << std::endl;
}