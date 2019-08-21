#include <vector>
#include <algorithm>
#include <iostream>
#include <string>

#include "maze_generation.h"

void Maze::print() {
    for (int i = 0; i != n_rows; ++i) {
        // top line for cell
        std::cout << "#"; // left most wall
        for (int j = 0; j != n_columns; ++j) {
            if (grid[i][j]->walls[2]) {
                std::cout << "##";
            } else {
                std::cout << " #";
            }
        }
        std::cout << std::endl << "#"; // next line and left most
        //bottom line for cell (this one contains the cells center)
        for (int j = 0; j != n_columns; ++j) {
            // entry/exit/nothing
            if (grid[i][j]->is_entry) {
                std::cout << "+"; // start cell
            } else if (grid[i][j]->visited) {
                std::cout << "o"; // visited cell
            } else if (grid[i][j]->is_exit) {
                std::cout << "-"; // exit cell
            } else {
                std::cout << " "; // unvisited cell
            }
            // right wall
            if (grid[i][j]->walls[1]) {
                std::cout << "#";
            } else {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }
    for (int i=0; i!=n_columns; ++i) {
        std::cout << "##";
    }
    std::cout << "#" << std::endl;
}

Maze::Maze(const int in_rows, const int in_columns)
        : n_rows(in_rows), n_columns(in_columns) {
    /*
    Maze constructor
    */
    std::vector<std::vector<Maze::Cell*>>
        grid(n_rows, std::vector<Maze::Cell*>(n_columns));

    // initialize Maze::Cells
    for (int i = 0; i != n_rows; ++i) {
        for (int j = 0; j != n_columns; ++j) {
            Maze::Cell* c = new Maze::Cell(i, j);
            grid[i][j] = c;
        }
    }
    // add neighbours to Maze::Cells
    for (int i = 0; i != n_rows; ++i) {
        for (int j = 0; j != n_columns; ++j) {
            if (i != 0) {
                // Maze::Cell above
                grid[i][j]->neighbours[2] = grid[i-1][j];
            }
            if (i != n_rows-1) {
                // Maze::Cell below
                grid[i][j]->neighbours[3] = grid[i+1][j];
            }
            if (j != 0) {
                // Maze::Cell left
                grid[i][j]->neighbours[0] = grid[i][j-1];
            }
            if (j != n_columns-1) {
                // Maze::Cell right
                grid[i][j]->neighbours[1] = grid[i][j+1];
            }
        }
    }
    this->grid = grid;
}

Maze::~Maze() {
    /*
    Maze deconstructor
    */
    for (int i = 0; i != n_rows; ++i) {
        for (int j = 0; j != n_columns; ++j) {
            // free memory from Maze::Cells
            delete this->grid[i][j];
        }
    }
}

Maze::Cell* Maze::open_cell(Maze::Cell* c) {
    static std::vector<int> order({0, 1, 2, 3});
    // maps left wall in c to right wall in next, etc.
    static std::vector<int> op_map({1, 0, 3, 2});
    // randomize the order to check for neighbours;
    std::random_shuffle(order.begin(), order.end());
    c->visited = true;
    for (int n_ind: order) {
        if (c->neighbours[n_ind] && !(c->neighbours[n_ind]->visited)) {
            // randomly chosen neighbour hasn't been visited
            // beak wall
            c->walls[n_ind] = false;
            c = c->neighbours[n_ind];
            c->walls[op_map[n_ind]] = false;

            return c;
        }
    }    

    return nullptr;
}

void Maze::make_backtrack_maze() {
    /*
    Makes maze with recursive backtracking.
    */
    Maze::Cell* c = this->grid[0][0]; // starting Maze::Cell
    c->is_entry = true;
    for (int i = 0; i != this->n_rows; ++i) {
        for (int j = 0; j != this->n_columns; ++j) {
            while (c) {
                c = open_cell(c);
            }
            c = this->grid[i][j]; 
        }
    }
    // make exit
    this->grid[this->n_rows-1][this->n_columns-1]->is_exit = true;
    // unvisit all Maze::Cells
    for (int i = 0; i != this->n_rows; ++i) {
        for (int j = 0; j != this->n_columns; ++j) {
            this->grid[i][j]->visited = false;
        }    
    }
}