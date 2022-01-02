#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int moveEast(std::vector<std::string> &sea_bottom)
{
    int changes = 0;
    for (int i = 0; i < sea_bottom.size(); i++) {
        char temp1 = sea_bottom[i][0], 
             temp2 = sea_bottom[i][sea_bottom[i].size() - 1];
        for (int j = 0; j < sea_bottom[i].size() - 1; j++)
            if (sea_bottom[i][j] == '>' && sea_bottom[i][j+1] == '.') {
                    sea_bottom[i][j+1] = '>';
                    sea_bottom[i][j++] = '.';
                    changes++;
                }
        if (temp2 == '>' && temp1 == '.') {
            sea_bottom[i][0] = '>';
            sea_bottom[i][sea_bottom[i].size() - 1] = '.';
            changes++;
        }
    }
    return changes;
}

int moveSouth(std::vector<std::string> &sea_bottom)
{
    int changes = 0;
    for (int i = 0; i < sea_bottom[0].size(); i++) {
        char temp1 = sea_bottom[0][i],
             temp2 = sea_bottom[sea_bottom.size() - 1][i];
        for (int j = 0; j < sea_bottom.size() - 1; j++)
            if (sea_bottom[j][i] == 'v' && sea_bottom[j+1][i] == '.') {
                sea_bottom[j+1][i] = 'v';
                sea_bottom[j++][i] = '.';
                changes++;
            }
        if (temp2 == 'v' && temp1 == '.') {
            sea_bottom[0][i] = 'v';
            sea_bottom[sea_bottom.size() - 1][i] = '.';
            changes++;
        }
    }
    return changes;
}

int main(int argc, char **argv)
{
    std::ifstream input(argv[1]);
    if(!input) {
        std::cout << "Error opening the file!" << std::endl;
        return EXIT_FAILURE;
    }

    std::string line;
    std::vector<std::string> sea_bottom;
    while(std::getline(input,line)) 
        sea_bottom.push_back(line);
    
    int steps = 0;
    while (true) {
        steps++;
        if ((moveEast(sea_bottom) + moveSouth(sea_bottom)) == 0)
            break;
    }

    std::cout << "Broj koraka: " << steps << std::endl;
} 