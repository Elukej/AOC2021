#include <fstream>
#include <forward_list>
#include <unordered_set>
#include "../include/luka_string.h"

struct Point {
    int x;
    int y;
    Point (int x, int y) : x(x), y(y){}
    Point& mirror(Point &fold)
    {
        if ( (x >= fold.x) && (y >= fold.y)) {
            x = abs(fold.x - (x - fold.x));
            y = abs(fold.y - (y - fold.y));
        }
        return *this;
    }
    friend bool operator==(const Point& p1, const Point& p2)
    {
        return p1.x == p2.x && p1.y == p2.y;
    }
};

class MyHashFunc {
    public:
        size_t operator()(const Point &p) const //ovaj zadnji const zapravo MORA da ide!!!!
        {
            return p.y * 1000 + p.x;
        }
};

void foldPaper(Point &fold, std::forward_list<Point> &paper,
               std::unordered_set<Point, MyHashFunc> &paper_uset)
{
    for ( auto it1 = paper.before_begin(), it2 = paper.begin();
         it2 != paper.end(); )
    {
        if ( (fold.x == (*it2).x && fold.x) || 
             (fold.y == (*it2).y && fold.y) ) 
        {
            paper_uset.erase(*it2);
            it2++;
            paper.erase_after(it1);
        }
        else {
            paper_uset.erase(*it2);
            if ( !paper_uset.insert((*it2).mirror(fold)).second ) {
                it2++;
                paper.erase_after(it1);
            }
            else {it1++; it2++;}
        }
    }
}

int main(int argc, char** argv)
{
    std::cout << "Origami!" << std::endl;
    std::ifstream input(argv[1]);
    if (!input) {
        std::cout << "Error opening the file!" << std::endl;
        return EXIT_FAILURE;
    }

    std::forward_list<Point> paper;
    std::unordered_set<Point, MyHashFunc> paper_uset;
    std::string line;
    while (std::getline(input, line) && (line != "")) {
        std::vector<int> temp = luka_string::num_parser(line);
        paper.push_front(Point(temp[0], temp[1]));
        paper_uset.insert(Point(temp[0],temp[1]));
    }

    std::vector<Point> fold;
    while (std::getline(input, line)) 
        if (line.find('x') != std::string::npos) 
            fold.push_back(Point(luka_string::num_parser(line).front(),0)); 
        else fold.push_back(Point(0, luka_string::num_parser(line).front()));

    foldPaper(fold[0], paper, paper_uset);
    std::cout << "Number of #: " << paper_uset.size() << std::endl;
}