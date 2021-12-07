#include <iostream>
#include <string>
#include <fstream>

struct Position {
    private:
        int x;
        int y;
    public:    
        Position(int x = 0, int y = 0): x(x), y(y) {}
        int getX() { return this->x; }
        int getY() { return this->y; }
        void addX(int num) { this->x += num; }
        void addY(int num) { this->y += num; }
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
        if (dir == "forward") submarine.addX(std::stoi(num));
        if (dir == "down") submarine.addY(std::stoi(num));
        if (dir == "up") submarine.addY( - std::stoi(num) );
    }
    std::cout << submarine.getX() * submarine.getY() << std::endl;
}