#ifndef MAZE_SOLVER_H
#define MAZE_SOLVER_H

#include <vector>
#include <utility>
#include "maze_generation.h"



class Graph{
private:
    struct Node{  
        int row, column;
        bool visited;
        std::vector <Node*> links;
        explicit Node(const Maze::Cell& c) :
            row(c.row), column(c.column), visited(false), links({}) {};
    };

    const Maze& maze; // reference to maze
    Node* start; // Node at entrance of maze
    Node* exit; // Node at exit of maze
    // function to make nodes from maze, used in the constructor
    void make_nodes(const Maze&);

    static void delete_nodes(Node* /*start*/); // for destructor
    static void unvisit_nodes(Node* /*start*/); // unvisits all nodes

    std::vector<Node*> solve_graph_paths_recursive(
        Graph::Node* /*start*/);
    std::vector<Graph::Node*> solve_graph_paths();

public:
    explicit Graph(const Maze&);
    ~Graph();
    void mark_path(); // marks path on maze (with o's)
};

#endif