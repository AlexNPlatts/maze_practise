#ifndef MAZE_GENERATION_H
#define MAZE_GENERATION_H

#include <vector>
#include <string>


class Maze{
/*
A maze of cells
*/
    
private:
    struct Cell{
    /*
    A cell in a maze;
    */
        const int row;
        const int column;
        bool visited;
        bool is_entry;
        bool is_exit;
        // the order in both vectors is Left, Right, Up, Down
        std::vector<bool> walls;
        std::vector<Cell*> neighbours;

        Cell(int irow, int icolumn) :
            row(irow), column(icolumn), visited(false),
            is_entry(false), is_exit(false), walls({true, true, true, true}),
                neighbours({nullptr, nullptr, nullptr, nullptr}){};

    };

    const int n_rows;
    const int n_columns;
    std::vector<std::vector<Cell*>> grid;
    Cell* open_cell(Cell*);

public:
    Maze() = delete;
    Maze(const int /*n_rows*/, const int /*n_columns*/);
    ~Maze();
    void make_backtrack_maze();
    void print();

    friend class Graph;
};


#endif