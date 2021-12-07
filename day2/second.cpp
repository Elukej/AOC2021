#include <iostream>
#include <string>
#include <fstream>

struct Position {
    private:
        int x;
        int y;
        int aim;
    public:    
        Position(int x = 0, int y = 0, int aim = 0): x(x), y(y), aim(aim) {}
        int getX() { return this->x; }
        int getY() { return this->y; }
        int getAim() { return this->aim; }
        void addX(int num) { this->x += num; }
        void addY(int num) { this->y += num; }
        void addAim(int num) { this->aim += num; }
};

int main(int argc, char** argv)
{
    std::cout << "Program pokrece podmornicu po dve ose.\
 i izracunava proizvod krajnje pozicije po x i y" << std::endl;
    std::ifstream input(argv[1]);
    std::string dir, num;
    Position submarine;

    if(!input) {
        std::cout << "Neuspesno otvaranje fajla!" << std::endl;
        return EXIT_FAILURE;
    }
    while (!input.eof()) {
        std::getline(input, dir, ' '); // delimits by whitespace
        input >> std::ws; // eats whitespaces
        std::getline(input, num);
        if (dir == "forward") {
            int move = std::stoi(num);
            submarine.addX(move);
            submarine.addY(submarine.getAim() * move);
        }
        if (dir == "down") submarine.addAim(std::stoi(num));
        if (dir == "up") submarine.addAim( - std::stoi(num) );
    }
    std::cout << submarine.getX() * submarine.getY() << std::endl;
}