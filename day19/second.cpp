#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <forward_list>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

struct Point3d {
    int x;
    int y;
    int z;
    Point3d(int x = 0, int y = 0, int z = 0): x(x),y(y),z(z) {}
    Point3d operator+(const Point3d& p) 
    {
        return Point3d(x + p.x, y + p.y, z + p.z);
    }
    Point3d operator-(const Point3d& p)
    {
        return Point3d(x - p.x, y - p.y, z - p.z);
    }
    bool operator==(const Point3d& p) const
    {
        return x == p.x && y == p.y && z == p.z;
    }
    bool operator!=(const Point3d& p) const
    {
        return x != p.x || y != p.y || z != p.z;
    }
    bool operator<(const Point3d& p) const // again, const qualifier needed for the set to be able to use this
    {
        return (x < p.x) 
            || (x == p.x && y < p.y)
            || (x == p.x && y == p.y && z < p.z);
    }
};

struct Scanner {
    Point3d position;
    std::vector<Point3d> beacons;
    Scanner() {}
    Scanner(Point3d p): position(p) {}
};

struct MyHashFunc {
    size_t operator()(const Point3d &p) const
    {
        return p.x + p.y + p.z;
    }
};

void parseScanner(std::ifstream& input,
                  std::forward_list<Scanner> &scanners_list)
{
    Scanner scanner;    std::string line;   Point3d temp;
    input.ignore(100,'\n'); // discard line with scanner name
    while (std::getline(input, line) && (line != "")) {
        sscanf(line.c_str(),"%d,%d,%d", &temp.x, &temp.y, &temp.z);
        scanner.beacons.push_back(temp);
    }
    if(!input.eof()) parseScanner(input, scanners_list);
    scanners_list.push_front(scanner);
}

Scanner& createAbsScanner(Scanner &scanner)
{
    for (auto &beacon : scanner.beacons)
        beacon = beacon + scanner.position;
    return scanner;
}

Scanner ChOrientOfCube(Scanner &scanner, int transf_num)
{
    Scanner temp;   
    temp.beacons.reserve(scanner.beacons.size()); 
    switch(transf_num) {
        case 0: {
            for (auto &p : scanner.beacons)
                temp.beacons.emplace_back(p);
            return temp;
        }
        case 1: {
            for (auto &p : scanner.beacons)
                temp.beacons.emplace_back(Point3d(p.x, -p.z, p.y));
            return temp;
        }
        case 2: {
            for (auto &p : scanner.beacons)
                temp.beacons.emplace_back(Point3d(p.x, -p.y, -p.z));
            return temp;
        }
        case 3: {
            for (auto &p : scanner.beacons)
                temp.beacons.emplace_back(Point3d(p.x, p.z, -p.y));
            return temp;
        }
        case 4: {
            for (auto &p : scanner.beacons)
                temp.beacons.emplace_back(Point3d(-p.x, -p.y, p.z));
            return temp;
        }
        case 5: {
            for (auto &p : scanner.beacons)
                temp.beacons.emplace_back(Point3d(-p.x, -p.z, -p.y));
            return temp;
        }
        case 6: {
            for (auto &p : scanner.beacons)
                temp.beacons.emplace_back(Point3d(-p.x, p.y, -p.z));
            return temp;
        }
        case 7: {
            for (auto &p : scanner.beacons)
                temp.beacons.emplace_back(Point3d(-p.x, p.z, p.y));
            return temp;
        }
        case 8: {
            for (auto &p : scanner.beacons)
                temp.beacons.emplace_back(Point3d(-p.y, p.x, p.z));
            return temp;
        }
        case 9: {
            for (auto &p : scanner.beacons)
                temp.beacons.emplace_back(Point3d(p.z, p.x, p.y));
            return temp;
        }
        case 10: {
            for (auto &p : scanner.beacons)
                temp.beacons.emplace_back(Point3d(p.y, p.x, -p.z));
            return temp;
        }
        case 11: {
            for (auto &p : scanner.beacons)
                temp.beacons.emplace_back(Point3d(-p.z, p.x, -p.y));
            return temp;
        }
        case 12: {
            for (auto &p : scanner.beacons)
                temp.beacons.emplace_back(Point3d(p.y, -p.x, p.z));
            return temp;
        }
        case 13: {
            for (auto &p : scanner.beacons)
                temp.beacons.emplace_back(Point3d(p.z, -p.x, -p.y));
            return temp;
        }
        case 14: {
            for (auto &p : scanner.beacons)
                temp.beacons.emplace_back(Point3d(-p.y, -p.x, -p.z));
            return temp;
        }
        case 15: {
            for (auto &p : scanner.beacons)
                temp.beacons.emplace_back(Point3d(-p.z, -p.x, p.y));
            return temp;
        }
        case 16: {
            for (auto &p : scanner.beacons)
                temp.beacons.emplace_back(Point3d(-p.z, p.y, p.x));
            return temp;
        }
        case 17: {
            for (auto &p : scanner.beacons)
                temp.beacons.emplace_back(Point3d(-p.y, -p.z, p.x));
            return temp;
        }
        case 18: {
            for (auto &p : scanner.beacons)
                temp.beacons.emplace_back(Point3d(p.z, -p.y, p.x));
            return temp;
        }
        case 19: {
            for (auto &p : scanner.beacons)
                temp.beacons.emplace_back(Point3d(p.y, p.z, p.x));
            return temp;
        }
        case 20: {
            for (auto &p : scanner.beacons)
                temp.beacons.emplace_back(Point3d(p.z, p.y, -p.x));
            return temp;
        }
        case 21: {
            for (auto &p : scanner.beacons)
                temp.beacons.emplace_back(Point3d(-p.y, p.z, -p.x));
            return temp;
        }
        case 22: {
            for (auto &p : scanner.beacons)
                temp.beacons.emplace_back(Point3d(-p.z, -p.y, -p.x));
            return temp;
        }
        case 23: {
            for (auto &p : scanner.beacons)
                temp.beacons.emplace_back(Point3d(p.y, -p.z, -p.x));
            return temp;
        }    
        default: return temp;
    }
}

Point3d findCenter(Scanner &ref_scanner, Scanner &scanner)
{
    std::unordered_map<Point3d, int, MyHashFunc> all_centers;
    Point3d center; 
    for (auto &p_ref : ref_scanner.beacons)
        for (auto &p : scanner.beacons) { 
            center = p_ref - p; // pref is not going by reference because operator- changes it!!!
            if (all_centers.find(center) != all_centers.end()) {
                if (++all_centers[center] == 12)
                    return center;
            }
            else all_centers[center] = 1;
        }
    return Point3d(0,0,0);
}

void findNeighbours(std::forward_list<Scanner> &scanners_list,
                   std::vector<Scanner> &abs_scanners, int index)
{
    auto it = scanners_list.before_begin();
    Scanner temp_scanner;   Point3d position;   bool erased = false;
    for (auto it1 = scanners_list.begin(); it1 != scanners_list.end(); ) {
        for (int i = 0; i < 24; i++) {
            temp_scanner = std::move(ChOrientOfCube(*it1, i));
            position = findCenter(abs_scanners[index], temp_scanner);
            if (position != Point3d(0,0,0)) {
                temp_scanner.position = position;
                abs_scanners.push_back(std::move(createAbsScanner(temp_scanner)));
                it1++;
                scanners_list.erase_after(it);
                position = Point3d(0,0,0);
                i = 24;     erased = true;
                continue;
            }
        }  
        if (erased) erased = false;    
        else {it++; it1++;}
    }
}

int countBeacons (std::vector<Scanner> &abs_scanners)
{
    std::unordered_set<Point3d, MyHashFunc> beacons_map;
    for (auto &scanner : abs_scanners)
        for (auto &beacon : scanner.beacons)
            beacons_map.insert(beacon);
    return beacons_map.size();
}

int manhattanDist(const Point3d &p1, const Point3d &p2)
{
    return abs(p1.x - p2.x) + abs(p1.y - p2.y) + abs(p1.z - p2.z);
}

int main(int argc, char** argv)
{
    std::cout << "Beacon hell!" << std::endl;
    std::ifstream input(argv[1]);
    if (!input) {
        std::cout << "Error opening the file!" << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<Scanner>abs_scanners;
    std::forward_list<Scanner> scanners_list;
    parseScanner(input, scanners_list);
    abs_scanners.push_back(scanners_list.front()); //setting scanner zero as absolutely positioned
    scanners_list.pop_front();

    int index = 0;
    while (!scanners_list.empty()){
        findNeighbours(scanners_list, abs_scanners, index);
        index++;
    }       
    std::cout << "Number of beacons: " << countBeacons(abs_scanners) << std::endl;
    
    int manhattan_max = 0; int temp;
    for (auto it1 = abs_scanners.begin(); it1 != abs_scanners.end(); it1++)
        for (auto it2 = it1; it2 != abs_scanners.end(); it2++) {
            temp = manhattanDist((*it1).position, (*it2).position);
            manhattan_max = (temp > manhattan_max)? temp : manhattan_max;
        } 
    std::cout << "Max Manhattan distance: " << manhattan_max << std::endl;

}