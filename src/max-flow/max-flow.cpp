#include <iostream>
#include "lib/graph.hpp"
#include "../common/io.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cerr << "Error! you need to include the input file in the command line" << endl;
        return -1;
    }

    // We first create the graph
    Graph graph(argv[1]);
    cout << "\nThe graph:\n";
    graph.print();

    cout << "\n\n";
    tools::wait();

    Graph::Capacity
        flow = 0;

    // =================================================================================
    // The algorithm starts here!
    // =================================================================================


    // All the information you need is stored in data structure {problem}

    // Example algorithm:
    // ** Find a path from {s} to {t} ** [hint: Graph::Path p = graph.findPath("s", "t");]
    //                                   [     You can print the path with: p.print();]
    // repeat...
    //     ** Find the capacity for the path found ** [hint: Graph::Capacity c = p.findMaxCapacity()]
    //     ** Add current capacity to the total flow ** [hint: flow += c]
    //     ** Augment the path with the capacity we found ** [hint: graph.augment(p, c)]
    //     ** Find a new path ** [hint: p = graph.findPath("s", "t")]
    // ... until we can't find a valid path [hint: p.isValid() == true]
    // end

    cout << "\nThe flow for this graph is: " << flow << "\n\n\n";
    tools::wait();
}
