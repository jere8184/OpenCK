#include <string>
#include <vector>
#include <set>

namespace openck::simulator
{

    struct Actor;

    struct Condition {};
    struct Effects {};
    
    struct Option
    {
        std::string name;
        std::string text;
        Condition condition;
        Effects effects;
    };
    

    class Event
    {
        std::string name;
        std::string text;
        std::vector<Actor*> actor;
        std::set<Option> options;
        Effects effects;
    };
}