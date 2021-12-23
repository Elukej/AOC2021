#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>

using Tupl = std::tuple<unsigned long, unsigned long>;

struct Dice {
    int move;
    int modifier;
    Dice(std::initializer_list<int> lista)
    {
        auto it = lista.begin();
        move = *it;  it++;
        modifier = *it; 
    }
};

const static std::vector<Dice> DIRAC = {{3,1}, {4,3}, {5,6}, {6,7},
                                        {7,6}, {8,3}, {9,1}}; // length is 7

static Tupl MEMO[88200] = {std::make_tuple(0ul,0ul)};

int fastMod10(int num)
{
    return num = (num > 10)? num % 10 : num;
}

int hasher(int player, int score1, int score2, int pos1, int pos2)
{
    return player * 44100 + (pos2 - 1) * 4410  + (pos1 - 1) * 441 + 
           score2 * 21 + score1;
}

bool isZeroTuple(Tupl tup)
{
    return (std::get<0>(tup) == 0) && (std::get<1>(tup) == 0); 
}

Tupl operator*(const unsigned long &num, const Tupl &tup)
{
    return std::make_tuple(num * std::get<0>(tup), num * std::get<1>(tup));
}

Tupl universeCounter(bool player, unsigned long uni, int score1,
                     int score2, int pos1, int pos2)
{
    unsigned long count1 = 0, count2 = 0;

    if (score1 >= 21) return std::make_tuple(uni, 0);
    if (score2 >= 21) return std::make_tuple(0, uni);
    if (!isZeroTuple(MEMO[hasher(player, score1, score2, pos1, pos2)])) 
        return (uni * MEMO[hasher(player, score1, score2, pos1, pos2)]);

    for (auto &mem : DIRAC) 
        if (!player) {
            auto tup = universeCounter(!player, mem.modifier * uni,
                                  score1 + fastMod10(pos1 + mem.move), 
                                  score2, fastMod10(pos1 + mem.move), pos2);
            count1 += std::get<0>(tup);
            count2 += std::get<1>(tup);
        }
        else { 
            auto tup = universeCounter(!player, mem.modifier * uni, 
                                  score1, score2 + fastMod10(pos2 + mem.move), 
                                  pos1, fastMod10(pos2 + mem.move));
            count1 += std::get<0>(tup);
            count2 += std::get<1>(tup);
        }
    MEMO[hasher(player, score1, score2, pos1, pos2)] = 
        std::make_tuple(count1 / uni, count2 / uni); 
        //since value is amplified by current uni, make it general by division
    return std::make_tuple(count1, count2);;
}


int main(int argc, char** argv)
{
    std::cout << "Dirac dice!" << std::endl;
    FILE *file;     file = fopen(argv[1], "r");
    if (file == NULL) {
        std::cout << "Error opening the file!" << std::endl;
        return EXIT_FAILURE;
    }
    int p1, p2;
    fscanf(file,"Player 1 starting position: %d\n", &p1);
    fscanf(file,"Player 2 starting position: %d\n", &p2);
    fclose(file);
    std::cout << p1 << " " << p2 << std::endl;

    Tupl victories = universeCounter(0, 1, 0, 0, p1, p2);
    std::cout << "(" << std::get<0>(victories) << ", "
    << std::get<1>(victories) << ")" << std::endl;

} 