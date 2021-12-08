#include <fstream>
#include <algorithm>
#include <numeric>
#include "../include/luka_string.h"

int sum_first_n( int n)
{
    return n*(n+1)/2;
}

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
    double avg = std::accumulate(vec.begin(), vec.end(), 0.0) / vec.size();
    std::cout << "Average: " << avg << std::endl;
   
    int fuel1 = 0, fuel2 = 0;
    for (auto mem : vec) {
        fuel1 += sum_first_n(abs(mem - (int) avg));
        fuel2 += sum_first_n(abs(mem - (int)(avg + 1))); 
        // neccessary to check both integers around average for minimum.
    }
    std::cout << "Min fuel spent by crabs: "\
    << std::min(fuel1, fuel2) <<std::endl;
}