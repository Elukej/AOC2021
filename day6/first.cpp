#include <fstream>
#include "../include/luka_string.h"

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

    /* PEASANT SOLUTION. LOOK SECOND FOR BETTER */
    unsigned long long num_el = vec.size();
    for (int day = 0; day < 80; day++) {
        for (unsigned long long fish = 0; fish < num_el; fish++) 
            if (--vec[fish] < 0) {
                vec[fish] = 6;
                vec.push_back(8);
            }
        num_el = vec.size();
    }
    std::cout << num_el << std::endl;
}