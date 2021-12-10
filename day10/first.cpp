#include <iostream>
#include <fstream>
#include <string>
#include <stack>

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

int errorPoint(char c)
{
    if (c == ')') return 3;
    if (c == ']') return 57;
    if (c == '}') return 1197;
    if (c == '>') return 25137;
    return 0;
}

int findError(std::string line)
{
    std::stack<char> left_braces;
    for (auto it = line.begin(); it != line.end(); it++) 
        if (isLeftBrace(*it)) left_braces.push(*it);
        else {
            if (!left_braces.empty())
                if (areMatchingBraces(left_braces.top(), *it)) 
                    left_braces.pop();
                else return errorPoint(*it);
            else return errorPoint(*it);
        }
    return 0;
}

int main(int argc, char** argv)
{
    std::cout << "Bracket parser!" << std::endl;
    std::ifstream input(argv[1]);
    if (!input) {
        std::cout << "Error opening the file!" << std::endl;
        return EXIT_FAILURE;
    }

    int error_points = 0;
    std::string line;
    while (std::getline(input, line)) 
        error_points += findError(line);
    std::cout << "Total error score is: " << error_points << std::endl;
}