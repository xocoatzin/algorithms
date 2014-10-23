#ifndef STABLE_MATCHING_TOOLS_H
#define STABLE_MATCHING_TOOLS_H

#include <string>
#include <iostream>

#include "datastructures.h"
#include "../common/io.h"

using namespace tools;

namespace stable_matching
{
    class Parser
    {
    public:
        enum Mode
        {
            MEMBERS,
            PREFERENCES,
            IGNORE
        };

        Parser(std::string filename)
        {
            parse(filename);
        }
        void parse(std::string filename)
        {
            std::cout << "Reading [" << filename << "]" << std::endl;

            _members.clear();
            _preferences.clear();

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
                    if (line_low.compare("[members]") == 0)
                    {
                        mode = MEMBERS;
                        continue;
                    }
                    if (line_low.compare("[preferences]") == 0)
                    {
                        mode = PREFERENCES;
                        continue;
                    }
                }

                if (mode == MEMBERS)
                {
                    std::vector<std::string> tokens = tools::split(line, ":");
                    if (tokens.size() == 2)
                    {
                        _members.add(tokens[0], tokens[1]);
                    }
                    else
                    {
                        std::cerr << "Parser: ERROR! Bad format on line: " << line_id << std::endl;
                    }
                    continue;
                }

                if (mode == PREFERENCES)
                {
                    std::vector<std::string> tokens = tools::split(line, ">");
                    if (tokens.size() == 2)
                    {
                        std::vector<std::string> list = tools::split(tokens[1], ",");
                        if (list.size() > 0)
                        {
                            _preferences.add(tokens[0], list);
                        }
                        else
                        {
                            std::cerr << "Parser: ERROR! Bad format on line: " << line_id << " (no preferences?)" << std::endl;
                        }
                    }
                    else
                    {
                        std::cerr << "Parser: ERROR! Bad format on line: " << line_id << std::endl;
                    }
                    continue;
                }
            }
        }

        Members getMembers() { return _members; };
        Members getMembers(std::string filter)
        {
            return _members.copy_filter<Members>(
                [filter]
                (Members::iterator it)
                {
                    return beginsWith(it->first, filter);
                }
            );
        };

        Preferences getPreferences() { return _preferences; };
        Preferences getPreferences(std::string filter)
        {
            return _preferences.copy_filter<Preferences>(
                [filter]
            (Preferences::iterator it)
            {
                return beginsWith(it->first, filter);
            }
            );
        };

    private:
        Members
            _members;
        Preferences
            _preferences;
    };
}

#endif
