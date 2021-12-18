#include <fstream>
#include <iostream>
#include <string>
#include <vector>

const char* DIGITS = "-0123456789";

int main(int argc, char** argv)
{
    std::ifstream input(argv[1]);
    std::cout << "Probe launcher!" << std::endl;
    if (!input) {
        std::cout << "Error opening the file!" << std::endl;
        return EXIT_FAILURE;
    } 
    std::string line; std::getline(input, line);
    std::vector<int> coords;
    int pos;
    while ( (pos = line.find_first_of(DIGITS, 0)) != std::string::npos) {
        line.erase(0, pos);
        coords.push_back(std::stoi(line));
        pos = line.find_first_not_of(DIGITS, 0);
        line.erase(0,pos);
    }
    int max_y_vel = (abs(coords[2]) > abs(coords[3]))? 
                        abs(coords[2]) - 1 : abs(coords[3]) - 1;
    std::cout << "Max height: " << max_y_vel * (max_y_vel + 1) / 2 << std::endl; 
}