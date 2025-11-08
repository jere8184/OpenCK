
#pragma once

#include <string>

namespace openck::scripting
{

    template <typename T, typename Scope, typename Target>
    struct Condition
    {
        Condition(T (Scope::_scope_getter*)(), T (Scope::_target_getter*)()) :
            scope_getter(_scope_getter),
            target_getter(_target_getter)
        {}

        std::string name;

        Scope& enclosing_scope;

        void (Scope::scope_getter*)();
        void (Target::target_getter*)();
    };
}