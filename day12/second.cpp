#include <fstream>
#include <unordered_map>
#include <forward_list>
#include <stack>
#include "../include/luka_string.h"

struct Node {
    int counter;
    std::vector<std::string> next;
    Node(int counter = 0):counter(counter){}
    //list of hashmap iterators for the graph
};

struct StackFrame {
    std::string cave;
    int index;
    bool has2lower;
    StackFrame(std::string s, int i, bool t):cave(s), 
    index(i), has2lower(t) {}
};

int countPathsIterative( std::unordered_map<std::string, Node> &graph)
{
    int count = 0;
    std::string cave("start");
    int index = 0;
    bool has2lower = false;
    std::stack<StackFrame> stack;

    while ( !stack.empty() || (index < graph[cave].next.size()) ) {
        if (index == graph[cave].next.size()) {
            cave = stack.top().cave;
            index = stack.top().index;
            has2lower = stack.top().has2lower;
            stack.pop();
            graph[cave].counter--; index++;
            continue;
        }
        if (graph[cave].next[index] == "end") {
            count++; index++; continue;
        } 
        std::string &temp = graph[cave].next[index];
        if (islower(temp[0]) && has2lower && 
                (graph[temp].counter > 0) || temp == "start") { 
                index++; continue;
        }
        stack.push(StackFrame(cave, index, has2lower));
        graph[cave].counter++; 
        cave = temp;
        index = 0;  
        if ( islower(temp[0]) && graph[temp].counter == 1 
            && !has2lower)
                has2lower = true;              
    }
    return count;
}

int countPaths(const std::string &s1, bool has2lower,
               std::unordered_map<std::string, Node> &graph)
{
    int count = 0;

    if (s1 != "end") {
        graph[s1].counter++;
        for (auto &mem : graph[s1].next) {
            if (isupper(mem[0]) || graph[mem].counter == 0) 
                count += countPaths(mem, has2lower, graph);
            else if (!has2lower && (mem != "start"))
                count += countPaths(mem, true, graph);
        }
        graph[s1].counter--;
    }
    else return 1;
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
        cave_network[temp[0]].next.push_back(temp[1]);
        if (cave_network.find(temp[1]) == cave_network.end())
            cave_network[temp[1]] = Node(); 
        cave_network[temp[1]].next.push_back(temp[0]);
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
