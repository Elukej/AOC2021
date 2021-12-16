#include <fstream>
#include <limits>
#include <queue>
#include <functional>
#include "../include/luka_string.h"

struct Dir {
    int vert;
    int hor;
    Dir(int v, int h): vert(v), hor(h) {}
    Dir(std::initializer_list<int> lista)
    {
        auto it = lista.begin();
        vert = *it; it++;
        hor = *it; it++;
        if (it != lista.end())
            std::cout << "Warning, rest of the list is garbage!" << std::endl;
    }
};

std::vector<Dir> DIRECTIONS = {{1,0},{0,1},{-1,0},{0,-1}};

struct Track {
    bool visited;
    int min_dist;
    int row;
    int col;
    Track(){}
    Track(bool v, int m, int r = 0, int c = 0): visited(v),
    min_dist(m), row(r), col(c) {}
};

bool inBounds(int row, int col, int maxrow, int maxcol)
{
    return (row >= 0) && (col >= 0) && (row < maxrow) && (col < maxcol); 
}

int tileModer(int num, int inc)
{
    return (num + inc > 9)? num+inc-9: num + inc; 
}

void tileFiller(std::vector<std::vector<int>> &ref_vec, 
                    std::vector<std::vector<int>> &new_vec,
                    int tile_row, int tile_col)
{
    int inc = tile_row + tile_col;
    int row_start = ref_vec.size() * tile_row;
    int col_start = ref_vec[0].size() * tile_col;
    for (int i=0; i<ref_vec.size(); i++)
        for (int j=0; j<ref_vec[i].size(); j++) 
            new_vec[row_start + i][col_start + j] = 
            tileModer(ref_vec[i][j], inc);
}

void tileNetFiller(std::vector<std::vector<int>> &ref_vec, 
                 std::vector<std::vector<int>> &new_vec)
{
    for (int i=0; i<5; i++)
        for (int j=0; j<5; j++) 
            tileFiller(ref_vec, new_vec, i, j);
}

int minRiskPath(std::vector<std::vector<int>> &lab, 
                std::vector<std::vector<Track>> &tracker)
{
    auto cmp = [](const Track &t1, const Track &t2) {
        return t2.min_dist < t1.min_dist;
    };
    /*std::priority_queue<std::reference_wrapper<Track>, // wrapper in order to have queue of references
    std::vector<std::reference_wrapper<Track>>, decltype(cmp)> pq(cmp); */
    /* Nice idea for reference wrapper but unusable because queue won't react when the reference 
    content changes, and even worse, the old references in the queue won't be able to swim to their
    true minimum spot when they reach the before inserted version of themselves which stops their
    proggression since they aren't smaller from themselves. Keeping this for reminding myself. */
    std::priority_queue<Track, std::vector<Track>, decltype(cmp)> pq(cmp);
    pq.push(tracker[0][0]);
    int row, col;
    while (true) {
        auto temp = pq.top();
        pq.pop();
        if ((temp.row == lab.size() - 1) && (temp.col == lab[0].size() - 1))
            return temp.min_dist;
        tracker[temp.row][temp.col].visited = true;
        for (auto &dir : DIRECTIONS) {
            row = temp.row + dir.vert;
            col = temp.col + dir.hor;
            if (inBounds(row, col, lab.size(), lab[0].size())) {
                const int &new_min = tracker[temp.row][temp.col].min_dist 
                                    + lab[row][col];
                if (!tracker[row][col].visited 
                    && (tracker[row][col].min_dist > new_min)) 
                {
                    tracker[row][col].min_dist = new_min;
                    pq.push(tracker[row][col]);        
                }
            }
        }
    }
}

int main(int argc, char** argv)
{
    std::cout << "Labyrinth!" << std::endl;
    std::ifstream input(argv[1]);
    if (!input) {
        std::cout << "Error opening the file!" << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<std::vector<int>> labyrinth;
    std::string line;
    while (std::getline(input, line)) {
        std::vector<int> temp; 
        for (auto ch : line) 
            temp.push_back(ch - 48);
        labyrinth.push_back(temp);
    }

    std::vector<std::vector<int>> lab_tiles(5*labyrinth.size(),
    std::vector<int>(5*labyrinth[0].size(), 0));
    tileNetFiller(labyrinth, lab_tiles);

    std::vector<std::vector<Track>> 
    tracker(lab_tiles.size(), std::vector<Track>(lab_tiles[0].size(), 
    Track(false, std::numeric_limits<int>::max() )));
    tracker[0][0].min_dist = 0; 

    for (int i=0; i<tracker.size(); i++)
        for (int j=0; j<tracker.size(); j++) {
            tracker[i][j].row = i;
            tracker[i][j].col = j;
        }

    std::cout << "least riskiest path length: "
    << minRiskPath(lab_tiles, tracker) << std::endl;
}