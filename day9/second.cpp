#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

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

int basin_size(int row, int col, std::vector<std::vector<int>> &vec,
std::vector<std::vector<int>> &tracker)
{
    int suma = 0;
    tracker[row][col] = 1;
    if((vec[row][col - 1] != 9) && !tracker[row][col - 1])
         suma += basin_size(row, col - 1, vec, tracker);
    if((vec[row - 1][col] != 9) && !tracker[row - 1][col])
         suma += basin_size(row - 1, col, vec, tracker);
    if((vec[row + 1][col] != 9) && !tracker[row + 1][col])
        suma += basin_size(row + 1, col, vec, tracker);
    if((vec[row][col + 1] != 9) && !tracker[row][col + 1])
         suma += basin_size(row, col + 1, vec, tracker);
    suma++;
    return suma;
}

void insert_basin(int size, int arr[]) 
{
    if (size > arr[0]) {
        arr[0] = size;
        std::sort(arr, arr + 3);
    }
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
            lava_field[lava_field.size() - 1][i+1] = line[i] - 48; // char digit to int
    } while (std::getline(input, line));     
    lava_field.push_back(row_padder(9, lava_field[0].size())); // row with 9 bottom

    std::vector<std::vector<int>> tracker(lava_field.size(), std::vector<int>(lava_field[0].size(), 0)); 
    int basins[3] = {0};
    for (int i = 1; i < lava_field.size() - 1; i++)
        for (int j = 1; j < lava_field[i].size() - 1; j++) {
            if (lava_field[i][j] == 9 && tracker[i][j]) continue; // == is primary operator so it has priority over &&
            if (isLower(i, j, lava_field))
                insert_basin(basin_size(i, j, lava_field, tracker), basins);
        }
    std::cout << "Basin product is: " << 
    basins[0] * basins[1] * basins[2] << std::endl;
}