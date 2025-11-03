// cd /mnt/c/Users/daja/OneDrive/Documents/Coding\ Projects/Programs/C++/mlbb/
// g++ main.cpp -lnlohmann_json_schema_validator -o main.o

#include "./header/hero.hpp"
#include "./header/melanie.hpp"
#include "./header/rovia.hpp"
#include "./header/utils/io.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdarg>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <ncurses.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int chooseHero(WINDOW *win, std::vector<std::string> hero_list, int hero_count, int player_num);
std::unique_ptr<Hero> initHero(int hero_id, json hero_data);

void clearPanel(WINDOW *win);
int chooseSkill(WINDOW *panel, Hero *hero);
void showStat(WINDOW *panel, Hero *hero);
void displaySkill(WINDOW *panel, Hero *hero);

int main() {
    // Read hero data
    json hero_data;
    std::ifstream JSONFILE("hero_data.json");
    JSONFILE >> hero_data;
    JSONFILE.close();

    // Screen Initialization
    initscr();
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    keypad(stdscr, TRUE);
    nodelay(stdscr, FALSE);
    noecho();            // Disable key echo
    curs_set(0);

    // Environment Attributes
    int win_x, win_y, hero_panel_width, hero_panel_height, dist_from_scr_bound, feedback_panel_height, feedback_panel_width, status_panel_height, status_panel_width;
    getmaxyx(stdscr, win_y, win_x);

    // Title
    io::centerText("Heroes Reborn");
    move(win_y/3, 0);
    io::centerText("VS");

    feedback_panel_height = 9;
    feedback_panel_width = 71;
    
    WINDOW *feedback_panel = newwin(feedback_panel_height, feedback_panel_width, 24, win_x/2-feedback_panel_width/2);
    refresh();
    wborder(feedback_panel, '|', '|', '-', '-', '*', '*', '*', '*');
    wmove(feedback_panel, 1, 1);
    wrefresh(feedback_panel);

    status_panel_height = 30;
    status_panel_width = 28;

    WINDOW *status_panel1 = newwin(status_panel_height, status_panel_width, 3, 0);
    refresh();
    wborder(status_panel1, '|', '|', '-', '-', '*', '*', '*', '*');
    wmove(status_panel1, 1, 1);
    wrefresh(status_panel1);

    WINDOW *status_panel2 = newwin(status_panel_height, status_panel_width, 3, win_x-status_panel_width);
    refresh();
    wborder(status_panel2, '|', '|', '-', '-', '*', '*', '*', '*');
    wmove(status_panel2, 1, 1);
    wrefresh(status_panel2);

    std::vector<std::string> hero_list = {"Melanie", "Rovia"}; // The order of elements should match the case order in initHero()

    // Panel Attributes (Applied for all panels)
    hero_panel_height = 20;
    hero_panel_width = 30;
    dist_from_scr_bound = 30;

    // Panel 1 Configuration
    WINDOW *player1_panel = newwin(hero_panel_height, hero_panel_width, 3, dist_from_scr_bound);
    refresh();
    wborder(player1_panel, '|', '|', '-', '-', '*', '*', '*', '*');
    wmove(player1_panel, 1, 1);
    keypad(player1_panel, true);
    nodelay(player1_panel, false);
    wrefresh(player1_panel);

    // Panel 2 Configuration
    WINDOW *player2_panel = newwin(hero_panel_height, hero_panel_width, 3, win_x-hero_panel_width-dist_from_scr_bound);
    refresh();
    wborder(player2_panel, '|', '|', '-', '-', '*', '*', '*', '*');
    wmove(player2_panel, 1, 1);
    keypad(player2_panel, true);
    nodelay(player2_panel, false);
    wrefresh(player2_panel);

    // chooseHero returns the id of the hero selected
    // hero id is just the index of the hero element in the hero_list vector
    int p1 = chooseHero(player1_panel, hero_list, 2, 1);
    int p2 = chooseHero(player2_panel, hero_list, 2, 2);

    auto hero_ptr1 = initHero(p1, hero_data);
    auto hero_ptr2 = initHero(p2, hero_data);

    Hero *hero1 = hero_ptr1.get();
    Hero *hero2 = hero_ptr2.get();

    clearPanel(player1_panel);
    clearPanel(player2_panel);

    showStat(status_panel1, hero1);
    showStat(status_panel2, hero2);
    
    // Main battle proper
    bool first_player_turn = true;
    int skill_active;
    while (1) {
        showStat(status_panel1, hero1);
        showStat(status_panel2, hero2);

        // Display the skill selection into non-active one each turn
        displaySkill(player1_panel, hero1);
        displaySkill(player2_panel, hero2);

        if (first_player_turn) {
            hero1->applyEffects();
            showStat(status_panel1, hero1);
            skill_active = chooseSkill(player1_panel, hero1);
            clearPanel(feedback_panel);
            switch(skill_active) {
                case 0:
                    hero1->basicAttack(hero2, feedback_panel);
                    break;
                case 1:
                    hero1->skill1(hero2, feedback_panel);
                    break;
                case 2:
                    hero1->skill2(hero2, feedback_panel);
                    break;
                case 3:
                    break;
            }
            first_player_turn = false;
        } else {
            hero2->applyEffects();
            showStat(status_panel2, hero2);
            skill_active = chooseSkill(player2_panel, hero2);
            clearPanel(feedback_panel);
            switch(skill_active) {
                case 0:
                    hero2->basicAttack(hero1, feedback_panel);
                    break;
                case 1:
                    hero2->skill1(hero1, feedback_panel);
                    break;
                case 2:
                    hero2->skill2(hero1, feedback_panel);
                    break;
                case 3:
                    break;
            }
            first_player_turn = true;
        }

        hero1->refreshEffects();
        hero2->refreshEffects();
    }
    
    getch();

    endwin();
}

// This function is the UI for choosing a hero
int chooseHero(WINDOW *win, std::vector<std::string> hero_list, int hero_count, int player_num) {
    int ch, current_choice, y, x;

    getmaxyx(win, y, x);

    current_choice = 0;
    do {
        clearPanel(win);
        io::centerText(win, false, "%s %s %s", 3, "Player ", std::to_string(player_num).c_str(), " Choosing...");
        wmove(win, y/2, 0);
        if (ch == KEY_DOWN) {
            current_choice -= 1;

            if (current_choice < 0) {
                current_choice = hero_count-1;
            }
        } else if (ch == KEY_UP) {
            current_choice += 1;

            if (current_choice >= hero_count) {
                current_choice = 0;
            }
        }

        io::centerText(win, false, "%s", 1, hero_list.at(current_choice).c_str());
    } while ((ch = wgetch(win)) != '\n');
    clearPanel(win);
    io::centerText(win, false, "%s %s %s", 3, "Player ", std::to_string(player_num).c_str(), " Ready!");
    wmove(win, y/2, 0);
    io::centerText(win, false, "%s", 1, hero_list.at(current_choice).c_str());
    usleep(1000000);
    return current_choice;
}

// This function initialize the hero derived class base from the index data of hero
// Unique pointer approach so that the derived class initialized in this function is stored in heap memory
std::unique_ptr<Hero> initHero(int hero_id, json hero_data) {
    // The order of cases should match the elements indices in the hero_list vector initalized in the main function
    switch(hero_id) {
        case 0:
            return std::make_unique<Melanie>(hero_data.at("melanie"));
        case 1:
            return std::make_unique<Rovia>(hero_data.at("rovia"));
        default:
            return nullptr;
    }
}

// This function clears the panel
void clearPanel(WINDOW *win) {
    werase(win);
    wborder(win, '|', '|', '-', '-', '*', '*', '*', '*');
    wmove(win, 1, 1);
    wrefresh(win);
}

// This function display the skill set of the hero to the hero panel
void displaySkill(WINDOW *panel, Hero *hero) {
    int y, x;
    getmaxyx(panel, y, x);

    clearPanel(panel);
    io::centerText(panel, true, "%s", 1, hero->name.c_str());
    wmove(panel, y/2-2, 0);
    for (auto skill : hero->skill_set) {
        io::centerText(panel, true, "%s", 1, skill.c_str());
    }
}

// This function makes the skill set display in the panel active enabling the user to choose one of the skills
int chooseSkill(WINDOW *panel, Hero *hero) {
    int x, y;
    getmaxyx(panel, y, x);

    int ch, current_active;
    current_active = 0;

    do {
        clearPanel(panel);
        io::centerText(panel, true, "%s", 1, hero->name.c_str());
        io::centerText(panel, true, "%s", 1, "|");
        io::centerText(panel, true, "%s", 1, "V");
        wmove(panel, y/2-2, 0);

        if (ch == KEY_DOWN) {
            current_active++;

            if (current_active == 4) {
                current_active = 0;
            }
        } else if (ch == KEY_UP) {
            current_active--;

            if (current_active == -1) {
                current_active = 3;
            }
        }

        int i = 0;
        for (auto skill : hero->skill_set) {
            if (current_active == i) {
                wattron(panel, A_REVERSE);
            }
            io::centerText(panel, true, "%s", 1, skill.c_str());
            wattroff(panel, A_REVERSE);
            i++;
        }
    } while((ch = wgetch(panel)) != '\n');

    return current_active;
}

// This function prints hero stats in the specified hero panel
void showStat(WINDOW *panel, Hero *hero) {
    clearPanel(panel);

    io::printAndMoveDown(panel, "%s %d", 2, "HP: ", (int)hero->hp);
    io::printAndMoveDown(panel, "%s %d", 2, "MP: ", (int)hero->mp);
    io::printAndMoveDown(panel, "%s %d %s %d", 2, "Exp: ", (int)hero->exp, "/", (int)hero->exp_cap);
    io::printAndMoveDown(panel, "%s %d", 2, "Level: ", hero->level);
    io::printAndMoveDown(panel, "%s %d", 2, "Accuracy: ", (int)(hero->temp_accuracy*100));
}
