#include <vector>
#include <string>

#ifndef FNC_H
#define FNC_H


void clrscr();
int readInput();
bool gameplay();
void draw_map(std::vector<std::vector<char>> map);
char intro(bool play);
void animate_text(std::string text);
std::vector<std::string> interact(char object, std::vector<std::string> inventory, int x, int y, std::string map_title);
std::vector<std::string> manage_inventory(std::vector<std::string> inventory);
std::vector<std::string> pickup(std::vector<std::string> inv, int x, int y, std::string title);
void use_item(std::string item_name);
void game_over_screen(std::string game_over_text);
void combat(char monster);
bool is_hit(int miss_chance);
bool is_crit(int crit_chance);
int hit_amount(int crit_chance, int damage_max, int damage_min);
std::string use_spell(std::vector<std::string> spellbook);
std::string use_veteran_skill(std::vector<std::string> veteran_skill);
std::string use_scribe_skill(std::vector<std::string> scribe_skill);
void play_sound(std::string sound_name);
void stop_sound();
void npc_interaction(char npc, int x, int y);
bool monster_chance_to_use_skill(char monster_type);
void randomize_garbage();
void animate_text_c(std::string text);

#endif