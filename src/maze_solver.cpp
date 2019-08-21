#include <vector>
#include <algorithm>
#include <iostream>
#include "maze_generation.h"
#include "maze_solver.h"

Graph::Graph(const Maze& imaze) : maze(imaze) {
    make_nodes(this->maze);
}

Graph::~Graph() {
    // unvisits nodes, necessary for deletion
    unvisit_nodes(this->start);
    // frees node memory
    Graph::delete_nodes(this->start);
}

void Graph::unvisit_nodes(Graph::Node* start) {
    /*
    unvisits all nodes (all visisted nodes must be linked by construction)
    */
    for (Graph::Node* l:start->links) {
        if (l->visited) {
            l->visited = false;
            unvisit_nodes(l);
        }
    }
}

void Graph::delete_nodes(Node* start) {
    /*
    deletes all nodes from tree ends.
    */
    start->visited = true;
    for (Node* l:start->links) {
        if (!l->visited) {
            Graph::delete_nodes(l);
        }
    }
    delete start;
}

void Graph::make_nodes(const Maze& maze) {
    /*
    make a node each time something happens in the maze (i.e. not a corridor).
    */
    std::vector<Node*> above(maze.n_columns, nullptr); // cells above
    Node* left = nullptr; // cell to left
    std::vector<Node*> node_vec;
    for (int i=0; i!=maze.n_rows; ++i) {
        for (int j=0; j!=maze.n_columns; ++j) {
            Maze::Cell* c = maze.grid[i][j];
            /*
             check if it's a node (it's a node if we can make a decision)
            */
            bool corridor = (
                c->walls[0] && c->walls[1] && !c->walls[2] && !c->walls[3]
                ) || (
                c->walls[2] && c->walls[3] && !c->walls[0] && !c->walls[1]
            );
            if (corridor) {
                // can't be a node on a corridor.
                continue;
            }

            Node* n = new Node(*c);
            node_vec.push_back(n);
            // set start and exit nodes for graph
            if (c->is_entry) {
                this->start = n;
            } else if (c->is_exit) {
                this->exit = n;
            }

            if (left) {
                // if we can link left
                left->links.push_back(n);
                n->links.push_back(left);
            }

            if (!c->walls[1]) {
                // if there is something to the left
                left = n;
            } else {
                left = nullptr;
            }

            if (above[j]) {
                // if we can link above
                above[j]->links.push_back(n);
                n->links.push_back(above[j]);
            }

            if (!c->walls[3]) {
                // if there is something below
                above[j] = n;
            } else {
                above[j] = nullptr;
            }
        }
        // new line don't carry left
        left = nullptr;
    }
}

std::vector<Graph::Node*> Graph::solve_graph_paths_recursive(
    Graph::Node* start) {
    /*
    Finds path recursively in breadth first (breath will be first in link list
    if possible). Not very fast, but pretty simple.
    */
    start->visited = true;
    if (start == this->exit) {
        // found the exit, return the end node;
        return {start};
    }
    for (Graph::Node* l:start->links) {
        if (!l->visited) {
            std::vector<Graph::Node*> p = solve_graph_paths_recursive(l);
            if (p.size() != 0) {
                // if any path ends with the exit push this node to path and
                // return new path
                p.push_back(start);
                return p;
            }  
        }
    }
    // no unvisited paths linked to this node contained the exit, return an empty path
    return {};
}

std::vector<Graph::Node*> Graph::solve_graph_paths() {
    // finds a path through the graph using solve_graph_paths_recursive
    std::vector<Graph::Node*> path = Graph::solve_graph_paths_recursive(
        this->start);
    std::reverse(path.begin(), path.end());
    return path;
}

static void swap(int& a, int&b) {
    // swaps int a and b with bitwise xor
    a = a^b;
    b = a^b;
    a = a^b;
}

void Graph::mark_path() {
    // visits all cells along the path
    std::vector<Graph::Node*> path = Graph::solve_graph_paths();
    for (int i=1; i!=path.size(); ++i) {
        Graph::Node* prev = path[i-1];
        Graph::Node* cur = path[i];
        int r_start = prev->row;
        int r_end = cur->row;
        int c_start = prev->column;
        int c_end = cur->column;

        if (r_start==r_end) {
            // change in column
            if (c_start > c_end) {
                // swap order
                swap(c_start, c_end);
            }
            for (int j=c_start; j<=c_end; ++j) {
                this->maze.grid[r_start][j]->visited = true;
            }
        } else {
            if (r_start > r_end) {
                // swap order
                swap(r_start, r_end);
            }
            for (int j=r_start; j<=r_end; ++j) {
                this->maze.grid[j][c_start]->visited = true;
            }   
        }

    }
}