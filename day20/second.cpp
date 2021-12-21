#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::string rowPadder(size_t n)
{
    return std::string(n,'.');
}

std::vector<std::string> parser(std::ifstream &input)
{
    std::vector<std::string> trench;
    std::string line;
    std::getline(input, line);
    trench.push_back(rowPadder(line.size() + 4));
    trench.push_back(rowPadder(line.size() + 4));
    trench.push_back(".." + line + "..");
    while(!input.eof()) {
        char c; line = "..";
        input.get(c);
        while (c != '\n' && !input.eof() ) {
            line += c;
            input.get(c);
        }
        line += "..";
        trench.push_back(line);
    }   
    trench.push_back(rowPadder(line.size()));
    trench.push_back(rowPadder(line.size()));
    return trench;
}

int getRefIndex(int row, int col, std::vector<std::string> &old_trench)
{
    int num = 0;
    for (int i = row - 1; i <= row + 1; i++)
        for (int j = col - 1; j <= col + 1; j++) {
            num = num << 1;
            num += (old_trench[i][j] & 1); // # is odd in ascii, and . is even;
        }
    return num;
}

std::vector<std::string> applyTransf(std::vector<std::string> &old_trench,
                                     std::string &ref_str, char ch)
{
    std::vector<std::string> new_trench(old_trench.size() + 2, 
                             std::string(old_trench[0].size() + 2, ch));
    for (int i = 1; i < old_trench.size() - 1; i++) 
        for (int j = 1; j < old_trench[i].size() - 1; j++) 
            new_trench[i+1][j+1] = ref_str[getRefIndex(i, j, old_trench)];
    return new_trench;
}

int countHashTags(std::vector<std::string> trench)
{
    int count = 0;
    for (int i = 2; i < trench.size() - 2; i++)
        for (int j = 2; j < trench[i].size() - 2; j++)
            count = (trench[i][j] == '#')? count + 1 : count;
    return count;
}

int main(int argc, char** argv)
{
    std::cout << "Trench Map!" << std::endl;
    std::ifstream input(argv[1]);
    if (!input) {
        std::cout << "Error opening the file!" << std::endl;
        return EXIT_FAILURE;
    }

    std::string ref_str, line;
    std::getline(input, ref_str);
    input.ignore(1,'\n');

    std::vector<std::string> trench = parser(input);

    char ch = '.';
    for (int round = 0; round < 50; round++) {
        ch = (ch == '.')? '#' : '.';
        trench = std::move(applyTransf(trench, ref_str, ch));
    }

    std::cout << "Nuber of #: " << countHashTags(trench) << std::endl;
}