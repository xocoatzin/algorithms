#include <iostream>
#include "simplex-tools.h"

using namespace tools;
using namespace simplex;
using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cerr << "Error! you need to include the input file in the command line" << endl;
        return -1;
    }

    Parser parser(argv[1]);
    Problem problem = parser.getProblem();

    if (problem.objective == Problem::VOID)
    {
        cerr << "Error! There was an error on the formulation of the problem" << endl;
        return -1;
    }

    problem.print();
    tools::wait();

    // =================================================================================
    // The algorithm starts here!
    // =================================================================================


    // All the information you need is stored in data structure {problem}

    // Example algorithm:
    // repeat...
    //     ** Find the next pivot ** [hint: problem.Pivot()]
    //     ** Formulate the problem ** [hint: problem.Formula()]
    //     ** Optimize the problem ** [hint: problem.Optimize()]
    //     ** Print current iteration values ** [hint: problem.print()]
    // ... until the solution is optimal [hint: problem.isOptimal == true]
    // end

    cout << "The results found:" << endl;
    problem.result();
    tools::wait();

    return 0;
}
