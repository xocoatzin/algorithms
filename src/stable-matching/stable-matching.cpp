#include <iostream>
#include "stable-matching-tools.h"
#include "datastructures.h"

using namespace tools;
using namespace stable_matching;
using namespace std;

int main(int argc, char *argv[])
{
    // *********************************************************************************
    // Don't modify this section
    typedef Population::Person Person;
    if (argc < 2)
    {
        cerr << "Error! you need to include the input file in the command line" << endl;
        return -1;
    }

    Parser parser(argv[1]);
    Preferences preferences = parser.getPreferences();
    Members members =  parser.getMembers();
    Population population(members, preferences);
    // *********************************************************************************

    // =================================================================================
    // The algorithm starts here!
    // =================================================================================


    // All the information you need is stored in data structure {population}
    // You can retrieve the list of single men: {population.single_men}

    // Example algorithm:
    // For each single man in the list... [hint: population.single_men.size()]
    //     ** Get the next single man ** [hint: Person *single_guy = population.nextSingleMan();]
    //     Repeat...
    //         ** Get the next preference ** [hint: Person *girl = single_guy->nextPreference();]
    //         ** Propose that girl ** [hint: Person::Response response = single_guy->propose(girl);]
    //     ... until someone accepts [hint: response == Person::ACCEPT_PROPOSAL]
    //     ** Show the results ** [hint: population.print()]
    //     ** Wait for key press ** [hint: cin.get()]
    // ... end loop
    // end


    return 0;
}
