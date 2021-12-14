#include <fstream>
#include <unordered_map>
#include <limits>
#include "../include/luka_string.h"

struct MinMax {
    unsigned long mcom;
    unsigned long lcom;
    MinMax(unsigned long m, unsigned long l):mcom(m),lcom(l){}
};

std::string newPairFirst(const std::string &old_pair, 
                    std::unordered_map<std::string,char> &hmap)
{
    return (std::string("") + old_pair.front() + hmap[old_pair]);
}

std::string newPairSecond(const std::string &old_pair, 
                    std::unordered_map<std::string,char> &hmap)
{
    return (std::string("") + hmap[old_pair]) + old_pair.back();
}

std::unordered_map<std::string, unsigned long> polymerization( 
            std::unordered_map<std::string, char> &hmap,
            std::unordered_map<std::string, unsigned long> &pair_counting_map,
            unsigned long letters[])
{
    std::unordered_map<std::string, unsigned long> new_map;
    for (auto &mem : pair_counting_map) {
        const std::string &s1 = newPairFirst(mem.first, hmap);
        const std::string &s2 = newPairSecond(mem.first, hmap);
        if (new_map.find(s1) == new_map.end()) 
            new_map[s1] = mem.second;
        else 
            new_map[s1] += mem.second;
        if (new_map.find(s2) == new_map.end()) 
            new_map[s2] = mem.second;
        else 
            new_map[s2] += mem.second;        
        letters[hmap[mem.first] - 65] += mem.second;
    }
   // pair_counting_map.clear();
    return new_map;
}

MinMax numOfApps(unsigned long letters[])
{
    unsigned long mcom = letters[0], lcom = std::numeric_limits<unsigned long>::max();
    for (int i=0; i<26; i++) 
        if (letters[i] > 0) {
            if (letters[i] > mcom) mcom = letters[i];
            if (letters[i] < lcom) lcom = letters[i];
        }
    return MinMax(mcom, lcom);
}

int main(int argc, char** argv)
{
    std::cout << "Polymerization!" << std::endl;
    std::ifstream input(argv[1]);
    if (!input) {
        std::cout << "Error opening the file!" << std::endl;
        return EXIT_FAILURE;
    }

    unsigned long letters[26] = {0};
    std::unordered_map<std::string, char> hmap;
    std::unordered_map<std::string, unsigned long> pair_counting_map;
    std::string line, polymers;
    std::getline(input, polymers);
    input >> std::ws;

    for (int i=0; i<polymers.length() - 1; i++) {
        if (pair_counting_map.find(polymers.substr(i,2)) == 
                                    pair_counting_map.end())
            pair_counting_map[polymers.substr(i,2)] = 1;
        else pair_counting_map[polymers.substr(i,2)]++;  //creating map tracking pairs
        letters[polymers[i] - 65]++; //counting letter numbers
    }
    letters[polymers.back() - 65]++; //last letter

    while (std::getline(input, line)) {
        std::vector<std::string> vec = luka_string::split(line, " -> ");
        hmap[vec[0]] = vec[1].front();  // map of pair to character
    }   

    //better function for polymerization then in first
    for (int i=0; i<40; i++) {
        pair_counting_map = std::move(polymerization(hmap, pair_counting_map,letters));
    }
   
    MinMax min_max_pair = numOfApps(letters);
    std::cout << "Max - Min: "
    << min_max_pair.mcom - min_max_pair.lcom << std::endl;
}