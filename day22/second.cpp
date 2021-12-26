#include <fstream>
#include <tuple>
#include <string.h>
#include <set>
#include <unordered_set>
#include <forward_list>
#include "../include/luka_string.h"

struct Cuboid {
    int xl;  int xh;
    int yl;  int yh;
    int zl;  int zh;
    bool light;
    std::forward_list<Cuboid> dark;
    std::unordered_set<int> isect_elems;
    Cuboid() {
        xl = (xh = (yl = (yh = (zl = (zh = (light = 0))))));
    }
    
    Cuboid(int xl, int xh, int yl, int yh, int zl, int zh, bool l = false):
    xl(xl), xh(xh), yl(yl), yh(yh), zl(zl), zh(zh), light(l) {}
    
    Cuboid(std::vector<int> vec, const char* l)
    {
        xl = vec[0]; xh = vec[1];
        yl = vec[2]; yh = vec[3];
        zl = vec[4]; zh = vec[5];
        if (!strcmp(l,"on")) light = 1;
        else light = 0;
    }

    bool operator==(const Cuboid &c1) const
    {
        return xl == c1.xl && xh == c1.xh &&
               yl == c1.yl && yh == c1.yh &&
               zl == c1.zl && zh == c1.zh &&
               light == c1.light &&
               isect_elems == c1.isect_elems;
    }
    
    bool operator<(const Cuboid &c1) const
    {
        if (*this == c1) return false;
        return xl < c1.xl || (xl == c1.xl && xh < c1.xh) ||
              (xl == c1.xl && xh == c1.xh && yl < c1.yl) ||
        (xl == c1.xl && xh == c1.xh && yl == c1.yl && yh < c1.yh) ||
        (xl == c1.xl && xh == c1.xh && yl == c1.yl && yh == c1.yh 
                                                    && zl < c1.zl) ||
        (xl == c1.xl && xh == c1.xh && yl == c1.yl && yh == c1.yh &&
                                            zl == c1.zl && zh < c1.zh);                                            
    }
};


bool isZeroTuple(const std::tuple<int,int> &tup)
{
    return !std::get<0>(tup) && !std::get<1>(tup);
}

bool isZeroCube(const Cuboid &c)
{
  return  c.xl == 0 && c.xh == 0 && 
          c.yl == 0 && c.yh == 0 &&
          c.zl == 0 && c.zh == 0;
}

auto axIntersect(const int x1l, const int x1h, const int x2l, const int x2h)
{
    if (x1l > x2h || x2l > x1h) 
        return std::make_tuple(0,0);
    int lower = (x1l < x2l)? x2l : x1l;
    int higher = (x1h > x2h)? x2h : x1h;
    return std::make_tuple(lower, higher);
}

auto cubeIntersect(const Cuboid &c1, const Cuboid &c2)
{   
    Cuboid isect;
    auto tupx = axIntersect(c1.xl, c1.xh, c2.xl, c2.xh);
    auto tupy = axIntersect(c1.yl, c1.yh, c2.yl, c2.yh);
    auto tupz = axIntersect(c1.zl, c1.zh, c2.zl, c2.zh);
    if (!isZeroTuple(tupx) && !isZeroTuple(tupy) && !isZeroTuple(tupz)) 
        std::tie(isect.xl, isect.xh, isect.yl, isect.yh, isect.zl, isect.zh) =
             std::tuple_cat(tupx, tupy, tupz);
    return isect;
}

long cubeVol(const Cuboid &cube)
{
    return  (long) (cube.xh - cube.xl + 1) * 
            (long) (cube.yh - cube.yl + 1) *
            (long) (cube.zh - cube.zl + 1); //+1 is to account for both border inclusion
}

int difBy1(const Cuboid &c1, const Cuboid &c2)
{
    int dif, count = 0;
    for (auto i : c2.isect_elems)
        if (c1.isect_elems.find(i) == c1.isect_elems.end()) {
            count++; dif = i;
        }
    if (count == 1) 
        return dif; 
    else return -1;
}

void listInsert(std::forward_list<Cuboid> &cubes, Cuboid &cube)
{
    for (auto &mem : cubes)
        if (mem.isect_elems == cube.isect_elems)   return;
    cubes.push_front(cube);
}

long cubeUnionVol(const std::forward_list<Cuboid> &cubes)
{
    std::forward_list<Cuboid> new_cubes;
    long volume = 0;
    for (auto it1 = cubes.begin(); it1 != cubes.end(); it1++) {
        for (auto it2 = std::next(it1); it2 != cubes.end(); it2++) {
            auto dif = difBy1(*it1, *it2);
            if (dif != -1) {
                Cuboid isect = cubeIntersect(*it1, *it2);
                if (!isZeroCube(isect)) { 
                    isect.isect_elems = (*it1).isect_elems;
                    isect.isect_elems.insert(dif);
                    listInsert(new_cubes, isect);
                }
            }
        }
        volume += cubeVol(*it1); 
    }
    if (new_cubes.empty()) return volume;
    return volume -= cubeUnionVol(new_cubes);
}

void darkCubeUpdate(std::forward_list<Cuboid> &lista, Cuboid &cube)
{   
    auto it = lista.before_begin();
    for (auto it1 = lista.begin(); it1 != lista.end(); ) {
        const Cuboid &temp = cubeIntersect(*it1, cube);
        if (temp == cube) return;
        if (temp == *it1) {
            it1++;  lista.erase_after(it);
        }
        else { it++; it1++;}
    }
    lista.push_front(std::move(cube));
}

int main(int argc, char** argv) {
    std::cout << "Cuboid madness!" << std::endl;
    std::ifstream input(argv[1]);
    if (!input) {
        std::cout << "Error opening the file!";
        return EXIT_FAILURE;
    }

    std::string line;
    char light[3];
    std::vector<Cuboid> cubes;
    while (std::getline(input, line)) {
        sscanf(line.c_str(),"%s", light);
        Cuboid temp = Cuboid(luka_string::num_parser(line), light);
            for (auto &mem : cubes) {
                Cuboid isect = cubeIntersect(temp, mem);
                if (!isZeroCube(isect))
                   darkCubeUpdate(mem.dark, isect);
            }
            if (temp.light) cubes.push_back(std::move(temp));
    }

    for (auto &mem : cubes) {
        int i = 0;
        for (auto it = mem.dark.begin(); it != mem.dark.end(); it++) {
            (*it).isect_elems.insert(i); i++;
        }
    }

    long total_volume = 0l;
    for (auto &mem : cubes)
        total_volume += ((cubeVol(mem) - cubeUnionVol(mem.dark)));

    std::cout << "Volume: " << total_volume << std::endl;
} 