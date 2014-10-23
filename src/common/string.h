#ifndef TOOLS_STRING_H
#define TOOLS_STRING_H

#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <string>
#include <vector>
#include <sstream>

namespace tools
{
    //checks if a string {str} is prefixed with {prefix}
    static inline bool beginsWith(std::string str, std::string prefix)
    {
        return std::mismatch(prefix.begin(), prefix.end(), str.begin()).first == prefix.end();
    }

    // trim from start
    static inline std::string &ltrim(std::string &s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
    }

    // trim from end
    static inline std::string &rtrim(std::string &s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
    }

    // trim from both ends
    static inline std::string &trim(std::string &s)
    {
        return ltrim(rtrim(s));
    }

    static inline std::vector<std::string> split(std::string str, std::string delimiter)
    {
        std::vector<std::string> tokens;

        std::size_t pos = 0;
        std::string token;
        while ((pos = str.find(delimiter)) != std::string::npos)
        {
            token = str.substr(0, pos);
            if (!token.empty())
                tokens.push_back(trim(token));
            str.erase(0, pos + delimiter.length());
        }
        if (!str.empty())
            tokens.push_back(trim(str));

        return tokens;
    }

    static inline bool match(std::string str, std::string pattern, unsigned int offset = 0)
    {
        unsigned int i = 0;
        bool _match = true;
        for (auto &c : pattern)
        {
            if (str.at(offset + i++) != c)
            {
                _match = false;
                break;
            }
        }

        return _match;
    }

    static inline std::string repeat(std::string str, unsigned int times = 1)
    {
        std::stringstream ss;

        for (unsigned int s = 0; s < times; s++)
            ss << str;

        return ss.str();
    }

    static inline std::string spaces(unsigned int times = 1)
    {
        return repeat(" ", times);
    }
}

#endif
