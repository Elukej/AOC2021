#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main(int argc, char** argv)
{
    std::cout << "Program trazi proivod brojeva dobijenih\
 od najcescih cifara i najredjih cifara iz liste" << std::endl;
    std::ifstream input(argv[1]);
    std::string line;
    std::vector<int> vec;
    int total = 0;
    int mcom = 0, lcom = 0;
 
    // doing the first line so vector can be properly allocated
    std::getline(input, line);
    for (std::string::iterator it = line.begin(); it != line.end(); it++)
        vec.push_back((*it) == '1');
    total++;

    // every other line with proper vector length
    for ( ; std::getline(input, line) ; ) {
        for (int i = 0; i < line.length(); i++)
            vec[i] += (line[i] == '1');
        total++;
    }

    // create numbers from vector
    for (int i = 0; i < vec.size(); i++)
        if (total - vec[i] < vec[i]) {
            mcom = 2*mcom + 1;
            lcom *= 2;
        }
        else {
            lcom = 2*lcom + 1;
            mcom *= 2;           
        }       
    std::cout << mcom * lcom << std::endl;
}