#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <math.h>
#include "../include/luka_string.h"

struct BoardField {
    int row;
    int col;
    bool visited;
    BoardField(){}
    BoardField(int row, int col, bool visited = false):
        row(row), col(col), visited(visited){}
    
};

struct RowColTracker {
    std::vector<int> row;
    std::vector<int> col;
    RowColTracker(){}
    RowColTracker(size_t x): row(x,0), col(x,0) {}
};

std::vector<int> parse_nums(std::string str, std::string delim)
{
    std::vector<int> vec;  size_t pos; 
    while (( pos = str.find(delim)) != std::string::npos) {
        vec.push_back(std::stoi(str.substr(0,pos)));
        str.erase(0, pos + delim.length());
        ltrim(str);
    }
    vec.push_back(std::stoi(str));

    return vec;
}

void parse_Board(std::ifstream& input,
    std::vector<std::unordered_map<int, BoardField>> &vec)
{
    input >> std::ws;
    std::string line;   int row = 0, col = 0;
    std::unordered_map<int, BoardField> temp_board;
    while (( input.peek() != '\n') && !input.eof()) {
        std::getline(input, line);
        ltrim(line);
        std::vector<int> temp_vec(parse_nums(line, " "));
        col = 0; 
        for (int broj : temp_vec)
            temp_board[broj] = BoardField(row,col++);
        row++;
    }
    vec.push_back(temp_board);
}

int main(int argc, char** argv)
{
    std::cout << "Bingo game with a squid!" << std::endl;
    std::ifstream input(argv[1]);
    std::string line;
    std::getline(input, line);
    std::vector<int> nums = parse_nums(line, ",");

    std::vector<std::unordered_map<int, BoardField>> board_list;
    while (!input.eof()) parse_Board(input, board_list);
    int board_dim = sqrt(board_list[0].size());
    std::vector<RowColTracker> tracker(board_list.size());
    for (auto &member : tracker) member = RowColTracker((size_t) board_dim);

    int i = 0 , j = 0; bool cond = true;
    for( ; cond && i < nums.size(); i++) {
        j = 0;
        for ( ; cond && j < board_list.size(); j++ ) {           
            if (board_list[j].find(nums[i]) != board_list[j].end()) {
                auto &field = board_list[j][nums[i]];
                field.visited = true;
                if ( (++(tracker[j].col[field.col])) == board_dim ||
                     (++(tracker[j].row[field.row])) == board_dim
                    ) cond = false;       
            } 
        }
    } i--; j--;
    int sum = 0;
    for (auto board : board_list[j])
        if (!board.second.visited) sum += board.first;
    std::cout << nums[i] << std::endl << j <<std::endl;
    std::cout << sum * nums[i] << std::endl;

}