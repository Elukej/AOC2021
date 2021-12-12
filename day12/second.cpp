#include <fstream>
#include <unordered_map>
#include <forward_list>
#include "../include/luka_string.h"


struct Node {
    int counter;
    std::forward_list<std::string> next;
    Node(int counter = 0):counter(counter){}
    //list of hashmap iterators for the graph
};

int countPaths(const std::string &s1, bool has2lower,
               std::unordered_map<std::string, Node> graph)
{
    int count = 0;
    bool in_graph = (graph.find(s1) != graph.end());
    if (in_graph) {
        graph[s1].counter++;
        for (auto &mem : graph[s1].next) {
            if (isupper(mem[0]) || graph[mem].counter == 0) 
                count += countPaths(mem, has2lower, graph);
            if (islower(mem[0]) && graph[mem].counter == 1 && !has2lower
                && (mem != "start") && (mem != "end"))
                count += countPaths(mem, true, graph);
        }
        graph[s1].counter--;
    }
    if (s1 == "end") count++;
    return count;
}

std::unordered_map<std::string, Node> createGraph(std::ifstream &input)
{
    std::unordered_map<std::string, Node> cave_network;
    std::string line;
    while (std::getline(input, line)) {
        std::vector<std::string> temp = luka_string::split(line,"-");
        if (cave_network.find(temp[0]) == cave_network.end())
            cave_network[temp[0]] = Node(); 
        cave_network[temp[0]].next.push_front(temp[1]);
        if (cave_network.find(temp[1]) == cave_network.end())
            cave_network[temp[1]] = Node(); 
        cave_network[temp[1]].next.push_front(temp[0]);
    }
    return cave_network;
}

int main(int argc, char** argv)
{
    std::cout << "Cavern paths!" << std::endl;
    std::ifstream input(argv[1]);
    if (!input) {
        std::cout << "Error opening the file!" << std::endl;
        return EXIT_FAILURE;
    }
   
    std::unordered_map<std::string, Node> cave_network = createGraph(input);
    int total_paths = countPaths("start", false, cave_network);
    std::cout << "Total path number: " << total_paths << std::endl;
}
//Nije 143283