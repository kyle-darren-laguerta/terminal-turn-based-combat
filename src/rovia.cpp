#include "../header/rovia.hpp"
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Rovia::Rovia(json hero_data) : Hero(hero_data) {}

// std::string Wanwan::getName() {
//     return Wanwan::name;
// }