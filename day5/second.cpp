#include <iostream>
#include <fstream>
#include <unordered_map>
#include "../include/luka_string.h"


struct Point {
    int x;
    int y;
    Point(){}
    Point(int x, int y):x(x),y(y){}
    friend bool operator==(const Point &p1, const Point &p2)
    {
        return (p1.x == p2.x && p1.y == p2.y);
    } // kad se ovaj operator neophodan za koriscenje ove strukture kao kljuca hash mape
    // ne deklarise kao prijatelj, kompajler javlja gresku
};

struct DangerLine {
    Point t1;
    Point t2;
    DangerLine(){}
    DangerLine(std::vector<int> vec) {
        t1.x = vec[0];
        t1.y = vec[1];
        t2.x = vec[2];
        t2.y = vec[3];
    }
    void print() 
    {
        std::cout << "t1.x: " << t1.x << ", t1.y: " << t1.y <<\
        ", t2.x: " << t2.x << ", t2.y: " << t2.y << std::endl;  
    } 
};

struct HashFunctionType {
    size_t operator()(const Point &p) const // ovaj const je dozvoljen samo na funkcijama clanovima klasa
    // i odnosi se na this pokazivac, tj da je stanje objekta nepromenjivo ovom funkcijom
    {
        return p.x * 1000 + p.y; 
    }
};

int main(int argc, char** argv)
{
    std::ifstream input(argv[1]);
    std::string line;
    std::vector<DangerLine> vec; 
    
    if(!input) {
        std::cout << "Neuspesno otvaranje fajla!" << std::endl;
        return EXIT_FAILURE;
    }
    
    for ( ; std::getline(input, line); )
        vec.push_back(DangerLine(num_parser(line)));


    std::unordered_map<Point, int, HashFunctionType> ground_map;
    for (auto mem : vec) {
        if (mem.t1.x == mem.t2.x)
            for (auto i = std::min(mem.t1.y, mem.t2.y);
                i <= std::max(mem.t1.y, mem.t2.y); i++)
                if (ground_map.find(Point(mem.t1.x, i)) != ground_map.end())
                    ground_map[Point(mem.t1.x, i)]++;
                else ground_map[Point(mem.t1.x, i)] = 1;
        else if (mem.t1.y == mem.t2.y)
            for (auto i = std::min(mem.t1.x, mem.t2.x);
                i <= std::max(mem.t1.x, mem.t2.x); i++)
                if (ground_map.find(Point(i, mem.t1.y)) != ground_map.end())
                    ground_map[Point(i, mem.t1.y)]++;
                else ground_map[Point(i, mem.t1.y)] = 1;
        else { // diagonal case: inc is (1,1) or (1, -1). incX is 1 always.
            int y, incY = (mem.t1.x - mem.t2.x == mem.t1.y - mem.t2.y)
                      -(mem.t1.x - mem.t2.x != mem.t1.y - mem.t2.y);
            if (mem.t1.x == std::min(mem.t1.x, mem.t2.x)) y = mem.t1.y;
            else y = mem.t2.y;
            for (auto x = std::min(mem.t1.x, mem.t2.x);
                x <= std::max(mem.t1.x, mem.t2.x); x++) {
                if (ground_map.find(Point(x, y)) != ground_map.end())
                    ground_map[Point(x, y)]++;
                else ground_map[Point(x, y)] = 1;
                y += incY;        
            }   
        }
    }
    int count = 0;
    for (auto mem : ground_map) count += (mem.second > 1);
    std::cout << count << std::endl;
}