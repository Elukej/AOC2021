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

    for(int i = 0; i<4; i++) {
        std::getline(input, line1);
        t[i] = std::stoi(line1);
    }
    sum1 = t[0] + t[1] + t[2];
    sum2 = t[1] + t[2] + t[3];
    if (sum2 > sum1) count_inc++;

    while (std::getline(input, line1)) {
        sum1 = sum2;
        for (int i = 0; i < 3; i++) t[i] = t[i+1];
        t[3] = std::stoi(line1);
        sum2 += (t[3] - t[0]);
        if (sum2 > sum1) count_inc++;
    };
    std::cout << count_inc << std::endl;
}