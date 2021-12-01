#include <iostream>
#include <fstream>
#include <string>

int main()
{
    std::cout << "Program broji koliko puta u nizu je vrednost\
 skocila od prethodne." << std::endl;
    std::ifstream input;
    int count_inc = 0;
    input.open("input.txt");
    std::string line1, line2;
    std::getline(input, line1);
    for ( ;std::getline(input, line2); ) {
        if (std::stoi(line2) > std::stoi(line1)) count_inc++;
        line1 = std::move(line2);
    }
    std::cout << count_inc << std::endl;
}