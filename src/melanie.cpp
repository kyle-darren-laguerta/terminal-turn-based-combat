#include "../header/melanie.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <string>
#include "io.hpp"
#include <ncurses.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Melanie::Melanie(json hero_data):Hero(hero_data) {}

void Melanie::basicAttack(Hero *target_hero, WINDOW *feedback_panel) {
    // Core conditions
    if (stunned) { // Check if the hero is stunned
        io::typePrintAndMoveDown(feedback_panel, "%s", 1, "Melanie is unable to attack");
        stunned -= 1;
        usleep(1500000);

        if (stunned == 0) {
            io::typePrintAndMoveDown(feedback_panel, "%s", 1, "Melanie get back to normal from being stunned");
        }
    } else if (target_hero->untargetable) { // Check if the enemy hero is untargetable
        io::typePrintAndMoveDown(feedback_panel, "%s", 1, "Enemy hero is untargetable");
    } else if (!dist(gen)) { // Check if attack missed base from the accuracy of the attack
        io::typePrintAndMoveDown(feedback_panel, "%s %s", 2, "Melanie shoot ", target_hero->name.c_str());
        usleep(500000);
        io::typePrintAndMoveDown(feedback_panel, "%s", 1, "Attack missed");
    } else {
        float damage_received = target_hero->receivePhysicalDamage(physical_attack, physical_pen);

        io::typePrintAndMoveDown(feedback_panel, "%s %s", 2, "Melanie shoot ", target_hero->name.c_str());
        usleep(500000);
        io::typePrintAndMoveDown(feedback_panel, "%s %s %s %s", 2, target_hero->name.c_str(), " received ", std::to_string(damage_received).c_str(), " physical damage");   
    }

    // Exclusive condition
    if (state == "Camouflage") {
        physical_attack -= passive_damage;
        state = "Normal";
        untargetable = false;
    }
}

void Melanie::skill1(Hero *target_hero, WINDOW *feedback_panel) {
    if (stunned) {
        io::typePrintAndMoveDown(feedback_panel, "%s", 1, "Melanie is unable to camouflage");
        stunned -= 1;

        usleep(1500000);

        if (stunned == 0) {
            io::typePrintAndMoveDown(feedback_panel, "%s", 1, "Melanie get back to normal from being stunned");
        }
    } else {
        io::typePrintAndMoveDown(feedback_panel, "%s %s", 2, "Melanie hallucinates ", target_hero->name.c_str());
        usleep(500000);

        if (target_hero->effectExist("accuracy_reduce_by_rate", [feedback_panel, target_hero](Effects &eff) {
            io::typePrintAndMoveDown(feedback_panel, "%s %s", 2, target_hero->name.c_str(), " is already hallucinated"); // No effect if the skill use again even if it is still active
            usleep(500000);
        })); else {
            target_hero->active_effects.push_back({"accuracy_reduce_by_rate", .3, 3, false});
            io::typePrintAndMoveDown(feedback_panel, "%s %s %s %s", 4, target_hero->name.c_str(), " accuracy for the next three turns goes ", std::to_string(target_hero->temp_accuracy-.1).c_str(), " %");
            usleep(500000);
            io::typePrintAndMoveDown(feedback_panel, "%s %s %s %s", 4, target_hero->name.c_str(), " evasion for the next three turns goes ", std::to_string(target_hero->evasion*100).c_str(), " %");
            usleep(500000);
        } 
    }
}

void Melanie::skill2(Hero *target_hero, WINDOW *feedback_panel) {
    if (stunned) {
        io::typePrintAndMoveDown(feedback_panel, "%s", 1, "Melanie is unable to attack");
        stunned -= 1;

        usleep(1500000);

        if (stunned == 0) {
            io::typePrintAndMoveDown(feedback_panel, "%s", 1, "Melanie get back to normal from being stunned");
        }
    } else {
        // Stunned for only one turn
        target_hero->stunned = 1;

        // This skill deals only 70% of the physical attack
        int damage_received = target_hero->receivePhysicalDamage(physical_attack * 0.7, physical_pen * 0.7);

        io::typePrintAndMoveDown(feedback_panel, "%s %s", 2, "Melanie throw a grenade at ", target_hero->name.c_str());
        usleep(500000);
        io::typePrintAndMoveDown(feedback_panel, "%s %s", 2, target_hero->name.c_str(), "get stunned");
        usleep(500000);
        io::typePrintAndMoveDown(feedback_panel, "%s %s %s %s", 2, target_hero->name.c_str(), " received ", std::to_string(damage_received).c_str(), " physical damage");   
        usleep(500000);
        io::typePrintAndMoveDown(feedback_panel, "%s", 1, "Melanie physical attack increases for the next turn");       
    }
}