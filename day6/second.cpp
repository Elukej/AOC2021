#include <fstream>
#include <algorithm>
#include "../include/luka_string.h"

void shift_left(std::vector<unsigned long long> &vec, int pos)
{
    for (int i = 0; i < vec.size() - pos ; i++) vec[i] = vec[i + pos];
}

int main(int argc, char** argv)
{
    std::cout << "Lantern fish counting" << std::endl;
    std::ifstream input(argv[1]);
    if(!input) {
        std::cout << "Neuspesno otvaranje fajla!" << std::endl;
        return EXIT_FAILURE;
    }
    std::string line;
    std::getline(input, line);
    std::vector<int> vec = luka_string::num_parser(line);
    std::vector<unsigned long long> mapper(9,0);

    /* KING SOLUTION */
    for (auto i : vec) mapper[i]++; // fill the map with number of different elements from 0 to 8

    for (int day = 0; day < 256; day++) {
        unsigned long long temp = mapper[0];
        shift_left(mapper, 1);
        mapper[8] = temp;
        mapper[6] += temp;
    }
    
    unsigned long long sum = 0;
    for (auto mem : mapper) sum += mem;

    std::cout << sum << std::endl;
}