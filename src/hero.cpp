#include "../header/hero.hpp"
#include <nlohmann/json.hpp>
#include <iostream>
#include <cstring>
#include <random>
#include <ncurses.h>

using json = nlohmann::json;

Hero::Hero(json hero_data) : accuracy(hero_data.at("accuracy").get<float>()), gen(rd()), dist(accuracy) {
    // Initialized attribute base from the data
    name = hero_data.at("name").get<std::string>();
    hp = hero_data.at("hp").get<float>();
    hp_regen = hero_data.at("hp regen").get<float>();
    mp = hero_data.at("mp").get<float>();
    mp_regen = hero_data.at("mp regen").get<float>();
    physical_attack = hero_data.at("physical attack").get<float>();
    magic_attack = hero_data.at("magic attack").get<float>();
    physical_defense = hero_data.at("physical defense").get<float>();
    magic_defense = hero_data.at("magic defense").get<float>();
    physical_pen = hero_data.at("physical penetration").get<float>(); 
    magic_pen = hero_data.at("magic penetration").get<float>();
    speed = hero_data.at("speed").get<float>();
    evasion = hero_data.at("evasion").get<float>();
    critical_chance = hero_data.at("critical chance").get<float>();
    critical_damage = hero_data.at("critical damage").get<float>();
    exp = 0;
    exp_cap = 1200;
    level = 0;

    skill_set = hero_data.at("Skill Set").get<std::vector<std::string>>();

    temp_accuracy = accuracy;
}

void Hero::attack(Hero *target_hero) {
    int damage_received = target_hero->receivePhysicalDamage(physical_attack, physical_pen);

    std::cout << name << " dealt " << damage_received << " physical damage to " << target_hero->name << std::endl;
    std::cout << target_hero->name << " current HP is " << target_hero->hp << std::endl;
}

float Hero::receivePhysicalDamage(float enemy_physical_attack, float enemy_physical_pen) {
    float physical_defense_temp = physical_defense;
    physical_defense_temp -= enemy_physical_pen;
    float damage_received = (enemy_physical_attack * 120) / (120 + physical_defense_temp);
    hp -= damage_received;
    return damage_received;
}

// Apply the effects from the active_effects vectors
void Hero::applyEffects() {
    for (auto &e : active_effects) {
        if (e.type == "accuracy_reduce") {
            if (e.applied == false) {
                temp_accuracy -= e.amount;
                dist.param(std::bernoulli_distribution::param_type(temp_accuracy));
                e.applied = true;
            }
            e.duration--;
        } else if (e.type == "accuracy_reduce_by_rate") {
            if (e.applied == false) {
                temp_accuracy -= temp_accuracy * e.amount;
                dist.param(std::bernoulli_distribution::param_type(temp_accuracy));
                e.applied = true;
            }
            e.duration--;
        }
    }
}

// Turn the attributes to its base value if the effect duration reaches 0
void Hero::refreshEffects() {
    for (auto it = active_effects.begin(); it != active_effects.end(); ) {
        if (it->duration == 0) {
            if (it->type == "accuracy_reduce") {
                temp_accuracy += it->amount;
                dist.param(std::bernoulli_distribution::param_type(temp_accuracy));
            } else if (it->type == "accuracy_reduce_by_rate") {
                temp_accuracy = accuracy;
                dist.param(std::bernoulli_distribution::param_type(temp_accuracy));
            }

            it = active_effects.erase(it); // returns the next iterator
        } else {
            ++it;
        }
    }
}

bool Hero::effectExist(std::string effect_name, std::function<void(Effects&)> modifyType) {
    for (auto &e : active_effects) {
        if (e.type == effect_name) {
            modifyType(e);
            return true;
        }
    }
    return false;
}

void Hero::basicAttack(Hero *target_hero, WINDOW *feedback_panel) {
    std::cout << "Basic Attack";
}

void Hero::skill1(Hero *target_hero, WINDOW *feedback_panel) {
    std::cout << "Skill 1";
}

void Hero::skill2(Hero *target_hero, WINDOW *feedback_panel) {
    std::cout << "Skill 2";
}