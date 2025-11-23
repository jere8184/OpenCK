
#include <map.hpp>

namespace openck::simulator
{

    Terrain pti;
    Terrain ocean;
    Terrain inland_ocean;
    Terrain arctic;
    Terrain farmlands;
    Terrain forest;
    Terrain hills;
    Terrain woods;
    Terrain mountain;
    Terrain impassable_mountains;
    Terrain steppe;
    Terrain plains;
    Terrain jungle;
    Terrain marsh;
    Terrain desert;
    Terrain coastal_desert;

    std::unordered_map<std::string, Terrain*> Terrain::terrain_map = {
        {"pti", &pti},
        {"ocean", &ocean},
        {"inland_ocean", &inland_ocean},
        {"arctic", &arctic},
        {"farmlands", &farmlands},
        {"forest", &forest},
        {"hills", &hills},
        {"woods", &woods},
        {"mountain", &mountain},
        {"impassable_mountains", &impassable_mountains},
        {"steppe", &steppe},
        {"plains", &plains},
        {"jungle", &jungle},
        {"marsh", &marsh},
        {"desert", &desert},
        {"coastal_desert", &coastal_desert}};
}