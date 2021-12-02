#include <iostream>
#include <fstream>
#include <string>

int main()
{
    std::cout << "Program broji koliko puta u nizu je vrednost\
 skocila od prethodne, ali u trojkama." << std::endl;
    std::ifstream input;
    int count_inc = 0;
    int t[4] = {0}, sum1 = 0, sum2 = 0;
    input.open("input.txt");
    std::string line1;

    for(int i = 0; i<3; i++) {
        std::getline(input, line1);
        t[i] = std::stoi(line1);
    }

    while (std::getline(input, line1)) {
        t[3] = std::stoi(line1);
        if (t[3] > t[0]) count_inc++;
        for (int i = 0; i < 3; i++) t[i] = t[i+1];
    };
    std::cout << count_inc << std::endl;
}