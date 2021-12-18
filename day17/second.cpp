#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

const char* DIGITS = "-0123456789";

struct XYinstep {
    std::vector<int> vec_x;
    std::vector<int> vec_y;
};

struct Velocity {
    int vel_x;
    int vel_y;
    Velocity(int x, int y) : vel_x(x), vel_y(y) {}
    bool operator==(const Velocity &v) const
    {
        return vel_y == v.vel_y && vel_x == v.vel_x; 
    }
};

struct MyHashFunc {
    size_t operator()(const Velocity &v) const
    {
        return 200 * v.vel_x + v.vel_y;
    }
};

bool inBetween(int n, int x1, int x2)
{
    return (x1 > x2)? ((n >= x2) && (n <= x2)) : ((n >= x1) && (n <= x2));
}

int inc(int x) 
{
    return (x > 0)? 1 : -1;
}

int sgn(int x)
{
    return (x >= 0)? 1: -1;
}

int sumN(int n, int count = 0)
{
    if (n <= 0) return -sumN(-n + count - 1, count);
    return (count == 0)? n*(n+1)/2 : n*count - (count*(count - 1) /2);
}

int findLow(int x, int step) 
{   
    if (x >= 0) {
        int temp = x / step + step / 2 ;
        if (temp >= step) {
            while (sumN(temp, step) < x) temp ++;
            return temp;
        }
        return step;
    } 
    else {
        int temp = ((x / step + step) > 0)? -1 : (x / step + step) ; 
        while (sumN(temp, step) > x) temp--;
        return temp;
    }
}

int findHigh(int x, int step)
{
    if (x >= 0) {
        int temp = x / step + step / 2; 
        if (temp > step) {
            while (sumN(temp, step) > x) temp--;
            return temp;
        }
        return  step;
    }
    else {
        int temp = x / step; 
        while (sumN(temp, step) < x && temp < 0) temp++;
        return temp;
    }
}

int main(int argc, char** argv)
{
    std::ifstream input(argv[1]);
    std::cout << "Probe launcher!" << std::endl;
    if (!input) {
        std::cout << "Error opening the file!" << std::endl;
        return EXIT_FAILURE;
    } 
    std::string line; std::getline(input, line);
    std::vector<int> coords;
    int pos;
    while ( (pos = line.find_first_of(DIGITS, 0)) != std::string::npos) {
        line.erase(0, pos);
        coords.push_back(std::stoi(line));
        pos = line.find_first_not_of(DIGITS, 0);
        line.erase(0,pos);
    }
    int max_y_vel = (abs(coords[2]) > abs(coords[3]))? 
                        abs(coords[2]) - 1 : abs(coords[3]) - 1;

    int x_low = (abs(coords[0]) < abs(coords[1]))? coords[0] : coords[1];
    int x_high = (abs(coords[0]) < abs(coords[1]))? coords[1] : coords[0];
    std::unordered_map<int, XYinstep> hmap;
    int step = 1;     bool cond = false;
    
    //filling x hash map;
    while (!cond) {
        int temp_x_low = findLow(x_low, step);
        int temp_x_high = findHigh(x_high, step);
        if (abs(sumN(temp_x_high, step)) > abs(x_high)) cond = true;
        else 
            for (int i = temp_x_low; abs(i) <= abs(temp_x_high); i += inc(i)) { 
                hmap[step].vec_x.push_back(i);
                for (auto &mem : hmap[0].vec_x)
                    if (std::find(hmap[step].vec_x.begin(), 
                    hmap[step].vec_x.end(), mem) == hmap[step].vec_x.end())
                        hmap[step].vec_x.push_back(mem);
                if ( i == step ) 
                    hmap[0].vec_x.push_back(step);
            }
        step++;
    }
    int max_step_x = step - 2;
    
    //filling y hash map
    int y_low = (abs(coords[2]) < abs(coords[3]))? coords[2] : coords[3];
    int y_high = (abs(coords[2]) < abs(coords[3]))? coords[3] : coords[2];
    step = 1;
    cond = false;
    while (!cond) {
        int temp_y_low = findLow(y_low, step);
        int temp_y_high = findHigh(y_high, step);
        if (temp_y_high == 0) cond = true;
        else
            for (int i = temp_y_low; abs(i) <= abs(temp_y_high); i += inc(i)) { 
                hmap[step].vec_y.push_back(i);
                if (i < 0) hmap[2*abs(i+1) + 1 + step].vec_y.push_back(abs(i+1)); // positive projection of values
            }
        step++;
    }

    std::unordered_set<Velocity, MyHashFunc> vel;
    for (auto &mem : hmap)
        for (auto &y : mem.second.vec_y)
            if (mem.first > max_step_x) 
                for (auto &x : hmap[0].vec_x) 
                    vel.insert(Velocity(x, y));
            else 
                for (auto &x : mem.second.vec_x)
                    vel.insert(Velocity(x, y));

    std::cout << "Number of shots: " << vel.size() << std::endl;
}
//Nije 2064