#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <math.h>

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

const std::string WHITESPACE = " \n\r\t\f\v";
 
void ltrim(std::string &s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    s.erase(0, start);
}
 
void rtrim(std::string &s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    s.erase(end);
}
 
void trim(std::string &s) {
    rtrim(s); ltrim(s);
}

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

int main()
{
    std::cout << "Bingo game with a squid!" << std::endl;
    std::ifstream input("input.txt");
    std::string line;
    std::getline(input, line);
    std::vector<int> nums = parse_nums(line, ",");

    for (int i = 0; i < nums.size(); i++) {
        std::cout << nums[i] << " ";
    }
    std::cout << std::endl;

    std::vector<std::unordered_map<int, BoardField>> board_list;
    while (!input.eof()) parse_Board(input, board_list);
    int board_dim = sqrt(board_list[0].size());
    std::vector<RowColTracker> tracker(board_list.size());
    for (auto &member : tracker) member = RowColTracker((size_t) board_dim);

    int i = 0; ; bool cond = true;
    std::vector<std::unordered_map<int, BoardField>>::iterator j;
    for( ; cond && i < nums.size(); i++) {
        j = board_list.begin();
        for ( ; cond && j != board_list.end(); ) {           
            if ((*j).find(nums[i]) != (*j).end()) {
                auto &field = (*j)[nums[i]];
                field.visited = true;
                if ( (++(tracker[std::distance(board_list.begin(), j)].col[field.col])) == board_dim ||
                     (++(tracker[std::distance(board_list.begin(), j)].row[field.row])) == board_dim
                    ) {
                    if (board_list.size() > 1) {    
                        board_list.erase(j);
                        tracker.erase(tracker.begin() + std::distance(board_list.begin(), j));
                        continue;
                    }
                    else cond = false;
                }       
            } 
            j++;
        }
    } i--; j--;
    int sum = 0;
    for (auto board : board_list[std::distance(board_list.begin(), j)])
        if (!board.second.visited) sum += board.first;
    std::cout << nums[i] << std::endl << std::distance(board_list.begin(), j) <<std::endl;
    std::cout << sum * nums[i] << std::endl;

}