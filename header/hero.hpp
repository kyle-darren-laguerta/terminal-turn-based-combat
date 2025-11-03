#ifndef HERO_H
#define HERO_H

#include <cstring>
#include <vector>
#include <random>
#include <ncurses.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct Effects {
    std::string type;
    float amount;
    int duration; // Turn duration
    bool applied;
};

class Hero {
    public:
         // Attributes
        std::string name = "Hero Name Template";
        float hp;
        float hp_regen;
        float mp;
        float mp_regen;
        float physical_attack;
        float magic_attack;
        float physical_defense;
        float magic_defense;
        float physical_pen;
        float magic_pen;
        float speed;
        float accuracy;
        float evasion;
        float critical_chance;
        float critical_damage;
        float exp;
        float exp_cap;
        int level;

        float temp_accuracy;

        // Other data
        std::string state;
        bool untargetable = false;
        unsigned int stunned = 0; // Value is base from the number of turns the hero is stunned
        std::vector<std::string> skill_set;

        // Tools involving randoms
        std::random_device rd;
        std::mt19937 gen;
        std::bernoulli_distribution dist;

        // Temporary-Attributes value
        std::vector<Effects> active_effects;

        Hero(json hero_data);

        void attack(Hero *target_hero);

        float receivePhysicalDamage(float enemy_physical_attack, float enemy_physical_pen);
        float reduceAccuracyByRate(float rate, int number_of_turns);

        void applyEffects();
        void refreshEffects();
        bool effectExist(std::string effect_name, std::function<void(Effects&)> modifyType);

        virtual void basicAttack(Hero *target_hero, WINDOW *feedback_panel);
        virtual void skill1(Hero *target_hero, WINDOW *feedback_panel);
        virtual void skill2(Hero *target_hero, WINDOW *feedback_panel);
};

#endif