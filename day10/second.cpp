#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <vector>
#include <algorithm>

bool areMatchingBraces(char c1, char c2)
{
    return (c1 == '(' && c2 == ')') ||
           (c1 == '[' && c2 == ']') ||
           (c1 == '{' && c2 == '}') ||
           (c1 == '<' && c2 == '>'); 
}

bool isLeftBrace(char c) 
{
    return (c == '(') || (c == '[') ||
           (c == '{') || (c == '<');
}

int autoCompletePoints(char c)
{
    if (c == '(') return 1;
    if (c == '[') return 2;
    if (c == '{') return 3;
    if (c == '<') return 4;
    return 0;
}

unsigned long autoCompleteScore(std::string line)
{
    std::stack<char> left_braces;
    for (auto it = line.begin(); it != line.end(); it++) 
        if (isLeftBrace(*it)) left_braces.push(*it);
        else {
            if (!left_braces.empty())
                if (areMatchingBraces(left_braces.top(), *it)) 
                    left_braces.pop();
                else return 0;
            else return 0;
        }
    unsigned long points = 0;
    while (!left_braces.empty()) { 
        points = points * 5 + autoCompletePoints(left_braces.top());
        left_braces.pop();
    }
    return points;
}

int main(int argc, char** argv)
{
    std::cout << "Bracket parser!" << std::endl;
    std::ifstream input(argv[1]);
    if (!input) {
        std::cout << "Error opening the file!" << std::endl;
        return EXIT_FAILURE;
    }

    std::string line;
    std::vector<unsigned long> auto_complete_points;
    while (std::getline(input, line)) {
        unsigned long temp = autoCompleteScore(line);
        if (temp) auto_complete_points.push_back(temp);
    }
    std::sort(auto_complete_points.begin(), auto_complete_points.end());
    unsigned long median = auto_complete_points[auto_complete_points.size() / 2];
    std::cout << "Middle auto complete score: " << median << std::endl;
}