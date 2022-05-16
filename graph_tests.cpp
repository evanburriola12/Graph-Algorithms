//Evan Burriola

#include <iostream>
#include <sstream>
#include "graph.h"

using std::cout;
using std::endl;

int main() {
    std::cout << "make an empty digraph" << std::endl;
    Graph G;

    std::cout << "add vertices" << std::endl;
    for (size_t n = 0; n < 7; n++) { //adjusted from [1..7] to [0..6]
        G.add_vertex(n);
    }

    std::cout << "add directed edges" << std::endl;
    // G.add_edge(1,2,5);  // 1 ->{5} 2; (edge from 1 to 2 with weight 5)
    // G.add_edge(1,3,3);
    // G.add_edge(2,3,2);
    // G.add_edge(2,5,3);
    // G.add_edge(2,7,1);
    // G.add_edge(3,4,7);
    // G.add_edge(3,5,7);
    // G.add_edge(4,1,2);
    // G.add_edge(4,6,6);
    // G.add_edge(5,4,2);
    // G.add_edge(5,6,1);
    // G.add_edge(7,5,1);
    //ADJUSTED FOR V0
    G.add_edge(0,1,5);  // 1 ->{5} 2; (edge from 1 to 2 with weight 5)
    G.add_edge(0,2,3);
    G.add_edge(1,2,2);
    G.add_edge(1,4,3);
    G.add_edge(1,6,1);
    G.add_edge(2,3,7);
    G.add_edge(2,4,7);
    G.add_edge(3,0,2);
    G.add_edge(3,5,6);
    G.add_edge(4,3,2);
    G.add_edge(4,5,1);
    G.add_edge(6,4,1);

    std::cout << "G has " << G.vertex_count() << " vertices" << std::endl;
    std::cout << "G has " << G.edge_count() << " edges" << std::endl;
    std::cout << std::endl;

    //G.print();

    std::cout << "compute mst path from 2" <<std::endl;
    G.prim(2);

    //G.printMST();

    std::cout << "print minimum spanning paths" <<std::endl;
    for (size_t n = 0; n <= 6; n++) {
        std::cout << "minimum spanning path from 2 to " << n << std::endl;
        std::cout << "  ";
        G.print_path(n);
    }
    std::cout << std::endl;

    std::cout << "compute shortest path from 2" <<std::endl;
    G.dijkstra(2);
    //G.printMST();

    std::cout << "print shortest paths" <<std::endl;
    for (size_t n = 0; n <= 6; n++) {
        std::cout << "shortest path from 2 to " << n << std::endl;
        std::cout << "  ";
        G.print_shortest_path(n);
    }

    Graph Gc = G;

    Gc.remove_edge(4,5);
    Gc.remove_vertex(5);

    std::cout << "V1 -> V2 cost: " << Gc.cost(1,2) << std::endl;

    std::cout << "G copy has " << Gc.vertex_count() << " vertices" << std::endl;
    std::cout << "G copy has " << Gc.edge_count() << " edges" << std::endl;
    std::cout << std::endl;

    std::cout << "compute mst path from 2" <<std::endl;
    Gc.prim(2);

    //G.printMST();

    std::cout << "print minimum spanning paths" <<std::endl;
    for (size_t n = 0; n <= 6; n++) {
        std::cout << "minimum spanning path from 2 to " << n << std::endl;
        std::cout << "  ";
        Gc.print_path(n);
    }
    std::cout << std::endl;

    std::cout << "compute shortest path from 2" <<std::endl;
    Gc.dijkstra(2);
    //G.printMST();

    std::cout << "print shortest paths" <<std::endl;
    for (size_t n = 0; n <= 6; n++) {
        std::cout << "shortest path from 2 to " << n << std::endl;
        std::cout << "  ";
        Gc.print_shortest_path(n);
    }

    return 0;
}
