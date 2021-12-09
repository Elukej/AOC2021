#include <iostream>
#include <fstream>
#include <vector>
#include <string>

std::vector<int> row_padder(int num, int length)
{
    return std::vector<int>(length, num);   
}

bool isLower(int row, int col, std::vector<std::vector<int>> &vec)
{
    return vec[row][col] < vec[row][col - 1] && 
           vec[row][col] < vec[row - 1][col] &&
           vec[row][col] < vec[row + 1][col] &&
           vec[row][col] < vec[row][col + 1];
}

int main(int argc, char** argv)
{
    std::cout << "Lava caves!" << std::endl;
    std::ifstream input(argv[1]);
    if (!input) {
        std::cout << "Error opening the file!" << std::endl;
        return EXIT_FAILURE;
    }
    // parsing with padding
    std::vector<std::vector<int>> lava_field;
    std::string line;
    std::getline(input, line);
    lava_field.push_back(row_padder(9, line.length() + 2)); // row with 9 on top
    do {
        lava_field.push_back(row_padder(9, lava_field[0].size()));
        for (int i = 0; i < line.length(); i++)
            lava_field[lava_field.size() - 1][i+1] = line[i] - 48; // char digit to int;
    } while (std::getline(input, line));     
    lava_field.push_back(row_padder(9, lava_field[0].size())); // row with 9 bottom

    int risk_lvl = 0;
    for (int i = 1; i < lava_field.size() - 1; i++)
        for (int j = 1; j < lava_field[i].size() - 1; j++) {
            if (lava_field[i][j] == 9) continue;
            if (isLower(i, j, lava_field)) 
                risk_lvl += lava_field[i][j] + 1;
        }
    std::cout << "Risk level is: " << risk_lvl << std::endl;
}