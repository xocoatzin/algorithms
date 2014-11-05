#ifndef SIMPLEX_TOOLS_H
#define SIMPLEX_TOOLS_H

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <cstdlib>
#include "../common/io.h"
#include "../common/string.h"

#define CMAX  10  //max. number of variables in economic function
#define VMAX  10  //max. number of constraints

using namespace tools;
namespace simplex
{
    class Problem
    {
    private:
        double
            TS[CMAX][VMAX];
        unsigned int
            P1,
            P2;

    public:
        enum Objective
        {
            MAXIMIZE =  1,
            MINIMIZE = -1,
            VOID = 0
        }; // R  MAXIMIZE = Y, MINIMIZE = N   //R1 =1 TO MAXIMIZE, =-1 TO MINIMIZE

        Objective
            objective;

        std::vector < double >
            variables;

        std::vector < std::vector < double > >
            constraints;

        bool
            error, //XERR:       BOOLEAN IF TRUE, NO SOLUTION
            isOptimal; // NOPTIMAL

        Problem() :
            P1(0),
            P2(0),
            objective(VOID),
            error(false),
            isOptimal(false)
        {};

        void Prepare()
        {
            unsigned int
                NC = NumberOfConstraints(),
                NV = NumberOfVariables();

            // right size?
            for (unsigned int j = 0; j < constraints.size(); j++)
            {
                if (constraints[j].size() - 1 != NV)
                {
                    objective = VOID;
                    return;
                }
            }

            for (unsigned int J = 1; J <= NV; J++)
            {
                double R2 = variables[J - 1];
                TS[1][J + 1] = R2 * objective;
            }
            TS[1][1] = variables[NV] * objective;

            for (unsigned int I = 1; I <= NC; I++)
            {
                for (unsigned int J = 1; J <= NV; J++)
                {
                    double R2 = constraints[I - 1][J - 1];
                    TS[I + 1][J + 1] = -R2;
                }
                TS[I + 1][1] = constraints[I - 1][NV];
            }

            for (unsigned int J = 1; J <= NV; J++)
                TS[0][J + 1] = J;
            for (unsigned int I = NV + 1; I <= NV + NC; I++)
                TS[I - NV + 1][0] = I;

        }

        unsigned int NumberOfVariables() { return variables.size() -1; } // NV
        unsigned int NumberOfConstraints() { return constraints.size(); } // NC

        void Pivot()
        {
            double RAP, V, XMAX;
            unsigned int
                NC = NumberOfConstraints(),
                NV = NumberOfVariables();
            unsigned int I, J;

            XMAX = 0.0;
            for (J = 2; J <= NV + 1; J++)
            {
                if (TS[1][J] > 0.0 && TS[1][J] > XMAX)
                {
                    XMAX = TS[1][J];
                    P2 = J;
                }
            }

            RAP = 999999.0;
            for (I = 2; I <= NC + 1; I++)
            {
                if (TS[I][P2] >= 0.0)
                    continue;

                V = fabs(TS[I][1] / TS[I][P2]);
                if (V < RAP)
                {
                    RAP = V;
                    P1 = I;
                }
            }

            V = TS[0][P2];
            TS[0][P2] = TS[P1][0];
            TS[P1][0] = V;
        }

        void Formula()
        {
            unsigned int
                NC = NumberOfConstraints(),
                NV = NumberOfVariables();
            unsigned int
                I,
                J;

            for (I = 1; I <= NC + 1; I++)
            {
                if (I != P1)
                {
                    for (J = 1; J <= NV + 1; J++)
                    {
                        if (J != P2)
                            TS[I][J] -= TS[P1][J] * TS[I][P2] / TS[P1][P2];
                    }
                }
            }

            TS[P1][P2] = 1.0 / TS[P1][P2];
            for (J = 1; J <= NV + 1; J++)
            {
                if (J != P2)
                    TS[P1][J] *= fabs(TS[P1][P2]);
            }

            for (I = 1; I <= NC + 1; I++)
            {
                if (I != P1)
                    TS[I][P2] *= TS[P1][P2];
            }
        }

        void Optimize()
        {
            unsigned int I, J;

            unsigned int
                NC = NumberOfConstraints(),
                NV = NumberOfVariables();

            for (I = 2; I <= NC + 1; I++)
                if (TS[I][1] < 0.0)
                    error = true;

            isOptimal = true;
            if (error)
                return;

            for (J = 2; J <= NV + 1; J++)
                if (TS[1][J] > 0.0)
                    isOptimal = false;
        }

        void print()
        {
            unsigned int
                NC = NumberOfConstraints(),
                NV = NumberOfVariables(),
                ww = 7;

            std::cout << "         |   rhs   |";
            for (unsigned int i = 2; i <= NV + 1; i++)
            {
                std::cout
                    << "    x"
                    << std::setprecision(0)
                    << TS[0][i]
                    << "   |";
            }
            std::cout << "\n" << repeat("---------+",(NV + 2)) << "\n";

            for (unsigned int j = 1; j <= NC + 1; j++)
            {
                std::cout
                    << "      "
                    << (j == 1 ? 'z' : 'x')
                    << (j == 1 ? ' ' : (char)('0' + TS[j][0]))
                    << " |";

                for (unsigned int i = 1; i <= NV + 1; i++)
                {
                    bool pivot = P1 == j && P2 == i;
                    std::cout
                        << (pivot ? ">" : " ")
                        << std::setw(ww)
                        << std::fixed
                        << std::setprecision(3)
                        << TS[j][i]
                        << (pivot ? "<" : " ")
                        << "|";
                }
                if (j != NC + 1)
                    std::cout << "\n";
            }
            std::cout << "\n" << repeat("---------+", (NV + 2)) << "\n";
        }

        void result()
        {
            unsigned int I, J;
            unsigned int
                NC = NumberOfConstraints(),
                NV = NumberOfVariables();

            if (error)
            {
                std::cout << "No solution!" << std::endl;
            }
            else
            {
                for (I = 1; I <= NV; I++)
                {
                    for (J = 2; J <= NC + 1; J++)
                    {
                        if (TS[J][0] == 1.0*I)
                            std::cout
                                << "Variable #" << I << ": "
                                << TS[J][1] << std::endl;
                    }
                }
                std::cout
                    << "Function: "
                    << TS[1][1] << std::endl;
            }
        }
};

    class Parser
    {
    public:
        enum Mode
        {
            OBJECTIVE,
            VARIABLES,
            CONSTRAINTS,
            IGNORE
        };

        Parser(std::string filename)
        {
            parse(filename);
        }

        void parse(std::string filename)
        {
            std::cout << "Reading [" << filename << "]" << std::endl;

            Problem &p = problem;

            Reader reader(filename);

            Mode mode = IGNORE;
            std::string line;
            std::size_t line_id;
            while (reader.getLine(line, line_id))
            {
                //mode
                {
                    std::string line_low = line;
                    std::transform(line_low.begin(), line_low.end(), line_low.begin(), ::tolower);
                    if (line_low.compare("[objective]") == 0)
                    {
                        mode = OBJECTIVE;
                        continue;
                    }
                    if (line_low.compare("[variables]") == 0)
                    {
                        mode = VARIABLES;
                        continue;
                    }
                    if (line_low.compare("[constraints]") == 0)
                    {
                        mode = CONSTRAINTS;
                        continue;
                    }
                }

                if (mode == OBJECTIVE)
                {
                    std::string line_low = line;
                    std::transform(line_low.begin(), line_low.end(), line_low.begin(), ::tolower);
                    if (line_low.compare("maximize") == 0)
                        p.objective = Problem::MAXIMIZE;
                    if (line_low.compare("minimize") == 0)
                        p.objective = Problem::MINIMIZE;

                    continue;
                }

                if (mode == VARIABLES)
                {
                    std::vector<std::string> tokens = tools::split(line, ",");
                    if (tokens.size() < 2)
                    {
                        std::cerr << "Parser: ERROR! Bad format on line: " << line_id << std::endl;
                        continue;
                    }

                    for (unsigned int j = 0; j < tokens.size(); j++)
                    {
                        double v = atof(tokens[j].c_str());
                        p.variables.push_back(v);
                    }

                    continue;
                }

                if (mode == CONSTRAINTS)
                {
                    std::vector<std::string> tokens = tools::split(line, ",");
                    if (tokens.size() < 2)
                    {
                        std::cerr << "Parser: ERROR! Bad format on line: " << line_id << std::endl;
                        continue;
                    }

                    std::vector < double > vec;
                    for (unsigned int j = 0; j < tokens.size(); j++)
                    {
                        double v = atof(tokens[j].c_str());
                        vec.push_back(v);
                    }
                    p.constraints.push_back(vec);

                    continue;
                }
            }

            p.Prepare();
        }

        Problem getProblem() { return problem; }

    private:
        Problem
            problem;
    };
}

#endif
