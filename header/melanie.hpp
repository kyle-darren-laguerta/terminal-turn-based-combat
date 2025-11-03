#ifndef MELANIE_H
#define MELANIE_H

#include "hero.hpp"
#include <cstring>
#include <vector>
#include <ncurses.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Melanie : public Hero {

    public:
        // Exclusive attributes
        int passive_damage = 35;

        Melanie(json hero_data);

        // Inherited Methods
        void basicAttack(Hero *target_hero, WINDOW *feedback_panel);
        void skill1(Hero *target_hero, WINDOW *feedback_panel);
        void skill2(Hero *target_hero, WINDOW *feedback_panel);
};

#endif