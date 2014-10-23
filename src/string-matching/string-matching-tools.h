#ifndef STRING_MATCHING_TOOLS_H
#define STRING_MATCHING_TOOLS_H

#include <string>
#include <vector>
#include "../common/io.h"

namespace string_matching
{

    class KMPPrefixTable :
        public std::vector < unsigned int >
    {
    public:

        KMPPrefixTable(std::string pattern) :
            std::vector < unsigned int >(pattern.size() + 1)
        {
            if (pattern.size() == 0)
            {
                at(0) = 0;
                return;
            }

            for (unsigned int l = 1; l < size(); l++)
            {
                std::string sub = pattern.substr(0, l);

                unsigned int max = 0, iteration = 0;
                auto it = sub.begin();
                it++;
                for (; it != sub.end(); it++)
                {
                    auto target = it;
                    auto origin = sub.begin();

                    unsigned int count = 0;
                    while (target != sub.end() && origin != it)
                    {
                        if (*origin != *target)
                        {
                            break;
                        }

                        count++;
                        target++;
                        origin++;
                    }

                    if (count == sub.size() - iteration - 1 && count > max)
                        max = count;

                    iteration++;
                }

                at(l) = max;
            }
        }
    };

    class BMHTable :
        public std::vector < unsigned int >
    {
    public:
        BMHTable(std::string pattern) :
            std::vector < unsigned int >(0xFF)
        {
            for (auto i = begin(); i != end(); i++) *i = pattern.length();

            unsigned int i = 1;
            for (auto c : pattern)
            {
                at(static_cast<unsigned int>(c)) = i++;
            }
        }
    };

    static void print(std::string str, std::string pattern, unsigned int offset)
    {
        std::cout
            << str << "\n"
            << tools::spaces(offset) << pattern;

        if (str.length() > (offset + pattern.length()))
            std::cout << tools::spaces(str.length() - (offset + pattern.length()));
    }
}

#endif
