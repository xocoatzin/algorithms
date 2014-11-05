#ifndef TOOLS_IO_H
#define TOOLS_IO_H

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <fstream>
#include <string>

#include "string.h"

namespace tools
{
    class Reader
    {
    public:
        std::string filename;
        std::ifstream file;
        std::size_t this_line;

        Reader(std::string _filename) :
            filename(_filename),
            file(_filename.c_str()),
            this_line(0)
        {
            if (!file.is_open())
                std::cerr << "Reader: ERROR! Can't read the input file. Is the filename right?" << std::endl;
        };

        ~Reader()
        {
            if (file.is_open())
                file.close();
        };

        bool getLine(std::string &str, std::size_t &id)
        {
            std::string s;
            while (std::getline(file, s))
            {
                this_line++;
                //is valid
                s = trim(s);
                if (!s.empty() && !beginsWith(s, "#"))
                {
                    str = s;
                    id = this_line;
                    return true;
                }
            }
            file.close();
            return false;
        }
    };

    static void wait()
    {
        std::cout << "Press [ENTER] to continue..." << std::endl;
        std::cin.get();
    }
}

#endif
