#include <fstream>
#include "../include/luka_string.h"

struct Point {
    int x;
    int y;
    Point(int x, int y): x(x), y(y){}
    Point(std::initializer_list<int> lista)
    {
        auto it = lista.begin();
        x = *(it++);
        y = *it; it++;
        if (it != lista.end())
            std::cout << "Warning: Rest of the " <<
            "initializer list is garbage" << std::endl;
    }
};

static const std::vector<Point> DIRECTIONS = {{-1,-1},{0,-1},{1,-1},
{-1,0},{1,0},{-1,1},{0,1},{1,1}};

bool inBounds(int row, int col, int max_row, int max_col)
{
    return row >= 0 &&
           row < max_row &&
           col >= 0 &&
           col < max_col;
}

void octopusInc(std::vector<std::vector<int>> &octopus)
{
    for (auto &row : octopus)
        for(auto &single : row)
            single++;
}

int octopusFlash(int row, int col, std::vector<std::vector<int>> &octopus)
{   
    int flashes = 0;
    octopus[row][col] = 0;
    for (auto dir : DIRECTIONS)
        if (  inBounds(row + dir.y, col + dir.x, 
              octopus.size(), octopus[0].size()) &&
              (octopus[row + dir.y][col + dir.x] != 0)  ) 
            if ( ++octopus[row + dir.y][col + dir.x] > 9 )
                flashes += octopusFlash(row + dir.y, col + dir.x, octopus);
    flashes++;
    return flashes;
}

int checkFlash(std::vector<std::vector<int>> &octopus)
{
    int total_flashes_ina_day = 0;
    for (int i = 0 ; i < octopus.size(); i++)
        for (int j = 0; j < octopus[i].size(); j++) 
            if (octopus[i][j] == 10) 
               total_flashes_ina_day += octopusFlash(i, j, octopus);
    return total_flashes_ina_day;
}

bool octoSync(const std::vector<std::vector<int>> &octopus)
{
    for (auto &row : octopus)
        for (auto &single : row)
            if (single != 0) return false;
    return true;
}

int main(int argc, char** argv)
{
    std::cout << "Shining octopus!" << std::endl;
    std::ifstream input(argv[1]);
    if (!input) {
        std::cout << "Error opening the file!" << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<std::vector<int>> octopus;
    std::string line;
    while (std::getline(input, line)) {
        std::vector<int> temp;
        for (auto ch : line) temp.push_back(ch - 48);
        octopus.push_back(temp);
    }

    int day = 0;
    while (!octoSync(octopus)) {
        octopusInc(octopus);
        checkFlash(octopus);
        day++;
    }
    std::cout << "First day of synch: " << day << std::endl;
}