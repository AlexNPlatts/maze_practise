#include <iostream>
#include "maze_generation.h"
#include "maze_solver.h"

int main() {
    std::cout << "Making maze...." << std::endl;
    Maze m(5, 5);
    m.make_backtrack_maze();
    std::cout << "Solving maze...." << std::endl;
    Graph g(m);
    g.mark_path();
    std::cout << "done!" << std::endl;
    m.print();

    return 0;
}