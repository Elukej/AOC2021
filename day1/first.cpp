#include <iostream>
#include <fstream>
#include <string>
// Program se poziva sa imenom input fajla
int main(int argc, char** argv)
{
    std::cout << "Program broji koliko puta u nizu je vrednost\
 skocila od prethodne." << std::endl;
    std::ifstream input;
    int count_inc = 0;
    input.open(argv[1]);

    if(!input) {
        std::cout << "Neuspesno otvaranje fajla!" << std::endl;
        return EXIT_FAILURE;
    }
    std::string line1, line2;
    std::getline(input, line1);
    for ( ;std::getline(input, line2); ) {
        if (std::stoi(line2) > std::stoi(line1)) count_inc++;
        line1 = std::move(line2);
    }
    std::cout << count_inc << std::endl;
}