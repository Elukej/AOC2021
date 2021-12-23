#include <iostream>
#include <fstream>
#include <string>

int fastMod10(int num)
{
    return num = (num > 10)? num % 10 : num;
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
    int score1 = (fastMod10(p1 + 6)); // 1+2+3 = 6
    int score2 = (fastMod10(p2 + 5)); // (4+5+6)%10 = 5
    int num_moves = 2;
    int pos1 = score1, pos2 = score2;
    p1 = 6; p2 = 5;
    while (score1 < 1000 && score2 < 1000) {
        p1 = fastMod10(p1 + 8); p2 = fastMod10 (p2 + 8);
        pos1 = (fastMod10(pos1 + p1) == 0)? 10 : fastMod10(pos1 + p1);  
        pos2 = (fastMod10(pos2 + p2) == 0)? 10 : fastMod10(pos2 + p2);
        score1 += pos1;  num_moves++;   
        if (score1 >= 1000) continue;
        score2 += pos2;  num_moves++;
    }
    num_moves *= 3;
    int min_score = (score1 < score2)? score1 : score2;
    std::cout << score1 << " " << score2 << " " << num_moves << std::endl;
    std::cout << "Product moves * min_score: "
    << min_score * num_moves << std::endl;
} 