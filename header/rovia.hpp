#ifndef ROVIA_H
#define ROVIA_H

#include "hero.hpp"
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Rovia : public Hero {
    public:

        Rovia(json hero_data);

        // std::string getName();
        // vector<std::string> getSkillSet();
};

#endif