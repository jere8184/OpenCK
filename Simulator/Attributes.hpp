#pragma once

namespace openck
{
    enum struct AttributesType 
    {
        NOT_SET,
        DIPLOMACY,
        STEWARDSHIP,
        INTRIGUE,
        LEARNING,
        MARTIAL,
        COMBAT_RATING
    };
   
    struct Attributes
    {
        int diplomacy = 0;
        int stewardship = 0;
        int intrigue = 0;
        int learning = 0;
        int martial = 0;
        int combat = 0;

        int diplomacy_penalty = 0;
        int stewardship_penalty = 0;
        int intrigue_penalty = 0;
        int learning_penalty = 0;
        int martial_penalty = 0;
        int combat_penalty = 0;
    };


    enum struct StatType 
    {
        NOT_SET,
        HEALTH,
        FERTILITY,
        AMBITION,
        GREED,
        ZEAL,
        RATIONALITY,
        THREAT_DECAY_SPEED
    };

    struct Stats
    {
        float health = 0;
        float fertility = 0;
        int amabition = 0;
        int greed = 0;
        int zeal = 0;
        int rationality = 0;
        int threat_decay_speed = 0;

        float health_penalty = 0;
        float fertility_penalty = 0;
    };
}
