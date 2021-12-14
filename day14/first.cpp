#include <fstream>
#include <unordered_map>
#include <limits>
#include "../include/luka_string.h"

struct MinMax {
    int mcom;
    int lcom;
    MinMax(int m, int l):mcom(m),lcom(l){}
};

std::string polymerization(std::string &polymers, 
            std::unordered_map<std::string, char> hmap,
            int letters[])
{
    std::string new_polymers;
    for (int i = 0; i < polymers.length() - 1; i++) {
        new_polymers.push_back(polymers[i]);
        new_polymers.push_back(hmap[polymers.substr(i,2)]);
        letters[hmap[polymers.substr(i,2)] - 65]++;
    }
    new_polymers.push_back(polymers.back());
    return new_polymers;
}

MinMax numOfApps(int letters[])
{
    int mcom = letters[0], lcom = std::numeric_limits<int>::max();
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

    int letters[26] = {0};
    std::unordered_map<std::string, char> hmap;
    std::string line, polymers;
    std::getline(input, polymers);
    input >> std::ws;
    for (auto c : polymers)
        letters[c - 65]++;

    while (std::getline(input, line)) {
        std::vector<std::string> vec = luka_string::split(line, " -> ");
        hmap[vec[0]] = vec[1].front();
    }   

    for (int i=0; i<10; i++)
        polymers = std::move(polymerization(polymers, hmap, letters));
   
    MinMax min_max_pair = numOfApps(letters);
    std::cout << "Max - Min: "
    << min_max_pair.mcom - min_max_pair.lcom << std::endl;
}
