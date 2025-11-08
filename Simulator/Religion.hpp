#include <string>

namespace openck
{
    struct ReligionGroup
    {
        std::string name;
        int id;
    };

    struct Religion
    {
        ReligionGroup* group;
        int id;
        std::string name;
        float moral_authority;
    };
}
