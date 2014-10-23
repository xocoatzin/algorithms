#ifndef STABLE_MATCHING_DATASTRUCTURES_H
#define STABLE_MATCHING_DATASTRUCTURES_H

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <unordered_map>
#include <functional>
#include <stack>
#include <queue>
#include <iomanip>

#include "../common/string.h"
#include "stable-matching-tools.h"

namespace stable_matching
{
    typedef std::string ID;

    template<typename T>
    class TableContainer
    {
    public:
        typedef T Table;
        typedef typename T::key_type Key;
        typedef typename T::mapped_type Type;
        typedef typename T::iterator iterator;
        typedef typename T::const_iterator const_iterator;

        virtual iterator begin() { return table.begin(); }
        virtual iterator end() { return table.end(); }

        virtual void add(Key key, Type name)
        {
            table.insert(typename Table::value_type(key, name));
        };

        virtual Type get(Key id)
        {
        	iterator it = table.find(id);
            if (it != table.end())
                return it->second;
            return Type();
        }

        virtual void clear()
        {
            table.clear();
        };

        virtual void print()
        {};

        virtual std::size_t size() { return table.size(); }

        template<typename K = TableContainer>
        K copy_filter(std::function<bool(iterator)> filter)
        {
            K t;
            for (iterator it = table.begin(); it != table.end(); it++)
            {
                if (filter(it))
                    t.add(it->first, it->second);
            }

            return (t);
        };

    protected:
        Table table;

        virtual TableContainer& self() { return *this; }
    };

    class Members
        : public TableContainer<std::map<std::string, std::string> >
    {
    public:
        void print()
        {
            for (Table::const_iterator it = table.begin(); it != table.end(); it++)
            {
                std::cout << it->first << ": " << it->second << std::endl;
            }
        };

        template<typename T>
        void print(T t)
        {
            t.print(self());
        };

    protected:
        virtual Members& self() { return *this; }
    };

    class Relationships
        : public TableContainer<std::map<std::string, std::string> >
    {
    public:
        bool exist(std::string key1, std::string key2)
        {
            iterator f1 = table.find(key1);
            bool ret = (f1 != table.end() && f1->second.compare(key2) == 0);

            std::cout << key1 << " -?- " << key2 << " >> " << (ret?"yes":"no") << std::endl;

            return ret;
        };

        bool are_single(std::string key1, std::string key2)
        {
            iterator f1 = table.find(key1);
            iterator f2 = table.find(key2);

            return !(f1 != table.end() || f2 != table.end());
        };

        bool is_single(std::string key1, bool print = false)
        {
            bool ret = (table.find(key1) == table.end());
            if(print) std::cout << "Is {" << key1 << "} single? " << (!ret ? "yes" : "no") << std::endl;
            return ret;
        };

        void free(std::string key)
        {
            iterator
                f1 = table.find(key),
                f2;

            if (f1 != table.end())
            {
                f2 = table.find(f1->second);

                std::cout << key << " -x- " << f1->second << std::endl;

                table.erase(f1);
                table.erase(f2);
            }
        };

        void add(std::string key1, std::string key2)
        {
            if (are_single(key1, key2))
            {
                table.insert(Table::value_type(key1, key2));
                table.insert(Table::value_type(key2, key1));

                std::cout << key1 << " <-> " << key2 << std::endl;
            }
            else
            {
                std::cout << key1 << " -x- " << key2 << " Error creating relationship, are they both single?" << std::endl;
            }
        }

        void print()
        {
            for (Table::const_iterator it = table.begin(); it != table.end(); it++)
            {
                if (it->first < it->second)
                    std::cout << it->first << " <-> " << it->second << std::endl;
            }
        };

        void print(Members &m)
        {
            for (Table::const_iterator it = table.begin(); it != table.end(); it++)
            {
                if (it->first < it->second)
                {
                    std::string id = m.get(it->first);
                    std::string name = m.get(it->second);

                    std::cout << id << " <-> " << name << std::endl;
                }
            }
        };
    };

    class Preferences
        : public TableContainer<std::map<std::string, std::vector<std::string> > >
    {
    public:
        void print()
        {
            for (Table::const_iterator it = table.begin(); it != table.end(); it++)
            {
                std::cout << it->first << " -> ";
                for (Type::const_iterator it2 = it->second.begin(); it2 != it->second.end(); it2++)
                {
                    std::cout << "[" << *it2 << "] ";
                }
                std::cout << std::endl;
            }
        };

        void print(Members &m)
        {
            for (Table::const_iterator it = table.begin(); it != table.end(); it++)
            {
                std::cout << m.get(it->first) << " -> ";
                for (Type::const_iterator it2 = it->second.begin(); it2 != it->second.end(); it2++)
                {
                    std::cout << "[" << m.get(*it2) << "] ";
                }
                std::cout << std::endl;
            }
        };
    };


    class Population
    {
    friend class Person;
    private:
        Members
            members;
        Preferences
            pref;
        Relationships
            relations;

    public:
        class Person
        {
            friend class Population;

        public:
            std::queue<std::string> preferences;
            std::vector<std::string> all_preferences;

            std::string id;
            enum Response
            {
                ACCEPT_PROPOSAL,
                REJECT_PROPOSAL
            };

        private:
            std::string couple_id;
            Population& _population;

            Person(std::string _id, std::vector<std::string> _preferences, Population& _pop) :
                all_preferences(_preferences),
                id(_id),
                couple_id(""),
                _population(_pop)
            {
                for (auto &p : _preferences)
                    preferences.push(p);
            };

        public:
            Person* nextPreference()
            {
                if (!preferences.size())
                    return NULL;

                ID id = preferences.front();
                preferences.pop();

                return _population.find(id);
            }

            std::string name()
            {
                return _population.members.get(id);
            }

            Response propose(Person *p)
            {
                if (p)
                    return propose(*p);
                return Response::REJECT_PROPOSAL;
            }

            Response propose(Person &p)
            {
                for (auto &pref : p.all_preferences)
                {
                    if (pref.compare(id) == 0)
                    {
                        //break relationship
                        if (!p.couple_id.empty())
                        {
                            std::cout << _population.members.get(p.id) << " dumps " << _population.members.get(p.couple_id);
                            if (tools::beginsWith(p.couple_id, "m"))
                            {
                                _population.single_men.push(p.couple_id);
                            }
                            else if (tools::beginsWith(p.couple_id, "f"))
                            {
                                _population.single_women.push(p.couple_id);
                            }
                            Person *per = _population.find(p.couple_id);
                            if (per) per->couple_id = "";
                        }
                        else
                        {
                            std::cout << _population.members.get(p.id) << " is single";
                        }

                        p.couple_id = id;
                        couple_id = p.id;
                        return Response::ACCEPT_PROPOSAL;
                    }
                    else if (pref.compare(p.couple_id) == 0)
                    {
                        std::cout << _population.members.get(p.id) << " prefers " << _population.members.get(p.couple_id);
                        return Response::REJECT_PROPOSAL;
                    }
                }
                return Response::REJECT_PROPOSAL;
            };
        };

        std::vector < Person >
            men,
            women;

        std::stack <std::string>
            single_men,
            single_women;

        Population(Members &m, Preferences &p) :
            members(m),
            pref(p)
        {
            for (auto &p : members)
            {
                Person prs(p.first, pref.get(p.first), *this);

                if (tools::beginsWith(p.first, "m"))
                {
                    men.push_back(prs);
                    single_men.push(p.first);
                }
                else if (tools::beginsWith(p.first, "f"))
                {
                    women.push_back(prs);
                    single_women.push(p.first);
                }
            }
        };

        Person* find(std::string id)
        {
            std::vector < Person > *person_list = NULL;
            if (tools::beginsWith(id, "m")) person_list = &men;
            else if (tools::beginsWith(id, "f")) person_list = &women;

            if (person_list)
            {
                for (auto &p : *person_list)
                {
                    if (id.compare(p.id) == 0)
                        return &p;
                }
            }

            return NULL;
        };

        void print(Person &m)
        {
            std::cout << std::setw(8) << members.get(m.id).substr(0, 8) << " :   ";
            for (auto &pr : m.all_preferences)
            {
                std::cout
                    << "[" <<
                    (
                    m.couple_id.compare(pr) == 0 ?
                    "O" : " "
                    )
                    << "] " << std::setw(6) << members.get(pr).substr(0, 6) << " ";
            }
            std::cout << std::endl;
        }

        void print()
        {
            for (auto &m : men) print(m);
            std::cout << std::endl;
            for (auto &m : women) print(m);
        }

        Person* nextSingleMan()
        {
            if (!single_men.size()) return NULL;

            ID id = single_men.top();
            single_men.pop();
            return find(id);
        }

        Person* nextSingleWoman()
        {
            if (!single_women.size()) return NULL;

            ID id = single_women.top();
            single_women.pop();
            return find(id);
        }
    };


}

#endif
