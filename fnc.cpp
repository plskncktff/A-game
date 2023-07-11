#include "fnc.h"
#include <iostream>
#include <windows.h>
#include <mmsystem.h>
#include <conio.h>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <random>

#pragma comment(lib, "winmm.lib")

//base values used in interactions, map traversal, combat, etc.
bool game_over{ false };
bool can_use_item{ false };
bool sword_used{ false };
bool seen_mass{ false };
bool seen_twins{ false };
bool seen_slug{ false };
bool seen_m{ false };
bool seen_hole{ false };
bool seen_book{ false };
bool can_use_magic{ false };
bool can_use_veteran_skills{ false };
bool can_use_scribe_skills{ false };
bool player_turn_skip{ false };
bool monster_turn_skip{ false };
bool used_static{ false };
bool used_water{ false };
bool used_earth{ false };
bool used_fire{ false };
bool used_trauma{ false };
bool used_focus{ false };
bool used_disregard{ false };
bool used_study{ false };
bool used_bloodboil{ false };
bool used_plague{ false };
bool used_collapse{ false };
bool used_entropy{ false };
bool was_crit{ false };
bool action_chosen{ false };
bool used_disregard_c{ false };
bool used_study_c{ false };
bool item_used{ false };
bool has_gate_key{ false };
bool monster_transformed{ false };
bool has_powder{ false };
bool has_fuse{ false };
bool has_flint{ false };
bool explosive_ready{ false };

const int block_bonus{ 1 };

char player_model{};
char reply{};

int x{};
int y{};
int wall_interact{ 0 };
int tree_interact{ 0 };
int water_interact{ 0 };
int tree_color{ 9 };
int player_class_value{};
int player_hp_max{};
int player_hp_current{};
int player_san_max{};
int player_san_current{};
int player_damage_max{};
int player_damage_min{};
int player_damage_bonus{};
int player_damage_bonus_duration{};
int player_defense_base{};
int player_defense_total{};
int player_defense_bonus{};
int player_defense_bonus_duration{};
int player_miss_chance_max{};
int player_miss_chance_bonus{};
int player_miss_chance_bonus_duration{};
int player_crit_chance{};
int player_bleed_duration{ 0 };
int monster_hp_max{ 0 };
int monster_hp_current{ 0 };
int monster_damage_max{ 0 };
int monster_defense{ 0 };
int monster_miss_chance{ 0 };
int monster_crit_chance{ 0 };
int monster_damage_min{ 0 };
int total_damage{ 0 };
int defense_penalty{ 0 };
int miss_chance_increase{ 0 };
int damage_penalty{ 0 };
int defense_debuff_time{ 0 };
int miss_chance_debuff_time{ 0 };
int damage_debuff_time{ 0 };
int runes{ 0 };
int bloodboil_duration{ 0 };
int plague_duration{ 0 };
int player_plague_duration{ 0 };
int entropy_duration{0};
int player_entropy_duration{ 0 };
int monster_count_abdv{ 6 };

std::string game_over_text;
std::string tree_talk{};
std::string skill_selected{};
std::string player_title{};
std::string monster_count_t_abdv{"six"};

std::mt19937 mt{ std::random_device{}() };

std::chrono::milliseconds frame = std::chrono::milliseconds(45);
std::chrono::milliseconds long_frame = std::chrono::milliseconds(600);
std::chrono::milliseconds pause_after_text = std::chrono::milliseconds(1000);

std::vector<std::vector<char>> current_map{};
std::string current_map_title;
std::vector<std::string> inventory;
std::vector<std::string> spellbook;
std::vector<std::string> veteran_skill;
std::vector<std::string> scribe_skill;


//maps are drawn here
//x - items
//^ - trees
//~ - water
//A-Z - NPCs
//M - player Mage
//W - player Warrior
//R - player Rogue
// | and -  - walls
//(, ), and = - building walls
//*, %, and & (so far) - enemy types
std::vector<std::vector<char>> abandoned_village
{
		  /*0*//*1*//*2*//*3*//*4*//*5*//*6*//*7*//*8*//*9*//*0*//*1*//*2*//*3*//*4*//*5*//*6*//*7*//*8*//*9*//*0*/
	/*0*/{ '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-' },
	/*1*/{ '|', '^', '^', '^', '^', '^', '^', '^', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '^', '|' },
	/*2*/{ '|', '^', ' ', ' ', ' ', ' ', 'x', '^', ' ', '^', '^', ' ', '^', ' ', '^', ' ', '^', ' ', ' ', '^', '|' },
	/*3*/{ '|', '^', '(', '=', '=', ')', ' ', '^', ' ', '^', '^', '^', '^', '^', '^', '^', ' ', '^', ' ', '^', '|' },
	/*4*/{ '|', '^', '(', 't', ' ', ')', ' ', ' ', ' ', ' ', ' ', ' ', '^', ' ', '^', ' ', ' ', '^', 'x', ' ', '|' },
	/*5*/{ '|', ' ', '(', ' ', ' ', ')', ' ', '^', '^', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '^', '^', ' ', '|' },
	/*6*/{ '|', ' ', '(', '=', ' ', ')', ' ', ' ', '(', '=', '=', '=', '=', '=', '=', ')', ' ', '^', ' ', ' ', '|' },
	/*7*/{ '|', ' ', ' ', ' ', ' ', ' ', ' ', '^', '(', 't', ' ', ' ', ' ', 'x', 'x', ')', ' ', ' ', ' ', '^', '|' },
	/*8*/{ '|', ' ', '^', ' ', ' ', ' ', ' ', '^', '(', ' ', ' ', ' ', ' ', ' ', 'x', ')', ' ', '^', '^', '^', '|' },
	/*9*/{ '|', ' ', '^', '^', ' ', ' ', '^', '^', '(', ' ', ')', '=', '=', '=', '=', ')', ' ', ' ', ' ', '^', '|' },
	/*0*/{ '|', ' ', ' ', '^', '^', ' ', '^', '^', '(', ' ', ')', '^', '^', '^', '^', ' ', ' ', '^', ' ', ' ', ' ' },
	/*1*/{ '|', '^', ' ', '^', '^', ' ', ' ', '^', '(', '@', ')', '^', ' ', '^', ' ', ' ', '^', '^', '^', ' ', '|' },
	/*2*/{ '|', ' ', ' ', '^', '^', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '^', '(', '=', ')', '|' },
	/*3*/{ '|', '^', '^', ' ', ' ', ' ', '^', ' ', '^', ' ', ' ', ' ', ' ', '^', ' ', ' ', '^', '(', 't', ')', '|' },
	/*4*/{ '|', '^', '(', '=', '=', ')', '^', '^', '^', ' ', '^', '^', ' ', '^', ' ', ' ', ' ', '(', ' ', ')', '|' },
	/*5*/{ '|', '^', '(', 'x', ' ', ')', '^', '^', '^', ' ', '^', '^', '^', ' ', ' ', ' ', ' ', '#', ' ', ')', '|' },
	/*6*/{ '|', '^', '(', ' ', ' ', ')', '^', '^', ' ', ' ', '^', '^', ' ', ' ', ' ', ' ', ' ', '(', ' ', ')', '|' },
	/*7*/{ '|', '^', '(', ' ', ' ', ')', '^', '^', '^', ' ', ' ', '^', '^', '^', ' ', ' ', ' ', '(', ' ', ')', '|' },
	/*8*/{ '|', '^', '(', '=', ' ', ')', '^', '^', '^', '^', '*', 't', '^', ' ', ' ', ' ', ' ', '(', 'x', ')', '|' },
	/*9*/{ '|', '^', '^', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '^', '^', '^', '|' },
	/*0*/{ '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '<', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-' },
};
std::vector<std::vector<char>> silent_pond
{
		  /*0*//*1*//*2*//*3*//*4*//*5*//*6*//*7*//*8*//*9*//*0*//*1*//*2*//*3*//*4*//*5*//*6*//*7*//*8*//*9*//*0*//*1*//*2*/
	/*0*/{ '-', '-', '-', '-', '-', '-', '-', '-', '@', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-' },
	/*1*/{ '|', '^', '^', '^', '^', ' ', ' ', ' ', ' ', ' ', '^', ' ', ' ', '^', '^', ' ', '^', ' ', '(', '=', '=', ')', '|' },
	/*2*/{ '|', '^', ' ', ' ', ' ', ' ', ' ', ' ', '^', ' ', ' ', ' ', '^', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'M', ')', '|' },
	/*3*/{ '|', '^', ' ', '^', ' ', ' ', ' ', ' ', '^', ' ', ' ', ' ', '^', ' ', ' ', '~', ' ', ' ', '(', ' ', ' ', ')', '|' },
	/*4*/{ '|', '^', ' ', '^', '^', ' ', ' ', ' ', ' ', '~', ' ', ' ', ' ', ' ', '~', '~', ' ', ' ', '(', ' ', ' ', ')', '|' },
	/*5*/{ '|', ' ', ' ', '^', ' ', ' ', ' ', '~', '~', '~', '~', '~', ' ', '~', '~', '~', '~', ' ', '(', '=', '=', ')', '|' },
	/*6*/{ '|', ' ', '^', ' ', ' ', ' ', ' ', ' ', ' ', '~', '~', '~', ' ', ' ', '~', '~', '~', ' ', ' ', ' ', ' ', '^', '|' },
	/*7*/{ '|', ' ', ' ', ' ', ' ', ' ', '~', ' ', '~', '~', '~', '~', '~', ' ', ' ', '~', '~', '~', ' ', '^', '^', ' ', '|' },
	/*8*/{ '|', ' ', '^', '^', ' ', '~', '~', ' ', '~', '~', '~', '~', '~', '^', ' ', '^', '~', ' ', ' ', ' ', '^', ' ', '|' },
	/*9*/{ '|', ' ', ' ', '^', ' ', ' ', '~', ' ', '~', '~', '~', '~', '~', '~', ' ', ' ', '~', '~', '~', ' ', '^', ' ', '|' },
	/*0*/{ '|', ' ', ' ', ' ', ' ', ' ', '~', '~', '~', '~', '~', '~', '~', '~', '~', ' ', '^', '^', '~', ' ', ' ', ' ', '|' },
	/*1*/{ ' ', ' ', ' ', ' ', ' ', ' ', '~', '~', '~', '~', '~', '~', '~', '~', '~', ' ', 't', '^', '~', ' ', ' ', '^', '|' },
	/*2*/{ '|', ' ', ' ', ' ', ' ', '~', '~', '~', '~', '~', '~', '~', '~', ' ', ' ', ' ', '~', '~', '~', ' ', '^', ' ', '|' },
	/*3*/{ '|', ' ', ' ', ' ', ' ', '~', ' ', ' ', '~', '~', '~', '~', '~', ' ', '~', '^', '~', ' ', '~', ' ', ' ', ' ', '|' },
	/*4*/{ '|', ' ', ' ', ' ', '~', '~', '^', ' ', '~', '~', '~', '~', '~', '~', '~', '~', '~', ' ', ' ', ' ', ' ', ' ', '|' },
	/*5*/{ '|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '~', '~', '~', '~', '~', '~', '~', '~', ' ', ' ', ' ', '^', ' ', ' ', '|' },
	/*6*/{ '|', ' ', ' ', '^', ' ', ' ', '^', '^', '~', '~', '~', '~', ' ', ' ', ' ', '~', '~', ' ', ' ', '^', '^', ' ', '|' },
	/*7*/{ '|', ' ', ' ', '^', ' ', ' ', '^', ' ', '~', '~', '~', '~', '~', ' ', ' ', '~', '~', '~', ' ', ' ', '^', '^', '|' },
	/*8*/{ '|', ' ', '^', '^', ' ', ' ', ' ', ' ', '~', '~', '~', '~', '~', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '^', '|' },
	/*9*/{ '|', ' ', '^', '^', '^', ' ', ' ', ' ', ' ', '~', '~', '~', '~', ' ', ' ', ' ', ' ', '^', '^', ' ', ' ', '^', '|' },
	/*0*/{ '|', ' ', '^', '^', '^', '^', ' ', '^', ' ', ' ', '~', '~', '~', ' ', ' ', '^', '^', '^', '^', ' ', ' ', '^', '|' },
	/*1*/{ '|', '^', '^', '^', ' ', ' ', ' ', '^', '^', ' ', ' ', '~', '~', '~', ' ', '^', ' ', '^', '^', ' ', '^', '^', '|' },
	/*2*/{ '|', '^', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '^', '^', '|' },
	/*3*/{ '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-' }
};
std::vector<std::vector<char>> graveyard
{
	      /*0*//*1*//*2*//*3*//*4*//*5*//*6*//*7*//*8*//*9*//*0*//*1*//*2*//*3*//*4*//*5*//*6*//*7*//*8*//*9*//*0*//*1*//*2*/
	/*0*/{ '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-' },
	/*1*/{ '|', '^', '^', '^', ' ', ' ', ' ', ' ', ' ', ' ', '*', '^', '_', '^', 'x', '^', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
	/*2*/{ '|', ' ', ' ', ' ', ' ', '^', '^', ' ', '^', '^', ' ', '^', '+', '^', ' ', '^', '^', '^', ' ', '^', '^', ' ', '|' },
	/*3*/{ '|', ' ', '^', 'x', '^', '+', '_', '^', '^', '^', ' ', '^', '+', '^', ' ', ' ', '&', '^', ' ', '_', '^', ' ', '|' },
	/*4*/{ '|', ' ', '^', '^', '+', '+', '_', '+', '^', '^', ' ', '^', '+', '^', ' ', '^', ' ', '^', ' ', '+', '^', '%', '|' },
	/*5*/{ '|', ' ', '&', '+', '+', '+', '_', '_', '_', '^', ' ', '^', '_', '^', ' ', '^', ' ', '^', ' ', '+', '^', ' ', '|' },
	/*6*/{ '|', '^', ' ', '_', '+', '+', '+', '+', '+', ' ', ' ', '^', '^', '^', ' ', '^', 'x', '^', ' ', '_', 'x', ' ', '|' },
	/*7*/{ '|', '^', ' ', '+', '+', '_', '_', '+', '+', '^', ' ', '^', 'x', '^', ' ', '^', '^', '^', ' ', '^', '^', '^', '|' },
	/*8*/{ '|', ' ', ' ', '^', '+', '_', '+', '_', '^', '^', ' ', '^', ' ', '^', ' ', '^', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
	/*9*/{ '|', ' ', '^', '^', '_', '+', '+', '^', 'x', ' ', ' ', ' ', ' ', ' ', '*', ' ', ' ', '^', '^', '^', '^', ' ', '|' },
	/*0*/{ '|', ' ', '^', '^', '^', '_', ' ', '^', '^', '^', ' ', '^', '^', '^', '+', '^', '^', '^', '^', '^', '^', ' ', '|' },
	/*1*/{ '|', ' ', ' ', '^', '^', ' ', ' ', '_', '^', '^', ' ', '^', '+', '+', '_', '+', '_', '+', '+', '+', '_', ' ', '<' },
	/*2*/{ '|', ' ', '^', '^', '^', ' ', '+', '^', ' ', '^', ' ', '^', '+', '^', '^', '^', '^', '^', '^', '^', '^', ' ', '|' },
	/*3*/{ '|', '%', ' ', 'x', '^', ' ', '^', 'x', ' ', ' ', ' ', '^', '_', 'x', '^', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|' },
	/*4*/{ '|', '^', '^', '^', '^', ' ', '^', '^', ' ', '^', '^', '^', '+', ' ', '^', ' ', '^', '^', '^', ' ', '^', '^', '|' },
	/*5*/{ '|', '^', ' ', ' ', ' ', ' ', '^', '^', '%', '^', '^', '^', '^', ' ', '^', ' ', '^', '^', '^', ' ', '^', '^', '|' },
	/*6*/{ '|', '^', ' ', '^', '^', ' ', ' ', ' ', ' ', '^', ' ', ' ', ' ', ' ', '^', ' ', '^', '+', '^', ' ', '^', 'x', '|' },
	/*7*/{ '|', '^', ' ', '^', '^', ' ', ' ', ' ', ' ', '^', ' ', '^', '^', ' ', '^', ' ', '^', '_', '^', ' ', '^', ' ', '|' },
	/*8*/{ '|', 'x', ' ', '^', '^', '^', '^', '^', ' ', '^', ' ', '^', '^', ' ', '^', ' ', '^', '_', '^', ' ', '^', ' ', '|' },
	/*9*/{ '|', '^', ' ', ' ', '^', 'x', '^', 'x', ' ', '^', ' ', '^', '^', ' ', '^', ' ', '^', '_', '+', '%', '^', ' ', '|' },
	/*0*/{ '|', '^', '^', ' ', '^', ' ', '^', ' ', ' ', '^', '%', '^', ' ', ' ', ' ', '*', '^', '+', '^', ' ', ' ', ' ', '|' },
	/*1*/{ '|', '+', '^', ' ', '^', ' ', '^', '^', '^', '^', ' ', '^', ' ', '^', '^', '^', '^', '_', '^', ' ', ' ', ' ', '|' },
	/*2*/{ '|', '^', '^', ' ', '^', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '&', '^', '^', '^', '^', '^', '^', '^', '^', '^', '|' },
	/*3*/{ '-', '-', '-', ' ', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-' }
};


void stop_sound()
{
	PlaySound(0, 0, 0);
}


void play_sound(std::string sound_name)
{
	stop_sound();
	if (sound_name == "Theme 1")
	{
		PlaySound(TEXT("sounds/Theme 1.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	}
	else if (sound_name == "Theme 2")
	{
		PlaySound(TEXT("sounds/Theme 2.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	}
	else if (sound_name == "Theme 3")
	{
		PlaySound(TEXT("sounds/Theme 3.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	}
	else if (sound_name == "Theme 4")
	{
		PlaySound(TEXT("sounds/Theme 4.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	}
	else if (sound_name == "Theme 3 long")
	{
		PlaySound(TEXT("sounds/Theme 3 long.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	}
	else if (sound_name == "Battle theme")
	{
		PlaySound(TEXT("sounds/Battle theme.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	}
	else if (sound_name == "NPC theme")
	{
		PlaySound(TEXT("sounds/NPC theme.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	}
	else if (sound_name == "Can't open gates")
	{
		PlaySound(TEXT("sounds/Can't open gates.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}
	else if (sound_name == "Gate open")
	{
		PlaySound(TEXT("sounds/Gate open.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}
	else if (sound_name == "Select")
	{
		PlaySound(TEXT("sounds/Select.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}
	else if (sound_name == "Step")
	{
		PlaySound(TEXT("sounds/Step.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}
	else if (sound_name == "Pages")
	{
		PlaySound(TEXT("sounds/Pages.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}
	else if (sound_name == "Pile")
	{
		PlaySound(TEXT("sounds/Pile.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}
	else if (sound_name == "Fleshy gate")
	{
		PlaySound(TEXT("sounds/Fleshy gate.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}
	else if (sound_name == "Flint strike")
	{
		PlaySound(TEXT("sounds/Flint strike.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}
	else if (sound_name == "Fuse")
	{
		PlaySound(TEXT("sounds/Fuse.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}
	else if (sound_name == "Explosion")
	{
		PlaySound(TEXT("sounds/Explosion.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}
}

char intro(bool play)
{
	bool intro{false};
	bool class_chosen{ false };
	bool item_chosen{ false };
	char player_class{};

	

	if (play)
	{
		play_sound("Theme 2");
		clrscr();
		animate_text("The land has known nothing but bloodshed since the dawn of creation.\nChildren are born and raised only to strenghten the numbers, then be trampled into blood-soaked soil.\nIn the neverending grinding of this whetstone of war, savage commanders seize power - then fall, grand tales are told - then forgotten.");
		clrscr();
		animate_text("Although numbers are thinned quick, armies do not dwindle.\nFor a reason that interests none, mothers bear many children. Families are forced to provide for tens of mouths.\nThis leads to hunger. Hunger leads to kin devouring kin.\nAnd yet, being born alone is considered a curse - a tradition that originated from an ancient tale of the world's creation.");
		clrscr();
		animate_text("What unseen demiurge guides the hand cutting down an enemy, setting fire to a village, butchering a child?\nNone of the souls living in this world question this. None dwell on the meanings.\nUrge to survive takes away this privilege.\nThere are primal needs to be quenched.");
		clrscr();
		animate_text("You are born as a sole child.\nYou have never seen your father - long dead in an unmarked grave, amongst unknown fodder.\nYour mother is trialed as a cursed soul, sentenced to burn.\nYou never know care, never attached to anything.\nYou are to struggle to your very last breath.");
		clrscr();
		animate_text("The villages such as the one you're born into are common in these battle-scorched wastelands.\nFew broken veterans, some farmers trying to provide for themselves.\nAll in fear of plunder, famine, curse.\nNo purpose weighs down on your shoulders. None to demand you to have one.\nYou take on whatever work you're told to do - nothing more, nothing less.");
		clrscr();
		animate_text("A peculiarly dressed man visits the village one day.\nThe man is tall, he is thin, he charms people around him.\nHis eyes meet yours and a spark of unexpected curiosity flickers in them.\nThe man brings about marvelous tales of Gods.\nHe tells of how they fell one by one - bitter fruits of a cosmic conflict.\nMuch of what the man tells you about provokes no stirring of imagination.\nSuch is the way of a mute, deaf, and blind flotsam stranded in the outer reaches of existence.\nSuch is your way.");
		clrscr();
		animate_text("Though the man's words resonate not, your attention is drawn to several items placed before him.\nThe man demonstrates the items, telling how each is a symbol for many facets of the Gods.\nYou see a torn battle standard driven into the mud. Despite the sturdy handle, it stands lame and uncertain.\nNext to it on a table is a ritual bowl with several bags in it. The contents of the bags are never shown.\nAside from the bowl is a dagger with a chipped point. The double-edged blade is sure to cut a misfortunate wielder.");
		animate_text("\nThe items all stir awe in you, but what catches your eye the most is...");
		std::cout << "\n\n1. The standard\n2. The bowl\n3. The dagger";

		while (!item_chosen)
		{
			switch (readInput())
			{
			case 49:
				item_chosen = true;
				player_class = 'V';
				can_use_veteran_skills = true;
				veteran_skill.push_back("Relive trauma");
				veteran_skill.push_back("Focus anger");
				veteran_skill.push_back("Disregard limitations");
				veteran_skill.push_back("Study enemy");
				inventory.push_back("Bandages");
				player_hp_max = 50;
				player_san_max = 20;
				player_damage_max = 8;
				player_damage_min = 4;
				player_miss_chance_max = 15;
				player_crit_chance = 20;
				player_defense_base = 2;
				player_model = 'V';
				player_title = "Veteran";

				break;
			case 50:
				item_chosen = true;
				player_class = 'C';
				can_use_magic = true;
				spellbook.push_back("Summon static elemental");
				spellbook.push_back("Summon water elemental");
				spellbook.push_back("Summon earth elemental");
				spellbook.push_back("Summon fire elemental");
				inventory.push_back("Rune of stillness");
				inventory.push_back("Bandages");
				player_hp_max = 35;
				player_san_max = 50;
				player_damage_max = 4;
				player_damage_min = 2;
				player_miss_chance_max = 35;
				player_crit_chance = 15;
				player_defense_base = 1;
				player_model = 'C';
				player_title = "Conjurer";
				break;
			case 51:
				item_chosen = true;
				player_class = 'R';
				scribe_skill.push_back("Bloodboil rune");
				scribe_skill.push_back("Plague rune");
				scribe_skill.push_back("Collapse rune");
				scribe_skill.push_back("Entropy rune");
				inventory.push_back("Bandages");
				can_use_scribe_skills = true;
				player_hp_max = 40;
				player_san_max = 30;
				player_damage_max = 6;
				player_damage_min = 3;
				player_miss_chance_max = 20;
				player_crit_chance = 30;
				player_defense_base = 1;
				player_model = 'R';
				player_title = "Runescribe";
				break;
			default:
				item_chosen = false;
			}
		}

		player_hp_current = player_hp_max;
		player_san_current = player_san_max;
		
		clrscr();
		animate_text("The man concluded his tales with the mention of a rumor he'd heard long ago.\nThat not all Gods fell.\nThat they are still there, blurred in the Cosmos, weaving the fates of the stars.\nThe man's voice sounds rigid, when he mentions this rumor. It's as if he's not pleased with this thought.\nThe man then opened up a shop in the village's outskirts, but you never went to see the trinkets he sold.");

		if (player_class == 'V')
		{
			clrscr();
			animate_text("What has drawn your attention in the man's tale foretold much of what became of your life.\nSoon after the man left your village, a group of soldiers led by a heavily armored knight came to recruit new fodder.\nWhat you lacked in mental capabilities you made up for with tireless manual labour in the fields.\nThis made you a perfect candidate to be enlisted.\nYou were given basic combat training and thrown into battle days after being taken from your village.\nIn the coming years, you've seen the faces of war - brutality, depravity, fear.\nThe wars were meaningless to you, and so were they to any other involved in any way. Though you never pretended otherwise.\nYears pass, scars pile up. Both inside your mind, and on your body.\nThough severely traumatised by the unending horrors of the battlefield, you gained some tactical ability and a fine control over your fighting instincts.");
		}
		else if (player_class == 'C')
		{
			clrscr();
			animate_text("What has drawn your attention in the man's tale foretold much of what became of your life.\nSoon after the man left your village, a group of mages led by a figure in a four-colored robe came to recruit new apprentices.\nWhat you lacked in corporeal capabilities you made up for with inquisitive mind.\nThis made you a perfect candidate to be enlisted.\nYou were given basic conjuration instructions and thrown into battle days after being taken from your village.\nIn the coming years, you've seen the faces of war - brutality, depravity, fear.\nThe wars were meaningless to you, and so were they to any other involved in any way. Though you never pretended otherwise.\nYears pass, scars pile up. Both inside your mind, and on your body.\nThough shaken by the revelations exposed by your conjurations, you gained sufficient mastery of the elements.");
		}
		else if (player_class == 'R')
		{
			clrscr();
			animate_text("What has drawn your attention in the man's tale foretold much of what became of your life.\nSoon after the man left your village, a group of mercenaries led by a masked man came to recruit new brigands.\nWhat you lacked in body and mind you made up for with cunning and intuition.\nThis made you a perfect candidate to be enlisted.\nYou were taught the basic runecarving techniques and thrown into battle days after being taken from your village.\nIn the coming years, you've seen the faces of war - brutality, depravity, fear.\nThe wars were meaningless to you, and so were they to any other involved in any way. Though you never pretended otherwise.\nYears pass, scars pile up. Both inside your mind, and on your body.\nThough shaken by constantly gambling on your life, you honed your instincts and learned some useful curses.");
		}
		clrscr();
		animate_text("Despite being provided with basic needs, you never found your purpose.\nWith no meaning in this senseless meddling, pursuing some vain goal seemed excessive.\nYou had no interest in anything aside from meeting the next sunrise.\n");
		clrscr();
		animate_text("Still, there would arise a meaning to your existence.");
		clrscr();
		animate_text("One night, you find yourself on a battlefield, surrounded by familiar sounds of clashing steel and agonized cries.\nAs you are stalking through the battle formations searching for valuable targets, an enemy soldier crosses your path...");
		combat('H');
		clrscr();
		play_sound("Theme 2");
		animate_text("You make your way through the battlefield, rushing to the enemy formation's center.\nYou seek to end the fight as soon as possible by taking the enemy general's head.\nRight as you're about the break through the guardsmen, you hear a trumpet off to the west.\nThe enemy came prepared.\nYou realize that the odds disfavor your side of the conflic significantly.\nLoyalty has never been one of your strongest traits, so you cut your losses and make hasty way to the nearby forest.\nDodging sword slashes, cannon ball blasts, arrows, and spells, you realize that the fortune seems to be on your side alone today.\n");
		clrscr();
		animate_text("Without so much as being scratched by the enemy, you get to the thick bushes guarding the ominous looking trees.\nYou run through the gloomy overgrowth for as long as you can, and you never look back.");
		clrscr();
		animate_text("After some time, you come across an odd scenery.\nThere's a wall in the middle of the forest.\nThe wall is covered in moss and fallen leaves, and it's too hight to see what's on the other side.\nYou decide that it's better to take your chances being caught as a burglar than to get taken as a war prisoner.\nYou look for an opening in the wall, and soon enough you find it.\nOne section of the wall seems to be broken through up to your waist's level.\nAn unlit gap is staring at you like a wound dealt by an unknown assailant.");
		clrscr();
		animate_text("You take a short glance back - the sounds of retreat and pursuit are getting closer. Yet nobody is on your tracks for now.\nYou are alone.\nYou hesitate for a moment, then crawl into the hole.");
		clrscr();
		animate_text("You reach the other side of the wall after few moments.\nAs you're crawling out, you bump into some rock oddly sticking out from the others.\nThe rock falls out and seems eager to take others with it, as other pieces of the wall start filling the crawlspace quickly.\nA cloud of dust emerges, and the hole is closed.\nIt doesn't seem to be possible to get back into the hole, and you can't climb over the wall - it's too smooth.\nWith no way out, you take a look at your surroundings...");

		stop_sound();
	}


	else
	{
		clrscr();
		std::cout << "You are a:\n";
		std::cout << "\n1. Veteran\n2. Conjurer\n3. Runescribe\n";

		while (!class_chosen)
		{
			switch (readInput())
			{
			case 49:
				play_sound("Select");
				class_chosen = true;
				player_class = 'V';
				can_use_veteran_skills = true;
				veteran_skill.push_back("Relive trauma");
				veteran_skill.push_back("Focus anger");
				veteran_skill.push_back("Disregard limitations");
				veteran_skill.push_back("Study enemy");
				inventory.push_back("Bandages");
				player_hp_max = 50;
				player_san_max = 20;
				player_damage_max = 8;
				player_miss_chance_max = 15;
				player_crit_chance = 20;
				player_defense_base = 2;
				player_model = 'V';
				player_title = "Veteran";
				break;

			case 50:
				play_sound("Select");
				class_chosen = true;
				player_class = 'C';
				can_use_magic = true;
				spellbook.push_back("Summon static elemental");
				spellbook.push_back("Summon water elemental");
				spellbook.push_back("Summon earth elemental");
				spellbook.push_back("Summon fire elemental");
				inventory.push_back("Rune of stillness");
				inventory.push_back("Bandages");
				player_hp_max = 35;
				player_san_max = 50;
				player_damage_max = 4;
				player_miss_chance_max = 35;
				player_crit_chance = 15;
				player_defense_base = 1;
				player_model = 'C';
				player_title = "Conjurer";
				break;

			case 51:
				play_sound("Select");
				class_chosen = true;
				player_class = 'R';
				scribe_skill.push_back("Bloodboil rune");
				scribe_skill.push_back("Plague rune");
				scribe_skill.push_back("Collapse rune");
				scribe_skill.push_back("Entropy rune");
				inventory.push_back("Bandages");
				can_use_scribe_skills = true;
				player_hp_max = 40;
				player_san_max = 30;
				player_damage_max = 6;
				player_miss_chance_max = 20;
				player_crit_chance = 30;
				player_defense_base = 1;
				player_model = 'R';
				player_title = "Runescribe";
				break;
			}
		}
		player_hp_current = player_hp_max;
		player_san_current = player_san_max;
	}
	intro = false;
	return player_class;
}

void game_over_screen(std::string game_over_text)
{
	clrscr();
	if (game_over_text == "trees")
	{
		animate_text("As you fall to the ground, twigs and roots entagle your body.\nLeaves fall over you, as insects start digging into your flesh.\nDecomposition follows soon after; your body is rotting rapidly.\nThrough your sleep, you feel everything.\nYou agonize while being tossed across vast greenlands of overgrown thickets.\nWith your body dead, you are forever trapped in the neverending forest, constantly brutalized by the pain of decomposition.");
		clrscr();
		std::cout << "Game over.\n\nPress Enter to exit.";
		std::cin.get();
	}
	else if (game_over_text == "sword suicide")
	{
		animate_text("You die of extensive blood loss.\nYour blood will surely extend this forsaken place's reach.\nYour sacrifice will not have been in vain.");
		clrscr();
		std::cout << "Game over.\n\nPress Enter to exit.";
		std::cin.get();
	}
	else if (game_over_text == "lump")
	{
		animate_text("You are knocked down to the ground - lacerated and bleeding.\nThe mass hovers over you and starts descending slowly.\nIt lowers its clawed protuberances until the tips begin to slowly puncture your chest.\nIts actions are precise, and there's no place for hesitation or hastiness in this performance.\nThe mass lowers itself until its bottom is pressing against a large part of your upper body.\nThe claws dig deeper and start twisting as if burrowing, while your bones are snapped and ground.\nYou faint.\nAfter some time, you regain your consciousness.\nYou discover that you are but an eye now without any authority, madly rolling in your socket, rubbing agains other unfortunates like yourself.\nAfter some time, you give in to desperate madness...");
		clrscr();
		std::cout << "Game over.\n\nPress Enter to exit.";
		std::cin.get();
	}
	else if (game_over_text == "slug")
	{
		animate_text("You fall to your knees, coughing blood streaming to your mouth from the wound in your neck.\nYou feel cold, bloodloss sets in quickly.\nAs your body starts going numb, you feel the creature's gelatinous body enveloping you.\nIt burns, as if you're dragged into boiling water.\nThe pain rapidly increases, so you scream.\nThe acidic mass starts flowing down your throat, mixed with blood, scorching your insides.\nMoments later your consciousness snaps, and the shock from the pain ends you.\nYour remains will be regurgitated by the creature, making it stronger.");
		clrscr();
		std::cout << "Game over.\n\nPress Enter to exit.";
		std::cin.get();
	}
	else if (game_over_text == "twins")
	{
		animate_text("As the creature's blow drops you to the ground, it jumps to you, grabs you by your hands.\nYou try breaking its grasp, but your efforts are futile.\nThe creature's heads regard you for a moment, their faces emotionless, stale.\nYou then feel your arms twisted until both elbows are dislocated, then your forearms snap.\nThe creature then does the same to your legs.\nYou groan and whimper as sharp pain pierces you.\nThe creature then picks you up, it's hands pressed agains your ribs.\nWithout regarding you this time, the creature presses its hands together.\nYour ribs crack, splinter, pierce your organs.\nYou cannot breathe because of the pain.\nThe creature drops you down to the ground without any care.\nAs you draw your last breath, the creature places its hands agains your face and starts squeezing its thumbs in your eye sockets.\nBefore you could start screaming, the creature's fingers reach your brain.");
		clrscr();
		std::cout << "Game over.\n\nPress Enter to exit.";
		std::cin.get();
	}
	else if (game_over_text == "sanity loss combat m")
	{
		animate_text("Suddenly, you realise how laughable this conflict is.\nAs if pulled from under the water by an unseen hand, you see the situation clearly for the first time.\nWhy fight this enemy, when it's simply trying to communicate?\nYou feel that you need to embrace the enemy and amend for your wrongdoings.\nYou drop your weaponry and step closer to the enemy, a crooked smile slashed across your face devoid of emotion.\nAs the enemy is having its way with you, you feel a gaze - unbearably heavy - pressuring you from somewhere out of this plane of existence.\nThe beholder is pleased with your failure.");
		clrscr();
		std::cout << "Game over.\n\nPress Enter to exit.";
		std::cin.get();
	}
	else if (game_over_text == "sanity loss exploration")
	{
		animate_text("As your sanity finally collapses, panic overcomes you.\nYou sence impending doom as shivers go through your body in waves.\nYou start screaming at the terror ravagin in your consciousness.\nYou feel that the earth itself is devouring you. You fall, and you keep falling for all eternity.\nAlthough the fall is precipitated by the absence of reality beneath your feet, there's still something there.\nThere's a final depth you are going to.\nAnd the depth is not uninhabited.");
		clrscr();
		std::cout << "Game over.\n\nPress Enter to exit.";
		std::cin.get();
	}
	else if (game_over_text == "sanity loss combat h")
	{
		animate_text("The years of conflict and horrors of war finally claim your mind.\nYou are overcome with paranoia, fear, and hallucinations.\nAs you struggle to tell apart reality and delusion, your enemy strikes you down with no particular emotions.");
		clrscr();
		std::cout << "Game over.\n\nPress Enter to exit.";
		std::cin.get();
	}
	else if (game_over_text == "blown")
	{
		animate_text("You place the pouch inbetween the boulders.\nYou step away and start conjuring the fire elemental.\nAs the elemental forms, molten chunks start flying towards the pouch, nearly hitting it.\nNone of those hit the pouch, to your relief.\nThe elemental finishes forming, and moment after that, a fiery fury rains down on the rocks, which ignites the pouch.");
		play_sound("Explosion");
		animate_text("\nThe reaction is so violent, the powder explodes immediately, sending huge slabs of stone flying your way.\nOne of the slabs hits you in the face, slicing your head off in the process.\nYou weren't able to even feel the impact, nor realise that you have just perished.");
		clrscr();
		std::cout << "Game over.\n\nPress Enter to exit.";
		std::cin.get();
	}
	else if (game_over_text == "drown")
	{
		animate_text("The water calls to you.\nThe world is no longer present.\nThere's nothing but the water and what lies beneath it.\nYou are to return to your progenitor.\nYou are to become the missing part.\nYou cease to be, the deluge washes you over and dissolves you into nothingness.");
		clrscr();
		std::cout << "Game over.\n\nPress Enter to exit.";
		std::cin.get();
	}
	else if (game_over_text == "demo-end")
	{
		play_sound("Theme 4");
		animate_text("The passage is cleared. The way ahead is there.\nYet you hesitate. You look behind.\nThe graveyard is still, lit by moonlight.\nYou know not what waits you in this cave, but there is no turning back now.\nYou step into the darkness, hand outstretched.\nThe wheel of cosmic fortune turns once more.\nThere's something calling to all who come to this place.\nA dreamer to be awoken. An offering to be consumed. A purpose to be fulfilled.\nIt's not clear which of these beckons to you, but you will find that out eventually.\nThere's an end to all things, and so must even the most unclear of fates be resolved.");
		clrscr();
		std::cout << "Demo end.\n\nThank you for playing!\n\nPress Enter to exit.";
		std::cin.get();
	}
}

bool monster_chance_to_use_skill(char monster_type)
{
	int chance{};
	bool result = false;

	if (monster_type == '*')
	{
		chance = 5;
	}
	
	else if (monster_type == '&')
	{
		chance = 6;
	}

	else if (monster_type == '%')
	{
		chance = 5;
	}

	std::uniform_int_distribution chance_to_use{ 1, chance };
	
	if (chance_to_use(mt) == 1)
	{
		result = true;
	}

	return result;
}


bool is_hit(int miss_chance)
{
	bool result = true;
	std::uniform_int_distribution chance_to_miss{ 0, 99 };
	if (chance_to_miss(mt) < miss_chance)
	{
		result = false;
	}
	return result;
}

bool is_crit(int crit_chance)
{
	bool result = false;
	std::uniform_int_distribution chance_to_crit{ 0, 99 };
	if (chance_to_crit(mt) < crit_chance)
	{
		result = true;
	}
	return result;
}

int hit_amount(int crit_chance, int damage_max, int damage_min)
{
	int hit_amount{ 0 };

	std::uniform_int_distribution damage{ damage_min, damage_max };

	hit_amount = damage(mt);
	if (is_crit(crit_chance))
	{
		if (hit_amount == 1)
		{
			++hit_amount;
		}
		hit_amount *= 2;
		was_crit = true;
	}

	return hit_amount;
}


std::string use_scribe_skill(std::vector<std::string> scribe_skill)
{
	int instability_max{};
	bool result{ false };
	std::string skill_used{};
	std::uniform_int_distribution instability_check{0, 99};

	clrscr();
	std::cout << "\n	Runes:\n\n";
	std::cout << "1. " << scribe_skill[0] << " - 1 rune. stable: 1dmg/turn for 3 turns; unstable: also 2 dmg to scribe." << std::endl;
	std::cout << "2. " << scribe_skill[1] << " - 1 rune. stable: 1dmg/turn for 3 turns, if has other curses; unstable: also 1 dmg/turn to scribe." << std::endl;
	std::cout << "3. " << scribe_skill[2] << " - 2 runes. stable: all curses deal dmg immediately; unstable: also dmg to scribe." << std::endl;
	std::cout << "4. " << scribe_skill[3] << " - 3 runes. stable: 3-1 dmg/turn based on hp; unstable: 3-1san.dmg/turn to scribe." << std::endl;

	switch (readInput())
	{

		//bloodboil rune (DOT 1/tick, low instability)
	case 49:
		if (runes > 0)
		{
			skill_used = scribe_skill[0];
			instability_max = 9;
			if (instability_check(mt) <= instability_max)
			{
				skill_used += " unstable";
			}
			clrscr();
			animate_text_c(scribe_skill[0] + " used.");
		}
		else
		{
			clrscr();
			animate_text_c("You don't have enough runes carved.");
			skill_used = "Runes";
		}

		break;

		//plague rune (DOT 1/tick if has other DOTs, low instability)
	case 50:
		if (runes > 0)
		{
			skill_used = scribe_skill[1];
			instability_max = 9;
			if (instability_check(mt) <= instability_max)
			{
				skill_used += " unstable";
			}
			clrscr();
			animate_text_c(scribe_skill[1] + " used.");
		}
		else
		{
			clrscr();
			animate_text_c("You don't have enough runes carved.");
			skill_used = "Runes";
		}
		break;

		//collapse rune (combine the DOTs to deal immediate damage, medium instability)
	case 51:
		if (runes > 0)
		{
			skill_used = scribe_skill[2];
			instability_max = 14;
			if (instability_check(mt) <= instability_max)
			{
				skill_used += " unstable";
			}
			clrscr();
			animate_text_c(scribe_skill[2] + " used.");
		}
		else
		{
			clrscr();
			animate_text_c("You don't have enough runes carved.");
			skill_used = "Runes";
		}
		break;

		//entropy rune (hp-based DOT, high instability)
	case 52:
		if (runes > 1)
		{
			skill_used = scribe_skill[3];
			instability_max = 19;
			if (instability_check(mt) <= instability_max)
			{
				skill_used += " unstable";
			}
			clrscr();
			animate_text_c(scribe_skill[3] + " used.");
		}
		else
		{
			clrscr();
			animate_text_c("You don't have enough runes carved.");
			skill_used = "Runes";
		}
		break;

	}
	return skill_used;
}


std::string use_veteran_skill(std::vector<std::string> veteran_skill)
{
	std::string skill_used{};

	clrscr();
	std::cout << "\n	Tactics:\n\n";
	std::cout << "1. " << veteran_skill[0] << " - -3 san, +2 def for 3 turns." << std::endl;
	std::cout << "2. " << veteran_skill[1] << " - -4 san, +2 max.dmg for 3 turns." << std::endl;
	std::cout << "3. " << veteran_skill[2] << " - guaranteed hit + 5 bonus dmg, usable once per battle." << std::endl;
	std::cout << "4. " << veteran_skill[3] << " - -10% miss chance for 3 turns, usable once per battle." << std::endl;


	switch (readInput())
	{

		//relive trauma (lowers sanity, increases defense)
	case 49:
		if (player_san_current > 3)
		{
			clrscr();
			animate_text_c(veteran_skill[0] + " used.");
			clrscr();
			if (!used_trauma)
			{
				animate_text("Your vision tunnels.\nPast encounters flash before your eyes in quick succession.\nEnemies, comrades, civilians - torn to pieces, burnt, shot, stabbed; lands scorched, lives obliterated.\nA grizzly display of brutality.\nPast a certain threshold, you snap.\nYour mind collapses to a single directive - prevent intrusions at all costs.\nYou adopt a more tense, closed posture, no openings for the enemy to take advantage of.\nDefense increased!");
			}
			else
			{
				animate_text_c("Your guard tightens as your mind shuts in.\nDefense increased!");
			}
			skill_used = veteran_skill[0];
		}
		else
		{
			clrscr();
			animate_text_c("Your mind is too strained, you cannot focus.");
			skill_used = "Mind";
		}
		break;

		//focus anger (lowers sanity, increases damage)
	case 50:
		if (player_san_current > 4)
		{
			clrscr();
			animate_text_c(veteran_skill[1] + " used.");
			clrscr();
			if (!used_focus)
			{
				animate_text("You clench your teeth until they start audibly grinding.\nYour knuckles are white, and the hilt of your weapon almost cracks under pressure.\nYour breathing becomes deep, hasty.\nSpite overcomes you, and your only instinct is to kill whatever it is in front of you.\nTear it to shreds, rip it aparat, batter it until none is left to fight back.\nYour strikes will surely go through any obstacles, now that such madness dwells within you.\nDamage increased!");
			}
			else
			{
				animate_text_c("Your blood boils with guided rage.\nDamage increased!");
			}
			skill_used = veteran_skill[1];
		}
		else
		{
			clrscr();
			animate_text_c("Your mind is too strained, you cannot focus.");
			skill_used = "Mind";
		}
		break;

		//disregard limitations (damage to self, double damage to enemy, guaranteed hit, usable once per battle)
	case 51:
		if (!used_disregard_c && player_hp_current > 5)
		{
			clrscr();
			animate_text_c(veteran_skill[2] + " used.");
			clrscr();
			if (!used_disregard)
			{
				animate_text("You focus your breathing and let go of any subconscious barriers set within yourself by nature.\nYou wait for when the moment is right, for all the limits to be erased.\nIn a split second, you twist your body head to toe with blinding speed, guiding your weapon towards the enemy.\nBlood spurts from your eyes, nose, from under your fingernails, but your enemy had no time to even percieve the strike.\nBonus damage to enemy at the cost of your own health!");
			}
			else
			{
				animate_text_c("You command your body to forego caution - the force of your blow cuts both ways.\nBonus damage to enemy at the cost of your own blood!");
			}
			skill_used = veteran_skill[2];
		}
		else
		{
			clrscr();
			animate_text_c("Your body needs to recover to go beyond limit again.");
			skill_used = "Body";
		}
		break;

		//study enemy (lowers miss chance, usable once per battle)
	case 52:
		if (!used_study_c)
		{
			clrscr();
			animate_text_c(veteran_skill[3] + " used.");
			clrscr();
			if (!used_study)
			{
				animate_text("Summoning dueling expertise, you observe your enemy.\nTheir movements are being sorted, meticulously picked apart.\nThe enemy's movements become familiar, slow, predictable.\nAfter some moments, you are able to tell almost exactly what your enemy will do next.\nThe enemy is rendered unable to properly react to your strikes.\nMiss chance decreased!");
			}
			else
			{
				animate_text_c("You study your enemy's movements.\nMiss chance decreased!");
			}
			skill_used = veteran_skill[3];
		}
		else
		{
			clrscr();
			animate_text_c("You've already mastered this enemy's patterns.");
			skill_used = "None";
		}
		break;

	}
	return skill_used;
}

std::string use_spell(std::vector<std::string> spellbook)
{
	std::string spell_used{};

	clrscr();
	std::cout << "\n	Spellbook:\n\n";
	std::cout << "1. " << spellbook[0] << " - 6 san; moderate dmg + stun for 1 turn." << std::endl;
	std::cout << "2. " << spellbook[1] << " - 2 san; minor dmg + incresed miss chance." << std::endl;
	std::cout << "3. " << spellbook[2] << " - 2 san; minor dmg + decreased dmg." << std::endl;
	std::cout << "4. " << spellbook[3] << " - 10 san; heavy dmg + decreased def." << std::endl;

	switch (readInput())
	{
		//Static - moderate damage + stun
	case 49:
		clrscr();
		if (player_san_current > 6)
		{
			animate_text_c(spellbook[0] + " used.");
			spell_used = spellbook[0];

			if (!used_static)
			{
				clrscr();
				animate_text("Your mind strains, and the heavy air gathers in front of you, dimmed.\nA poorly defined shape resembling human body emerges out of the crackling mist.\nIn an instant, an electric arc forms between the elemental's arm and the enemy's body.\nThe smell of burning flesh reaches your nostrils.\nYou feel nauseous, and an extreme vertigo overcomes you.\nYou feel your sanity vaining.\nThe enemy is stunned and cannot act!\n");
				used_static = true;
			}
			else
			{
				clrscr();
				animate_text_c("Static elemental strikes the enemy with lightning.\nYour head spins wildly.\nThe enemy is stunned and cannot act!\n");
			}
		}
		else
		{
			animate_text_c("Your mind is too weak to cast this conjuration.");
			spell_used = "Mind";
		}
		break;

		//Water - minor damage + miss chance increase
	case 50:
		clrscr();
		if (player_san_current > 2)
		{
			animate_text_c(spellbook[1] + " used.");
			spell_used = spellbook[1];
			if (!used_water)
			{
				clrscr();
				animate_text("As you focus, the ground you stand on dries up.\nA fluidly moving, slick form manifests in front of you.\nA slenderly built elemental with feminine features swirls in anticipation of action.\nDespite its playful demeanor, the elemental's movements are sharp, resembling a whip ready to split open skin.\nThe elemental places its hands close together and forms a small bubble of water in-between.\nIn an instant, pressurized water starts slicing at the enemy.\nMost of the damage is being done to the creatur's eyes.\nThe enemy is nearly blind!");
				used_water = true;
			}
			else
			{
				clrscr();
				animate_text_c("Water elemental whips the enemy's eyes with pressurized water.\nThe enemy is almost blind!");
			}
		}
		else
		{
			animate_text_c("Your mind is too weak to cast this conjuration.");
			spell_used = "Mind";
		}
		break;

		//Earth - minor damage + damage reduction
	case 51:
		clrscr();
		if (player_san_current > 2)
		{
			animate_text_c(spellbook[2] + " used.");
			spell_used = spellbook[2];
			if (!used_earth)
			{
				clrscr();
				animate_text("You concentrate, and the earth between you and the enemy rumbles.\nAfter some delay, the ground splits and a stony hand of granite and soil reaches up.\nA heavy, bulky frame appears between you and the enemy.\nUnmovingly calm, the elemental regards the enemy with no apparent conclusions.\nYou form a telepathic command, and the elemental immediately claps its palms together.\nCountless pebbles and stones begin rolling and flying towards the enemy.\nAlthough not enough to crush the horrid monstrosity, the stones now weigh heavily on its appendages.\nThe enemy is significanly encumbered!");
				used_earth = true;
			}
			else
			{
				clrscr();
				animate_text_c("Earth elemental slams rocks into the enemy.\nThe enemy is significantly encumbered!");
			}
		}
		else
		{
			animate_text_c("Your mind is too weak to cast this conjuration.");
			spell_used = "Mind";
		}
		break;

		//Fire - heavy damage + defense reduction
	case 52:
		clrscr();
		if (player_san_current > 10)
		{
			animate_text_c(spellbook[3] + " used.");
			spell_used = spellbook[3];
			if (!used_fire)
			{
				clrscr();
				animate_text("Your mind becomes tangibly hot with molten anger.\nA maddening blaze sparks in your eyes, while waves of hot air oscillate around you.\nWhile the heat restricts the enemy in its terrifying assault, lava erupts from the gound.\nIn a glorious display of scorching brilliance, a fire elemental breaks free from the nothingness' hold.\nWithout wasting a moment, it brings all its rage down on the abomination in front of it.\nSkin is peeled off with heat, bones melted, and the enemy's blood boils and evaporates.\nThe enemy is defenseless!");
				used_fire = true;
			}
			else
			{
				clrscr();
				animate_text_c("Fire elemental ignites the enemy.\nThe enemy is defenseless!");
			}
		}
		else
		{
			animate_text_c("Your mind is too weak to cast this conjuration.");
			spell_used = "Mind";
		}
		break;
	}
	return spell_used;
}

void combat(char monster)
{
	play_sound("Battle theme");
	//class-specific resource reset
	runes = 7;

	//DOT duration resets
	bloodboil_duration = 0;
	plague_duration = 0;

	//monster intro text + stats definition
	if (monster == '*')
	{
		clrscr();
		if (!seen_mass)
		{
			animate_text("You come across a lump of flesh and skin hovering above ground.\nIt seems to be floating in the same position without any ability to comprehend its surroundings.\nAs you take a step closer, it twitches.\nIt quickly turns around at various angles until a cluster of eyes is revealed on its surface, facing you.\nThe eyes are wide open, moving rapidly in the sockets.\nAfter violently rotating for a few seconds, the mass' eyes all find their target.\nSeveral fleshy appendages ending in serrated claws erupt from inside the mass.\nThe enemy starts accelerating towards you with claws swinging seemingly at random.\nYou brace yourself before this abomination.");
			seen_mass = true;
		}
		else
		{
			animate_text_c("A writhing mass of flesh, skin, and eyes floats menacingly towards you.");
		}
		monster_hp_max = 25;
		monster_hp_current = monster_hp_max;
		monster_damage_max = 7;
		monster_damage_min = 2;
		monster_defense = 0;
		monster_crit_chance = 15;
		monster_miss_chance = 35;
	}
	else if (monster == 'H')
	{
		clrscr();
		animate_text_c("Another misfortunate soul stands in your way.");
		monster_hp_max = 15;
		monster_hp_current = monster_hp_max;
		monster_damage_max = 3;
		monster_damage_min = 1;
		monster_defense = 0;
		monster_crit_chance = 5;
		monster_miss_chance = 25;
	}
	else if (monster == '&')
	{
		if (!seen_slug)
		{
			clrscr();
			animate_text("You notice a slime-like pile of transparent mass, slithering across the ground slowly.\nAs you try to carefully side-step the thing, it slowly turns its attention to you, extending a tendril in your direction.\nIt seems to be studying you, taking note of your image.\nSuddenly, the tendril pulls inward, hiding under the moist surface.\nIn its place, a growth starts forming rapidly, sliding upwards, then bending in a loop.\nYou see that the insides of this abhorrent slug are made of human bones, innards, and what looks like congealed bodily fluids.\nThe toop of the loop opens to a crevice filled withe splindered bones, that trembles slightly.\nThe slug's mouth emits a hissing noise, readying to tear flesh out of you.");
			seen_slug = true;
		}
		else
		{
			clrscr();
			animate_text_c("A slug made of human remains is hissing at you.");
		}
		monster_hp_max = 35;
		monster_hp_current = monster_hp_max;
		monster_damage_max = 3;
		monster_damage_min = 1;
		monster_defense = 0;
		monster_crit_chance = 5;
		monster_miss_chance = 15;
	}
	else if (monster == '%')
	{
		if (!seen_slug)
		{
			clrscr();
			animate_text("You see a two-headed figure, broad in its frame covered in brown faded cloth - broad sleeves showing tips of sharp claws.\nAlthough it seems heavily built at first, you notice that it's arms and legs are slender, branch-like.\nIt doesn't seem like it would be hard to break such a creature, but its eyes betray perception, preparedness.\nWithout making a sound, the creature lowers its heads, and bends its legs slightly, arms raised.\nIts posture is now steady, closed. It's hard to find any openings in it.\nThe creature seems to be waiting for you to make a move. It's taking its time evaluating your abilities.\nYour fingers tremble. This creature will not be easy to take down.");
			seen_slug = true;
		}
		else
		{
			clrscr();
			animate_text_c("A two-headed figure stands before you, carefully anticipating your moves.");
		}
		monster_hp_max = 30;
		monster_hp_current = monster_hp_max;
		monster_damage_max = 6;
		monster_damage_min = 2;
		monster_defense = 1;
		monster_crit_chance = 10;
		monster_miss_chance = 10;
	}

	//combat loop
	while (monster_hp_current > 0 && player_hp_current > 0 && player_san_current > 0 && !game_over)
	{
		//player action reset
		action_chosen = false;

		//monster stun reset
		monster_turn_skip = false;

		//debuff resets
		if (defense_debuff_time == 0)
		{
			defense_penalty = 0;
		}

		if (miss_chance_debuff_time == 0)
		{
			miss_chance_increase = 0;
		}

		if (damage_debuff_time == 0)
		{
			damage_penalty = 0;
		}

		//buff resets
		if (player_defense_bonus_duration == 0)
		{
			player_defense_bonus = 0;
		}

		//damage calculation reset
		total_damage = 0;

		//player defense calculation reset
		player_defense_total = player_defense_base;

		//combat menu
		while (!action_chosen)
		{
		clrscr();
		std::cout << "The moment is silent with anticipation.\n\n";

		//monster debuffs display
		if (defense_debuff_time > 0)
		{
			std::cout << "Enemy defense is lowered by " << defense_penalty << " for " << defense_debuff_time << " turn"; if (defense_debuff_time > 1) { std::cout << "s"; } std::cout << "." << std::endl;
		}
		if (damage_debuff_time > 0)
		{
			std::cout << "Enemy damage is lowered by " << damage_penalty << " for " << damage_debuff_time << " turn"; if (damage_debuff_time > 1) { std::cout << "s"; } std::cout << "." << std::endl;
		}
		if (miss_chance_debuff_time > 0)
		{
			std::cout << "Enemy chance to miss is increased by " << miss_chance_increase << " for " << miss_chance_debuff_time << " turn"; if (miss_chance_debuff_time > 1) { std::cout << "s"; } std::cout << "." << std::endl;
		}

		//DOTs display
		if (bloodboil_duration > 0)
		{
			std::cout << "Enemy blood is boiling for " << bloodboil_duration << " turn"; if (bloodboil_duration > 1) { std::cout << "s"; } std::cout << "." << std::endl;
		}
		if (plague_duration > 0)
		{
			std::cout << "Enemy is riddled with disease for " << plague_duration << " turn"; if (plague_duration > 1) { std::cout << "s"; } std::cout << "." << std::endl;
		}
		if (entropy_duration > 0)
		{
			std::cout << "Enemy life is being drained for " << entropy_duration << " turn"; if (entropy_duration > 1) { std::cout << "s"; } std::cout << "." << std::endl;
		}
		if (player_plague_duration > 0)
		{
			std::cout << player_title << " is riddled with disease for " << player_plague_duration << " turn"; if (player_plague_duration > 1) { std::cout << "s"; } std::cout << "." << std::endl;
		}
		if (player_entropy_duration > 0)
		{
			std::cout << player_title << "'s mind is being ravaged for " << player_entropy_duration << " turn"; if (player_entropy_duration > 1) { std::cout << "s"; } std::cout << "." << std::endl;
		}
		if (player_bleed_duration > 0)
		{
			std::cout << player_title << " is bleeding for " << player_bleed_duration << " turn"; if (player_bleed_duration > 1) { std::cout << "s"; } std::cout << "." << std::endl;
		}

		//player buffs display
		if (player_defense_bonus_duration > 0)
		{
			std::cout << player_title << "'s defense increased by " << player_defense_bonus << " for " << player_defense_bonus_duration << " turn"; if (player_defense_bonus_duration > 1) { std::cout << "s"; } std::cout << "." << std::endl;
		}
		if (player_damage_bonus_duration > 0)
		{
			std::cout << player_title << "'s damage increased by " << player_damage_bonus << " for " << player_damage_bonus_duration << " turn"; if (player_damage_bonus_duration > 1) { std::cout << "s"; } std::cout << "." << std::endl;
		}
		if (player_miss_chance_bonus_duration > 0)
		{
			std::cout << player_title << "'s miss chance decreased by " << player_miss_chance_bonus << " for " << player_miss_chance_bonus_duration << " turn"; if (player_miss_chance_bonus_duration > 1) { std::cout << "s"; } std::cout << "." << std::endl;
		}

		std::cout << "\nEnemy health " << monster_hp_current << "/" << monster_hp_max;
		std::cout << "		Your health " << player_hp_current << "/" << player_hp_max << std::endl;
		std::cout << "				Sanity " << player_san_current << "/" << player_san_max << std::endl;
		if (can_use_scribe_skills)
		{
			if (runes > 0) { std::cout << "				Runes " << runes; }
			else { std::cout << "				Need to carve new runes"; }
		}

		std::cout << "\n		You decide to\n";
		std::cout << "		1. Attack\n		2. Block\n		3. Use item\n		4. Skip turn";
		if (can_use_magic)
		{
			std::cout << "\n		5. Open spellbook";
		}
		else if (can_use_veteran_skills)
		{
			std::cout << "\n		5. Use a tactic";
		}
		else if (can_use_scribe_skills)
		{
			std::cout << "\n		5. Break a rune";
		}

		
			//player turn
			switch (readInput())
			{
				//attack
			case 49:
				action_chosen = true;
				clrscr();
				animate_text_c("You take a swing at the enemy...");
				if (is_hit(player_miss_chance_max - player_miss_chance_bonus))
				{
					total_damage = (hit_amount(player_crit_chance, player_damage_max, player_damage_min) - (monster_defense - defense_penalty)) + player_damage_bonus;
					animate_text_c(" and get a clean hit.");
					if (was_crit)
					{
						animate_text_c(" Critical hit!");
						was_crit = false;
					}
					if (total_damage <= 0)
					{
						animate_text_c(" But the enemy managed to block your strike.");
						total_damage = 0;
					}
					monster_hp_current -= total_damage;

					if (monster == '&' && monster_transformed)
					{
						--player_hp_current;
					}

					total_damage = 0;
				}
				else
				{
					animate_text_c(" and miss.");
				}
				break;

				//block
			case 50:
				action_chosen = true;
				clrscr();
				animate_text_c("You steel your posture and raise your armaments. Defense increased!");
				player_defense_total = player_defense_base + block_bonus;
				break;

				//use item
			case 51:
				if (!item_used)
				{
					inventory = manage_inventory(inventory);
					item_used = true;
				}
				else
				{
					clrscr();
					animate_text_c("You have no time to use another item.");
				}
				break;

				//skip
			case 52:
				action_chosen = true;
				clrscr();
				animate_text_c("You disregard the enemy's advance with idle indifference.");
				break;

				//special actions
			case 53:
				action_chosen = true;
				if (can_use_magic)
				{
					skill_selected = use_spell(spellbook);
				}
				else if (can_use_veteran_skills)
				{
					skill_selected = use_veteran_skill(veteran_skill);
				}
				else if (can_use_scribe_skills)
				{
					skill_selected = use_scribe_skill(scribe_skill);
				}

				//can't use skill
				if (skill_selected == "Mind")
				{
					clrscr();
					animate_text_c("Your disregard for your own mind has cost you precious opportunity.");
					break;
				}
				else if (skill_selected == "Body")
				{
					clrscr();
					animate_text_c("Your disregard for your own body has cost you precious opportunity.");
					break;
				}
				else if (skill_selected == "Runes")
				{
					clrscr();
					animate_text_c("Your meddling with the runes has cost you precious opportunity.");
					break;
				}
				else if (skill_selected == "None")
				{
					clrscr();
					animate_text_c("Your hesitation has cost you precious opportunity.");
					break;
				}

				//static elemental cast
				if (skill_selected == "Summon static elemental")
				{
					monster_hp_current -= 10;
					player_san_current -= 6;
					monster_turn_skip = true;
					break;
				}

				//water elemental cast
				else if (skill_selected == "Summon water elemental")
				{
					monster_hp_current -= 3;
					player_san_current -= 2;
					miss_chance_increase = 15;
					miss_chance_debuff_time = 4;
					break;
				}

				//earth elemental cast
				else if (skill_selected == "Summon earth elemental")
				{
					monster_hp_current -= 6;
					player_san_current -= 2;
					damage_penalty = 2;
					damage_debuff_time = 3;
					break;
				}

				//fire elemental cast
				else if (skill_selected == "Summon fire elemental")
				{
					monster_hp_current -= 15;
					player_san_current -= 10;
					defense_penalty = 1;
					defense_debuff_time = 4;
					break;
				}

				//relive trauma cast
				else if (skill_selected == "Relive trauma")
				{
					player_san_current -= 3;
					player_defense_bonus = 2;
					player_defense_bonus_duration = 4;
					used_trauma = true;
					break;
				}

				//focus anger cast
				else if (skill_selected == "Focus anger")
				{
					player_san_current -= 4;
					player_damage_bonus = 2;
					player_damage_bonus_duration = 4;
					used_focus = true;
					break;
				}

				//disregard limitations cast
				else if (skill_selected == "Disregard limitations")
				{
					player_hp_current -= 5;
					total_damage = hit_amount(player_crit_chance, player_damage_max, player_damage_min) + 5 + player_damage_bonus;
					if (was_crit)
					{
						animate_text_c(" Critical hit!");
						was_crit = false;
					}

					if (total_damage <= 0)
					{
						animate_text_c(" But your attack has no effect.");
						total_damage = 0;
					}

					monster_hp_current -= total_damage;

					if (monster == '&' && monster_transformed)
					{
						--player_hp_current;
					}

					total_damage = 0;

					used_disregard_c = true;
					used_disregard = true;
					break;
				}

				//study enemy cast
				else if (skill_selected == "Study enemy")
				{
					player_miss_chance_bonus = 10;
					player_miss_chance_bonus_duration = 4;

					used_study_c = true;
					used_study = true;
					break;
				}

				//bloodboil rune cast
				else if (skill_selected == "Bloodboil rune" || skill_selected == "Bloodboil rune unstable")
				{
					--runes;
					if (skill_selected == "Bloodboil rune")
					{
						animate_text_c(" Crimson cracks cover the rune, and it crumbles to small pieces.");

						if (!used_bloodboil)
						{
							clrscr();
							animate_text("As the cracks start covering the rune, the enemy shakes and writhes.\nThe enemy is soon covered in steam, emitting from within it.\nThe enemy's blood is boiling which causes continuous trauma!");
						}
						else
						{
							clrscr();
							animate_text_c("The enemy's blood boils!");
						}
					}
					else if (skill_selected == "Bloodboil rune unstable")
					{
						animate_text_c(" The rune starts melting into vivid red liquid that cuts your hand.");
						player_hp_current -= 2;

						if (!used_bloodboil)
						{
							clrscr();
							animate_text("As the rune melts, the enemy shakes and writhes.\nThe enemy is soon covered in steam, emitting from within it.\nThe enemy's blood is boiling which causes continuous trauma!");
						}
						else
						{
							clrscr();
							animate_text_c("Your hand is bleeding.\nThe enemy's blood boils!");
						}
					}

					used_bloodboil = true;
					bloodboil_duration += 4;

					break;
				}

				//plague rune cast
				else if (skill_selected == "Plague rune" || skill_selected == "Plague rune unstable")
				{
					--runes;
					if (skill_selected == "Plague rune")
					{
						animate_text_c(" The rune decays and turns into dust.");

						if (!used_plague)
						{
							clrscr();
							animate_text("As the rune decays, the enemy twitches, as if pierced by a sudden jolt of pain.\nIt's skin pales, then starts undulating sickeningly.\nSuddenly, various places burst with pus and blood, other areas are lacerated and covered with ulcers.\nThe enemy's resilience is weakened significantly!");
						}
						else
						{
							clrscr();
							animate_text_c("The enemy's resilience is weakened significantly!");
						}
					}
					else if (skill_selected == "Plague rune unstable")
					{
						animate_text_c(" The rune covers in pustules and ulcers, emitting a vile stench, then bursts into vile liquid and melts entirely.");
						player_plague_duration += 2;

						if (!used_plague)
						{
							clrscr();
							animate_text("As the rune is liquified, the enemy twitches, as if pierced by a sudden jolt of pain.\nIt's skin pales, then starts undulating sickeningly.\nSuddenly, various places burst with pus and blood, other areas are lacerated and covered with ulcers.\nThe enemy's resilience is weakened significantly!");
						}
						else
						{
							clrscr();
							animate_text_c("You feel sick.\nThe enemy's resilience is weakened significantly!");
						}
					}

					used_plague = true;
					plague_duration += 4;

					break;
				}

				//collapse rune cast
				else if (skill_selected == "Collapse rune" || skill_selected == "Collapse rune unstable")
				{
					runes -= 2;

					//stable variant
					if (skill_selected == "Collapse rune")
					{
						animate_text_c(" The runes impossibly merge until both disappear.");


						if (!used_collapse)
						{
							if (bloodboil_duration > 0 || plague_duration > 0 || entropy_duration > 0)
							{
								clrscr();
								animate_text("Rigth as the runes merge, the enemy starts to morph and distort, shapeshifting to different forms.\nEven the sound the enemy is producing is rapidly switching pitches.\nA thunderous clap emerges from inside the enemy, which is shaken and damaged.\nThe burden of curses affecting the enemy collapses to a singularity!");
							}
							else
							{
								clrscr();
								animate_text_c("The enemy is not affected in any way.");
							}
						}
						else
						{
							if (bloodboil_duration > 0 || plague_duration > 0 || entropy_duration > 0)
							{
								clrscr();
								animate_text_c("The burden of curses affecting the enemy collapses to a singularity!");
							}
							else
							{
								clrscr();
								animate_text_c("The enemy is not affected in any way.");
							}
						}

						if (bloodboil_duration > 0 || plague_duration > 0 || entropy_duration > 0)
						{
							if (bloodboil_duration > 0)
							{
								monster_hp_current -= 1 * bloodboil_duration;
								bloodboil_duration = 0;
							}
							if (plague_duration > 0)
							{
								monster_hp_current -= 1 * plague_duration;
								plague_duration = 0;
							}
							if (entropy_duration > 0)
							{
								monster_hp_current -= 3 * entropy_duration;
							}
						}
					}

					//unstable variant
					else if (skill_selected == "Collapse rune unstable")
					{
						runes -= 2;
						animate_text_c(" The runes start distorting until both become an unusable mess covered in illegible writings.");

						if (!used_collapse)
						{
							if (bloodboil_duration > 0 || plague_duration > 0 || entropy_duration > 0)
							{
								animate_text("As you drop the defective runes, the enemy seems to morph and distort, shapeshifting to different forms.\nEven the sound the enemy is producing is rapidly switching pitches.\nA thunderous clap emerges from inside the enemy, which is shaken and damaged.\nThe clap resonates with your body, echoing each curse the enemy bears with intense pain.\nThe burden of curses affecting the enemy collapses to a singularity!");
							}
							else
							{
								clrscr();
								animate_text_c("The enemy is not affected in any way.\nYour mind feels distorted.");
							}
						}
						else
						{
							if (bloodboil_duration > 0 || plague_duration > 0 || entropy_duration > 0)
							{
								animate_text_c("The burden of curses affecting the enemy collapses to a singularity that resonates with your body!");
							}
							else
							{
								clrscr();
								animate_text_c("The enemy is not affected in any way.\nYour mind feels distorted.");
							}
						}

						if (bloodboil_duration > 0 || plague_duration > 0 || entropy_duration > 0)
						{
							if (bloodboil_duration > 0)
							{
								monster_hp_current -= 1 * bloodboil_duration;
								--player_hp_current;
								bloodboil_duration = 0;
							}
							if (plague_duration > 0)
							{
								monster_hp_current -= 1 * plague_duration;
								--player_hp_current;
								plague_duration = 0;
							}
							if (entropy_duration > 0)
							{
								monster_hp_current -= 3 * entropy_duration;
								player_hp_current -= 2;
								entropy_duration = 0;
							}
						}
						else
						{
							player_san_current -= 3;
						}
					}

					used_collapse = true;
					break;
				}

				//entropy rune cast
				else if (skill_selected == "Entropy rune" || skill_selected == "Entropy rune unstable")
				{
					runes -= 3;
					if (skill_selected == "Entropy rune")
					{
						animate_text_c(" The runes are violently pressed into thin crusts, which then erode quickly.");
						if (!used_entropy)
						{
							clrscr();
							animate_text("While the runes are being pressed, the enemy starts trembling and twitching uncontrollably.\nIt seems that it's going through excruciating pain.\nThe enemy's skin turns lifeless and dry, thin cracks forming here and there.\nIt seems that the very life force is being pulled from within the enemy.\nThe enemy's health is being drained!");
						}
						else
						{
							clrscr();
							animate_text_c("The enemy's health is being drained!");
						}
						entropy_duration += 3;
					}
					else if (skill_selected == "Entropy rune unstable")
					{
												
						animate_text_c(" The runes glow in an unnerving kaleidoscope of muted colors.");
						if (!used_entropy)
						{
							clrscr();
							animate_text("While the runes are glowing, the enemy starts trembling and twitching uncontrollably.\nThe enemy is in excruciating pain.\nIts skin turns lifeless and dry, thin cracks forming here and there.\nIt seems that the very life force is being extracted from the enemy.\nA similar ailment is rampaging through your mind.\nThe enemy's health and your sanity are being drained!");
						}
						else
						{
							clrscr();
							animate_text_c("The enemy's health and your sanity are being drained!");
						}
						entropy_duration += 3;
						player_entropy_duration += 3;
					}

					used_entropy = true;
					break;
				}

				//mistype
			default:
				action_chosen = false;
				break;
			}
		}

		//monster transformation
		if (monster == '*' && monster_hp_current <= monster_hp_max * 0.5)
		{
			if (!monster_transformed)
			{
				clrscr();
				animate_text_c("The creature grows additional appendages, slithering from the wounds you dealt!");
				monster_transformed = true;
			}
		}

		else if (monster == '&' && player_san_current <= player_san_max * 0.5)
		{
			if (!monster_transformed)
			{
				clrscr();
				animate_text_c("The creature suddenly looks sharper, a myriad of tiny claws erupt from it.\nAny strike will cause you damage!");
				monster_transformed = true;
			}
		}

		//monster turn
		clrscr();
		if (!monster_turn_skip && monster_hp_current > 0 && player_hp_current > 0 && player_san_current > 0)
		{
			if (monster == '*' && monster_transformed)
			{
				if (monster_chance_to_use_skill(monster))
				{
					animate_text_c("The mass' additional appendages swing...");
					if (is_hit(monster_miss_chance + miss_chance_increase))
					{
						animate_text_c(" and cleave through your flesh!");
						player_hp_current -= 3;
					}
					else
					{
						animate_text_c(" and miss!");
					}
				}
			}

			if (monster == '&')
			{
				if (monster_chance_to_use_skill(monster))
				{
					clrscr();
					animate_text("The slug stretches up and emits a deafening screech.\nThe sound unnerves you deeply. You clench your teeth as your body tenses in sudden surge of panic.");
					player_san_current -= 4;
				}
			}

			if (monster == '%')
			{
				if (monster_chance_to_use_skill(monster))
				{
					clrscr();
					animate_text("The creature arches its back preparing for attack.\nThe strike that follows is blindingly fast, performed with both clawed hands.\nAs the creature jumps past you, you realize both your hands are bleeding!");
					player_bleed_duration = 6;
				}
			}

			clrscr();
			animate_text_c("The enemy swings at you...");
			if (is_hit(monster_miss_chance + miss_chance_increase))
			{
				animate_text_c(" and hits you cleanly.");
				total_damage = (hit_amount(monster_crit_chance, (monster_damage_max - damage_penalty), monster_damage_min) - player_defense_total) - player_defense_bonus;
				if (was_crit)
				{
					animate_text_c(" Critical hit!");
					was_crit = false;
				}
				if (total_damage <= 0)
				{
					animate_text_c(" But you managed to block the attack just in time.");
					total_damage = 0;
				}
				player_hp_current -= total_damage;
			}
			else
			{
				animate_text_c(" and misses.");
			}
		}
		else
		{
			if (monster_hp_current > 0)
			{
				animate_text_c("The enemy is unable to act.");
			}
		}

		//DOT ticks
		if (plague_duration > 0)
		{
			if (bloodboil_duration > 0 || entropy_duration > 0)
			{
				--monster_hp_current;
			}
			--plague_duration;
		}
		if (bloodboil_duration > 0)
		{
			--monster_hp_current;
			--bloodboil_duration;
		}
		if (entropy_duration > 0)
		{
			if (monster_hp_current > monster_hp_max * 0.75 && monster_hp_current <= monster_hp_max)
			{
				monster_hp_current -= 3;
			}
			else if (monster_hp_current > monster_hp_max * 0.5 && monster_hp_current <= monster_hp_max * 0.75)
			{
				monster_hp_current -= 2;
			}
			else if (monster_hp_current <= monster_hp_max * 0.5)
			{
				--monster_hp_current;
			}
			--entropy_duration;
		}
		if (player_plague_duration > 0)
		{
			--player_hp_current;
			--player_plague_duration;
		}
		if (player_entropy_duration > 0)
		{
			if (player_san_current <= player_san_max && player_san_current > player_san_max * 0.75)
			{
				player_san_current -= 3;
			}
			else if (player_san_current > player_san_max * 0.5 && player_san_current <= player_san_max * 0.75)
			{
				player_san_current -= 2;
			}
			else if (player_san_current <= player_san_max * 0.5)
			{
				--player_san_current;
			}
			--player_entropy_duration;
		}
		if (player_bleed_duration > 0)
		{
			--player_hp_current;
			--player_bleed_duration;
		}

		//debuff ticks
		if (defense_debuff_time > 0)
		{
			--defense_debuff_time;
		}
		if (miss_chance_debuff_time > 0)
		{
			--miss_chance_debuff_time;
		}
		if (damage_debuff_time > 0)
		{
			--damage_debuff_time;
		}

		//buff ticks
		if (player_defense_bonus_duration > 0)
		{
			--player_defense_bonus_duration;
		}
		if (player_damage_bonus_duration > 0)
		{
			--player_damage_bonus_duration;
		}
		if (player_miss_chance_bonus_duration > 0)
		{
			--player_miss_chance_bonus_duration;
		}
	
		//inventory use reset
		item_used = false;
	}

	//DOTs reset
	if (plague_duration > 0)
	{
		plague_duration = 0;
	}
	if (bloodboil_duration > 0)
	{
		bloodboil_duration = 0;
	}
	if (entropy_duration > 0)
	{
		entropy_duration = 0;
	}
	if (player_plague_duration > 0)
	{
		player_plague_duration = 0;
	}
	if (player_entropy_duration > 0)
	{
		player_entropy_duration = 0;
	}
	if (player_bleed_duration > 0)
	{
		player_bleed_duration = 0;
	}

	//monster transformation reset
	monster_transformed = false;

	//reset class specific one-time use skills
	used_disregard_c = false;
	used_study_c = false;

	//monster defeat type-specific text
	if (monster_hp_current <= 0)
	{
		clrscr();
		if (monster == '*')
		{
			animate_text("As the creature's flailing steadily slows down, you notice its eyes dimming, filled with acceptance and relief.\nIt seems that whatever force propelling this ungodly defilement of humanly form is no longer present.\nThe mass lowers to the ground and ceases any movements.\nYou stand victorious.");
		}
		else if (monster == 'H')
		{
			animate_text("Another lifeless body decorates the battlefield.\nYou turn your attention to the rest of the battlefield.");
		}
		else if (monster == '&')
		{
			animate_text("The slug is torn to pieces and cannot hold its form together.\nIt trembles and vibrates, bubbles forming on its surface.\nAfter a moment, it splashes into a puddle of rancid remains.\nThe stench is unbearable.");
		}
		else if (monster == '%')
		{
			animate_text("You tense down a bit, while the creature falls to the ground, still silently trying to claw its way to you.\nThe look in both eyes tells a tale of unresolved duty, unfinished task.\nThe creature stills.\nYou remain vigilant for further encounters.");
		}
	}
	if (player_hp_current <= 0)
	{
		if (monster == '*')
		{
			game_over = true;
			game_over_screen("lump");
		}
		else if (monster == '&')
		{
			game_over = true;
			game_over_screen("slug");
		}
		else if (monster == '%')
		{
			game_over = true;
			game_over_screen("twins");
		}
	}
	if (player_san_current <= 0 && monster != 'H')
	{
		game_over = true;
		game_over_screen("sanity loss combat m");
	}
	if (player_san_current <= 0 && monster == 'H')
	{
		game_over = true;
		game_over_screen("sanity loss combat h");
	}

	if (current_map_title == "Abandoned Village" && (monster == '*' || monster == '&' || monster == '%'))
	{
		--monster_count_abdv;
	}
	stop_sound();
}


//randomize garbage pickup messages
void randomize_garbage()
{
	int what_message{};
	std::uniform_int_distribution x{ 0, 5 };
	what_message = x(mt);

	clrscr();
	if (what_message == 1)
	{
		animate_text("You find a pile of bones and torn cloth.\nThere's nothing useful among the remains.");
	}
	else if (what_message == 2)
	{
		animate_text("A cairn-like rock formation.\nSearching through the rocks reveals nothing.");
	}
	else if (what_message == 3)
	{
		animate_text("Wooden debris piled up on the ground.\nYou turn over some splintered planks, but there's nothing useful in there.");
	}
	else if (what_message == 4)
	{
		animate_text("You see a shallow hole in the ground, somewhat covered with soil.\nA quick dig in the dirt reveals a largely missing skeleton, but no belongings were buried with this one.");
	}
	else if (what_message == 5)
	{
		animate_text("This tree is hollowed-out, probably to stash something.\nIt's just a squirrel's nest now.");
	}
}


//defines the type of an item placed on a map. uses x, y, and a map's title to determine what item type is placed in that spot
std::vector<std::string> pickup(std::vector<std::string> inv, int x, int y, std::string title)
{
	clrscr();

	if (x == 3 && y == 15 && title == "Abandoned Village")
	{
		animate_text("Picked up some bandages.");
		inv.push_back("Bandages");
	}
	else if (x == 6 && y == 2 && title == "Abandoned Village")
	{
		animate_text("Picked up some bandages.");
		inv.push_back("Bandages");
	}
	else if (x == 18 && y == 4 && title == "Abandoned Village")
	{
		animate_text("Picked up a calming insence.");
		inv.push_back("Insence");
	}
	else if (x == 14 && y == 7 && title == "Abandoned Village")
	{
		animate_text("Picked up some bandages.");
		inv.push_back("Bandages");
	}
	else if (x == 13 && y == 7 && title == "Abandoned Village")
	{
		animate_text("Picked up a rune of stillness.");
		inv.push_back("Rune of stillness");
	}
	else if (x == 14 && y == 8 && title == "Abandoned Village")
	{
		animate_text("Picked up some bandages.");
		inv.push_back("Bandages");
	}
	else if (x == 18 && y == 18 && title == "Abandoned Village")
	{
		animate_text("Picked up a pouch of black powder.");
		has_powder = true;
		graveyard[10][6] = 'M';
	}
	else if (x == 12 && y == 7 && title == "Graveyard")
	{
		animate_text("Picked up some bandages.");
		inv.push_back("Bandages");
	}
	else if (x == 20 && y == 6 && title == "Graveyard")
	{
		animate_text("Picked up a rune of stillness.");
		inv.push_back("Rune of stillness");
	}
	else if (x == 7 && y == 13 && title == "Graveyard")
	{
		animate_text("Picked up some bandages.");
		inv.push_back("Bandages");
	}
	else if (x == 7 && y == 19 && title == "Graveyard")
	{
		animate_text("Picked up a calming insence.");
		inv.push_back("Insence");
	}
	else if (x == 1 && y == 18 && title == "Graveyard")
	{
		animate_text("Picked up a rune of wholeness.");
		inv.push_back("Rune of wholeness");
	}

	else if (title == "Graveyard" && ((x == 8 && y == 9) || (x == 3 && y == 3) || (x == 13 && y == 13) || (x == 16 && y == 6) || (x == 21 && y == 16)))
	{
		randomize_garbage();
	}


	else if (x == 3 && y == 13 && title == "Graveyard")
	{
		animate_text("You find a roll of fuse.");
		has_fuse = true;
	}
	else if (x == 5 && y == 19 && title == "Graveyard")
	{
		animate_text("You find an intact flint stone.");
		has_flint = true;
	}

	if (has_flint && has_fuse && has_powder)
	{
		clrscr();
		animate_text("You stick the fuse in the pouch with the powder.\nGiven a bit of spark, this will blow a substantial hole in any rock formation.");
		explosive_ready = true;
	}

	return inv;
}

//make the text "animated"
void animate_text(std::string text)
{
	for (int i = 0; i < text.size(); i++)
	{
		if (_kbhit())
		{
			clrscr();
			std::cout << text;
			break;
		}
		std::cout << text[i];
		if (text[i] == '.' || text[i] == '!' || text[i] == '?')
		{
			std::this_thread::sleep_for(long_frame);
		}
		std::this_thread::sleep_for(frame);
	}
	std::cin.get();
	return;
}

//make the text "animated"; combat version
void animate_text_c(std::string text)
{
	for (int i = 0; i < text.size(); i++)
	{
		std::cout << text[i];
		if (text[i] == '.')
		{
			std::this_thread::sleep_for(long_frame);
		}
		std::this_thread::sleep_for(frame);
	}
	std::this_thread::sleep_for(pause_after_text);
	return;
}

//item use function
void use_item(std::string item_name)
{
	clrscr();

	if (item_name == "Bandages")
	{
		if (player_hp_current != player_hp_max)
		{
			if (player_hp_max - player_hp_current <= 10)
			{
				player_hp_current = player_hp_max;
				animate_text("You used the bandages to stop a minor bleeding, dressing some cuts.");
			}
			else
			{
				player_hp_current += 10;
				animate_text("You used the bandages to stop the bleeding.\nThis will not suffice, though.\nThe cuts are too deep.");
			}
			can_use_item = true;
		}
		else
		{
			animate_text("You don't have any injuries to use the bandages on.");
		}
	}

	else if (item_name == "Rune of wholeness")
	{
		if (player_hp_current < player_hp_max)
		{
			player_hp_current = player_hp_max;
			animate_text("The rune crumbles in your arm, and the wounds you have start closing rapidly.\nYou feel invigorated.");
			can_use_item = true;
		}
		else
		{
			animate_text("You try to use the rune, but it does not react to your evocation.");
		}
	}


	else if (item_name == "Sword")
	{
		if (!sword_used)
		{
			animate_text("The sword is rusted and covered in cracks and dents.\nThe blade's dull for the most part, but there's some bite to one of the sides.\nUse it to draw some blood? (y/n)");
		}
		else
		{
			animate_text("Use the sword? (y/n)");
		}
		switch (readInput())
		{
		case 121:
			clrscr();
			if (!sword_used)
			{
				animate_text("The sword breaks as soon as you finish a cut near your elbow.\nAt first, there's practically no blood.\nYou blink, and the droplets from your arm form a small stain on the ground.");
			}
			else
			{
				animate_text("The sword breaks as soon as the cut is finished.");
			}
			sword_used = true;
			can_use_item = true;
			player_hp_current -= 5;
			if (player_hp_current == 0)
			{
				game_over = true;
				game_over_text = "sword suicide";
			}
			break;
		case 110:
			clrscr();
			animate_text("You put the sword away.");
			break;
		}
	}


	else if (item_name == "Rune of stillness")
	{
		if (player_san_current != player_san_max)
		{
			player_san_current = player_san_max;
			
			animate_text("As the rune crumbles in your arm, you feel your mind soothing.\nThe burden of life seems to not be so crushing at the moment.");
			can_use_item = true;
		}
		else
		{
			animate_text("You try to use the rune, but it does not react to your evocation.");
		}
	}

	else if (item_name == "Insence")
	{
		clrscr();
		if (player_san_current == player_san_max)
		{
			animate_text("You feel calm and collected, no need to use the insence.");
		}
		else if (player_san_current >= player_san_max - 5)
		{
			animate_text("The smell of burning insence steadies your mind, eases your worries.");
			player_san_current = player_san_max;
			can_use_item = true;
		}
		else if (player_san_current < player_san_max - 5)
		{
			animate_text("You watch the burning insence tremble in your shaking hands.\nIt helps somewhat, but that is not enough to calm you down.");
			player_san_current += 5;
			can_use_item = true;
		}
	}
}

//managing inventory
std::vector<std::string> manage_inventory(std::vector<std::string> inventory)
{
	int item_selected{};
	std::string temp{};
	if (inventory.size() == 0)
	{
		clrscr();
		std::cout << "\n	Inventory:\n\n";
		animate_text("You don't have any items.");
	}
	else
	{
		clrscr();
		std::cout << "\n	Inventory:\n\n";
		for (int i = 0; i < inventory.size(); i++)
		{
			std::cout << i + 1 << ". " << inventory[i];
			if (i != inventory.size() - 1)
			{
				std::cout << ";\n";
			}
			else
			{
				std::cout << ".\n";
			}
		}
		if (inventory.size() == 6)
		{
			std::cout << "Your inventory is full. You cannot pick up new items.";
		}
		item_selected = readInput();

		switch (item_selected)
		{
			//1
		case 49:
			clrscr();
			if (inventory[0] == "Bandages")
			{
				use_item("Bandages");
			}
			else if (inventory[0] == "Sword")
			{
				use_item("Sword");
			}
			else if (inventory[0] == "Rune of stillness")
			{
				use_item("Rune of stillness");
			}
			else if (inventory[0] == "Insence")
			{
				use_item("Insence");
			}
			else if (inventory[0] == "Rune of wholeness")
			{
				use_item("Rune of wholeness");
			}
			if (can_use_item)
			{
				temp = inventory[0];
			for (int i = 0; i < inventory.size(); i++)
			{
				if (i < inventory.size() - 1)
				{
					inventory[i] = inventory[i + 1];
				}
			}
				inventory[inventory.size() - 1] = temp;
				inventory.pop_back();
				can_use_item = false;
			}
			break;

			//2
		case 50:
			clrscr();
			if (inventory.size() > 1)
			{
				if (inventory[1] == "Bandages")
				{
					use_item("Bandages");
				}
				else if (inventory[1] == "Sword")
				{
					use_item("Sword");
				}
				else if (inventory[1] == "Rune of stillness")
				{
					use_item("Rune of stillness");
				}
				else if (inventory[1] == "Insence")
				{
					use_item("Insence");
				}
				else if (inventory[1] == "Rune of wholeness")
				{
					use_item("Rune of wholeness");
				}
				if (can_use_item)
				{
					temp = inventory[1];
					for (int i = 1; i < inventory.size(); i++)
					{
						if (i < inventory.size() - 1)
						{
							inventory[i] = inventory[i + 1];
						}
					}
					inventory[inventory.size() - 1] = temp;
					inventory.pop_back();
					can_use_item = false;
				}
			}
			break;

			//3
		case 51:
			if (inventory.size() > 2)
			{
				clrscr();
				if (inventory[2] == "Bandages")
				{
					use_item("Bandages");
				}
				else if (inventory[2] == "Sword")
				{
					use_item("Sword");
				}
				else if (inventory[2] == "Rune of stillness")
				{
					use_item("Rune of stillness");
				}
				else if (inventory[2] == "Insence")
				{
					use_item("Insence");
				}
				else if (inventory[2] == "Rune of wholeness")
				{
					use_item("Rune of wholeness");
				}
				if (can_use_item)
				{
					temp = inventory[2];
					for (int i = 2; i < inventory.size(); i++)
					{
						if (i < inventory.size() - 1)
						{
							inventory[i] = inventory[i + 1];
						}
					}
					inventory[inventory.size() - 1] = temp;
					inventory.pop_back();
					can_use_item = false;
				}
			}
			break;

			//4
		case 52:
			clrscr();
			if (inventory.size() > 3)
			{
				if (inventory[3] == "Bandages")
				{
					use_item("Bandages");
				}
				else if (inventory[3] == "Sword")
				{
					use_item("Sword");
				}
				else if (inventory[3] == "Rune of stillness")
				{
					use_item("Rune of stillness");
				}
				else if (inventory[3] == "Insence")
				{
					use_item("Insence");
				}
				else if (inventory[3] == "Rune of wholeness")
				{
					use_item("Rune of wholeness");
				}
				if (can_use_item)
				{
					temp = inventory[3];
					for (int i = 3; i < inventory.size(); i++)
					{
						if (i < inventory.size() - 1)
						{
							inventory[i] = inventory[i + 1];
						}
					}
					inventory[inventory.size() - 1] = temp;
					inventory.pop_back();
					can_use_item = false;
				}
			}
			break;

			//5
		case 53:
			clrscr();
			if (inventory.size() > 4)
			{
				if (inventory[4] == "Bandages")
				{
					use_item("Bandages");
				}
				else if (inventory[4] == "Sword")
				{
					use_item("Sword");
				}
				else if (inventory[4] == "Rune of stillness")
				{
					use_item("Rune of stillness");
				}
				else if (inventory[4] == "Insence")
				{
					use_item("Insence");
				}
				else if (inventory[4] == "Rune of wholeness")
				{
					use_item("Rune of wholeness");
				}
				if (can_use_item)
				{
					temp = inventory[4];
					for (int i = 4; i < inventory.size(); i++)
					{
						if (i < inventory.size() - 1)
						{
							inventory[i] = inventory[i + 1];
						}
					}
					inventory[inventory.size() - 1] = temp;
					inventory.pop_back();
					can_use_item = false;
				}
			}
			break;

			//6
		case 54:
			clrscr();
			if (inventory.size() > 5)
			{
				if (inventory[5] == "Bandages")
				{
					use_item("Bandages");
				}
				else if (inventory[5] == "Sword")
				{
					use_item("Sword");
				}
				else if (inventory[5] == "Rune of stillness")
				{
					use_item("Rune of stillness");
				}
				else if (inventory[5] == "Insence")
				{
					use_item("Insence");
				}
				else if (inventory[5] == "Rune of wholeness")
				{
					use_item("Rune of wholeness");
				}
				if (can_use_item)
				{
					inventory.pop_back();
					can_use_item = false;
				}
			}
			break;


		default:
			return inventory;
		}
	}
	return inventory;
}


void npc_interaction(char npc, int x, int y)
{
	clrscr();
	if (npc == 'o')
	{
		play_sound("Theme 3");
		animate_text("As you get closer to the hole in the ground, you notice that the scenery around you is changing.\nYou halt, and the change ceases.\nConfused, you take a step back... but you do not move.\nYou turn around and start running towards the path leading to shore.\nNothing changes. It's as if you're being pulled to the hole with a force too strong for you to outrun.");
		clrscr();
		animate_text("You turn around and face the hole again.\nThe light around the hole seems to be bending and distorting.\nAs if everything is frozen in time, stuck in the moment of being devoured by this strange place...\nOr is it the other way around?");
		clrscr();
		animate_text("With no other option, you start slowly walking towards the hole.\nUnsurprisingly, movements occur normally now.\nAs you get closer to the edge of the hole, you notice a faint light escaping the hole.\nIt lools like something is there, at the bottom of the hole, waiting.\n");
		clrscr();
		animate_text("You reach the hole.\nThe glowing is present, but you can't see the source.\nAs you get down to one knee, your head starts spinning wildly.\nVisions, sounds, feelings - a stream of foreign consciousness assaults your own.\nYou do not understand any of what you're seeing.\nAll looks incoherent as if produced by a dying mind, feverishly clawing at reality, trying to escape the grasp of nothingness.\nYou feel crushing fear, maddening realisation of inevitable doom.\nYou scream, but all you produce is a dry screech.\nYou come undone, as immense terror is tearing through you.\nYou gasp for air, but it's never enogh, you can't breathe.");
		clrscr();
		animate_text("You drop to the ground, clawing at the damp soil until your mind finally gives up.\nYou faint...");
		stop_sound();
		
		clrscr();
		animate_text("You come to.\nYou take a look around. Nothing seems to have changed, and yet all feels different somehow.\nA sudden alertness rising in your mind.\nYou get off the ground.\nYou need to get out of here.");
		seen_hole = true;
		current_map[12][10] = ' ';
	}


	else if (npc == '#')
	{
		if (!has_gate_key && current_map_title == "Abandoned Village")
		{
			play_sound("Can't open gates");
			animate_text("A gate is blocking your way. It's too sturdy to open by force.\nYou will need a key.");
		}
		else if (has_gate_key && current_map_title == "Abandoned Village")
		{
			play_sound("Gate open");
			animate_text("You unlock the door with the brass key you found.\n");
			current_map[15][17] = ' ';
		}
	}

	else if (npc == '@')
	{
		if (current_map_title == "Abandoned Village")
		{
			switch (monster_count_abdv)
			{
			case 6:
				monster_count_t_abdv = "";
				break;
			case 5:
				monster_count_t_abdv = "\nOne is broken.";
				break;
			case 4:
				monster_count_t_abdv = "\nTwo are broken.";
				break;
			case 3:
				monster_count_t_abdv = "\nThree are broken.";
				break;
			case 2:
				monster_count_t_abdv = "\nFour are broken.";
				break;
			case 1:
				monster_count_t_abdv = "\nFive are broken";
				break;
			}
			play_sound("Fleshy gate");
			if (monster_count_abdv > 0)
			{
				animate_text("An undulating spiral shaped growth blocks your way.\nYou try cutting through it, but the 'wounds' close right back without any marks left.\nThere are glowing marbles embedded in the growth." + monster_count_t_abdv);
			}
			else
			{
				animate_text("With all the marbles broken, the growth is starting to shrivel and die off.\nAs the growth ceases any movement, you realise its now easy to break through it.\nAs you finish breaking a segment off, you hear hissing...");
				current_map[11][9] = ' ';
				silent_pond[0][8] = ' ';
				combat('&');
				if (!game_over)
				{
					animate_text("Inside the creature's remains, there's a small brass key.\nYou wipe it of the mucus and tuck it in one of your pockets.");
					has_gate_key = true;
				}
			}
		}
		else if (current_map_title == "Silent Pond")
		{
			play_sound("Fleshy gate");
			animate_text("This growth blocks your passage. It's not clear how you can proceed.\nThere seems to be a vein of sorts protruding into the ground on the left side of the growth.");
		}
	}


	else if (npc == 't')
	{
		clrscr();
		if (current_map_title == "Abandoned Village" && x == 11 && y == 18)
		{
			animate_text("A crooked wooden sign.\nThe sign's front side is covered in moss and rotted.\nYou clean some moss, but there doesn't seem to be anything written.\nEither that or the writing faded away with time.");
		}

		else if (current_map_title == "Silent Pond" && x == 16 && y == 11)
		{
			animate_text("There seems to be some kind of engraving on the tree.\nIt depicts a human lying at the bottom of a deep hole in the ground.\nThere are people gathered around the edges of the hole.\nTheir hands are lifted to the skies.\nThere are numerous iron wedges driven into the tree's bark.\nThere are darkened stains under each of the wedges.");
		}

		else if (current_map_title == "Abandoned Village" && x == 9 && y == 7)
		{
			play_sound("Pages");
			animate_text("Some pages torn from a book - probably a diary - are scaddered in the dirt.\nMost of the text is smeared over with some kind of mucus, making most of the text illegible.");
			std::cout << "\n\n1. Read what text you can make out.\n2. Leave the pages.";
			switch (readInput())
			{
			case 49:
				clrscr();
				animate_text("*first half of the page illegible* sealed with boulders. Nodoby will ever go there again. I made sure of that.\n*illegible*, though.\nMy skin's all *illegible* won't even hold me upright anymore.\nThrew up some slimy crap last night.\nMy bones feel like rotten fruit.\nI might *illegible* one of those bastards that tore up Ulcio.\nCouldn't take care of the graveyard entrance.\nDon't think anybody will fit in that damned hole, though.\nI'll lock the door to the mining storage. Just in case somebody wants to blow the entrance open with some black powder.\nIn the morning I'll try for the south side of the wall. Looks like there's a crack forming in it.\nHope *illegible* at the bottom of that damn grave.");
				current_map[7][9] = ' ';
				break;
			case 50:
				clrscr();
				animate_text("You step away from the page.");
				break;
			}
		}

		else if (current_map_title == "Abandoned Village" && x == 18 && y == 13)
		{
			clrscr();
			animate_text("There's a locker with several shovels near the wall, and a small wooden table about it.\nThe shovels are badly worn, handles nearly broken in half each.\nThere's a registry of some sort on the table, that contains various dates and numbers near them.\nIt's unclear what the numbers refer to, but they were increasing over the time this registry was kept.");
		}

		else if (current_map_title == "Abandoned Village" && x == 3 && y == 4)
		{	
			if (!seen_book)
			{
				animate_text("The house is mostly empty, a few planks and other pieces of wood scattered on the floor.\nIn one of the house's corners you notice a wooden plank that's a bit cracked.\nThere's something stuck behind it in the wall."); play_sound("Pile"); animate_text("You tear the plank off the wall\nThere's a book behind it.\nIt's hard-cover, but the cover's material is damp and badly damaged - the thing practically falls apart in your hands.");
				seen_book = true;
			}
			else
			{
				animate_text("A book greatly damaged by time and elements is lying inside a pile of wooden shivers.");
			}
			std::cout << "\n\n1. Read the book.\n2. Leave the book.";
			switch (readInput())
			{
			case 49:
				clrscr();
				play_sound("Pages");
				animate_text("The book reads...");
				clrscr();
				play_sound("Theme 3 long");
				animate_text("This is the archive of the *illegible* mining camp.\n74 families are being sent to the camp to provide the army's smiths with a steady supply of ore for the war effort.\nThe document is being organized by M.A., mine overseer.");
				clrscr();
				animate_text("Day 1:\n\nArrived at the location.\nThe road was surprisingly calm, despite there being reports of numerous bandit attacks, wildlife encounters, and such.\nMade it to the mining site in less than a month, which is faster than expected.\nSent a group of 15 people to scout the surrounding area.\nWe need to set up tents for now, then start building our own huts, so we'll need wood.\nNo problems in this regard - nearby forests are thick.\nWhat troubles me the most is the quality of ore here.\nNot much reports on this place aside from an odd mention of the place being riddled with iron deposits.\nWe'll set up a simple fence around our camp for now, see if we can construct a more substantial cover later.");
				clrscr();
				animate_text("Day 12:\n\nNo significant events in two weeks we've been here.\nFound a promising chert formation in a pond to the east.\nThe pond seems to've been a small hill previously, later formed into a body of water with some elevation in the middle.\nAfter we're finished setting up the huts, we'll get to digging through that rock.");
				clrscr();
				animate_text("Day 38:\n\nSecond month.\nThe main camp is set up nice and firm, surrounded it by a dependable freestanding wall.\nThe guys are not worried some wild bear will come take a bite out of their wives anymore, so we've started digging.\nNot much to see yet, but there's a pretty large cave on the inner side of the pond-hill.\nCan't say I've seen any cave formed like that naturally, so the experience is captivating.\nWe will need to move to other potential sites scouted in our first days, though, since we still can't find any suitable veins in that cave.\nWe'll dig for a couple more days, see if maybe those reports weren't lying after all.");
				clrscr();
				animate_text("Day 49:\n\nFinally found some damn good ore!\nWe uncovered another opening in a passage deeper in the first cave we found.\nThe place is practically made of ore.\nIt's like the place heard we might be leaving and decided to convince us to stay.\nSomething's been on my mind, though.\nCan't shake the feeling somebody's been here.\nThe place looks untouched, but every pickaxe swing gets chunks of ore flying around.\nIt's like somebody put them back in after digging them up before us.\nDon't think I want to stay here for long.");
				clrscr();
				animate_text("Day 56:\n\nDamn cave is cursed, I'm sure of it.\nFirst we get 5 men buried under rubble, then some disease gets the whole village down with fever.\nNow the guys we send deeper into the cave's passages come back with some insane reports of creatures looking at them in the dark.\nWhat creatures can there be? It's a damn empty cave.\nI went down the passages. All of them. There's nothing there.\nI swear I'll leave this place, if they keep going on about all that nonsense.\nThe disease seems to be gone now, we made a small graveyard for the people we lost north of the pond.\nWe'll keep digging now.");
				clrscr();
				animate_text("Day 59:\n\nI've seen one of them. At least, I think I saw it?\nI don't know. I swear nothing was there, but I saw it.\nIt's as if I wasn't looking at it with my eyes.\nI'm afraid I was right about the curse...");
				clrscr();
				animate_text("Day 73:\n\nWe found a corpse.\nIt was... odd, to say the least.\nSomehow I'm convinced I know the poor bastard. All the guys are saying the same thing.\nAll the same, none of us know who that is.\nWe tried moving it, but we kept getting back to *thinking* of moving it.\nNone of that makes any sense, and I cannot put it to words.\nGuys think we should leave this place, but I know we can't.\nWe'll be executed as traitors after they kill our families, if we come back without enough ore.\nAnd we don't have enough ore yet.\nWe will keep digging. What other choice we have?");
				clrscr();
				animate_text("Day 78:\n\nPeople are vanishing. We can't find any of those that disappear.\nSome are saying that they went to the mines.\nWe checked the damn mines - nodoby's there.\nWe can't find any bodies, nor are there any signs anybody went down there. Or anywhere, for that matter.\nWe still don't have enough ore, so it might be best to just try our luck in the wilds.\nNobody wants to stay in this place.");
				clrscr();
				animate_text("Day 80:\n\nFinal entry.\nDecided to abandon the village and go out south.\nWe'll try our luck in the coastal villages.\nThey're always in need of fishing crews, or so I heard.\nWhatever happens to us now, it can't be worse than staying here.");
				clrscr();
				animate_text("Day 81:\n\nWe couldn't leave.\nNone of us could.\nThe place didn't let us go.\nWe headed south after we'd left the village. But we didn't go anywhere.\nSome went running, and... none of them so much as moved.\nIt's like we're pulled back in by this cursed land.\nDamn it all to hell, I should've never come here...");
				clrscr();
				animate_text("Day 93:\n\nThere're some damn shrines built all around the village now.\nFigures made of bone are found buried in the ground near the huts.\nWe think some of us might've organized some kind of a coven.\nAs if there wasn't enough trouble already.\nNone come forward to admit they did this.\nPeople's spirits are broken. All of us are going mad here.");
				clrscr();
				animate_text("Day 127:\n\nThe food ran out.\nWe were supposed to receive new provisions on our third month here. Nobody came.\nWe can't go to the woods to hunt. They are in the woods.\nThe children are there, too.\nThey screamed, but none came to rescue them.\nWe heard them drown in their own blood, but none of us left the huts.\nNone of us will make it out.\nWe're all just offerings.");
				clrscr();
				animate_text("Day 149:\n\nI wanted to escape through the side-tunnels, but I ended up in the main cave no matter which way I went.\nEventually I just started wandering around without any idea where I was going.\nAs soon as I though that I'm lost beyond hope, I came to the burial cave.\nThe corpse spoke to me. It tried to, at least.\nAll I heard was screams, agonized wailing, and maddened howls.\nWhatever tried to reach out to me had no words.\nI felt terror, and tried to run, but I just stood there each time I though of making any movements.\nThen... I don't know what happened then, but I was in front of my hut, when I came to.\nThere were people standing around the hut, chanting. \"Return\", they chanted.\nI went into the hut, locked the doors.\nThink I'll just wait for a while, see what happens...");
				clrscr();
				animate_text("\n\nSome wanted to leave again.\nBuilding walls.\nNobody must leave now.\nWe're close now, I feel it. We just need a bit more time.\nIt will come back.\nWe can finally see it.\nWe'll all finally become one when it comes back.\nComing here was a blessing.\nWe're all blessed.");
				clrscr();
				animate_text("After you finish reading, the book finally crumbles to pieces.\nWhatever is left of it is unreadable.");
				current_map[4][3] = ' ';
				break;
			case 50:
				clrscr();
				animate_text("You place the book back to the pile carefully.");
				break;
			}
		}
	}

	else if (npc == '<')
	{
		if (current_map_title == "Graveyard" && !explosive_ready && !can_use_magic)
		{
			animate_text("The passageway ahead is blocked.\nYou can't move the blockage with your hands alone.");
		}

		else if (current_map_title == "Graveyard" && !explosive_ready && can_use_magic && has_powder)
		{
			animate_text("The passageway ahead is blocked.\nYou can't move the blockage with your hands alone.\nYou can try summoning a fire elemental to try and ignite the black powder pouch.");
			std::cout << "\n\n1. Summon the elemental.\n2. Best not to risk it.";
			bool option_chosen{ false };
			while (!option_chosen)
				switch (readInput())
				{
				case 49:
					clrscr();
					option_chosen = true;
					game_over = true;
					game_over_screen("blown");
					break;
				case 50:
					clrscr();
					option_chosen = true;
					animate_text("You shake the idea off.\nBetter safe than sorry.");
					break;
				}
		}

		else if (current_map_title == "Graveyard" && explosive_ready)
		{
			animate_text("You place the pouch with the powder and the fuse between the boulders.");
			play_sound("Flint strike");
			animate_text("\nA couple of strikes against the flint with a stone, and the necessary ingnition is produced.");
			play_sound("Fuse");
			animate_text("\nThe fuse starts burning rapidly.\nYou hurry to take cover in behind the nearby tree line.");
			play_sound("Explosion");
			animate_text("\nMoments after you duck behind the trees, a powerful explosion goes off.\n");
			current_map[11][22] = ' ';
		}

		else if (current_map_title == "Abandoned Village" && !explosive_ready)
		{
			animate_text("The way back collapsed. There's no going through it again.");
		}

		else if (current_map_title == "Abandoned Village" && explosive_ready)
		{
			animate_text("You place the readied pouch in the blockage.\n");
			play_sound("Flint strike");
			animate_text("You strike the flint with a stone.\nYet the fuse won't light despite your best efforts.");
		}
	}


	else if (npc == '~')
	{
		if (water_interact == 0)
		{
			clrscr();
			animate_text("The water's surface is still.\nNothing but an occasional ripple produced by a dragonfly leaving its resting place atop a lonely scirpus.");
			water_interact++;
		}
		else if (water_interact == 1)
		{
			clrscr();
			animate_text("This giant mirror starts looking eerie.\nYou feel as though the pond is pulling you towards itself.\nAs if the water's surface is bending to angles at which you'd go under.\nAfter a while, you realize that you've been staring at the pond for what feels like hours.\nYou feel uneasy thinking about the water in this pond.");
			water_interact++;
		}
		else if (water_interact == 2)
		{
			clrscr();
			if (player_hp_current < player_hp_max)
			{
				animate_text("You force yourself to stare at the pond, entranced once more.\nYour head hurts a little, but you notice that the bruises you had gotten earlier don't hurt so much now.");
				player_hp_current++;
				water_interact++;
			}
			else
			{
				animate_text("You force yourself to stare at the pond, entranced once more.\nYour head hurts a little.\nYou wonder if there's a point to this vanity.");
				player_san_current--;
				water_interact++;
			}
		}
		else if (water_interact == 3)
		{
			clrscr();
			if (player_hp_current < player_hp_max)
			{
				animate_text("The thoughts of throwing yourself in the pond fill your head for whatever reason.\nYour body feels invigorated, while your mind is drowning.");
				player_san_current -= 2;
				player_hp_current++;
				water_interact++;
			}
			else
			{
				animate_text("The thoughts of throwing yourself in the pond fill your head for whatever reason.");
				player_san_current -= 2;
				water_interact++;
			}
		}
		else if (water_interact > 3)
		{
			clrscr();
			if (player_hp_current < player_hp_max)
			{
				animate_text("Your body feels invigorated, while your mind is drowning.");
				player_san_current -= 2;
				player_hp_current++;
			}
			else
			{
				animate_text("You're overcome with a desire to get to the bottom of this pond.");
				player_san_current -= 2;
			}
		}
		if (player_san_current <= 0)
		{
			game_over = true;
			game_over_screen("drown");
		}
	}


	else if (npc == 'M')
	{
		if (!seen_m && current_map_title == "Silent Pond")
		{
			play_sound("NPC theme");
			animate_text("As you step into the hut, you notice a figure crouched in front of the fireplace.\nThe man seems not to notice you at first, but then tilts his head a little as if alarmed by something.\nThe man slowly raises and turns towards you.");
			
			clrscr();
			animate_text("A-ah... took you long enough.");
			std::cout << "\n\n1. Waiting for someone?\n2. Who're you talking to?\n3. Sorry, had to take the scenic route.";
			switch (readInput())
			{
			case 49:
				clrscr();
				animate_text("Not anymore I'm not. Seeing as how you decided to finally show up.");
				break;

			case 50:
				clrscr();
				animate_text("Bumped with your head this time, eh? I'm talking to you, dimwit.");
				break;

			case 51:
				clrscr();
				animate_text("Heh. Got some humor in you now. That's good.");
				break;
			}

			std::cout << "\n\n1. I think you got me confused with someone.";
			readInput();
			clrscr();
			animate_text("Oh, I don't think so, lad.\nI think I haven't been confused by anything - or anyone - for a long, long time now.\nSince we *haven't* met, name's Mercil.\nAnd since you finally showed up, we can get to the fun part.");
			clrscr();
			animate_text("The man's expression turns wicked.\nA smile cuts his face as if he'd remembered something exciting.");
			clrscr();
			animate_text("Say, you haven't seen that hole in the middle of the pond yet, haven't you?");
			std::cout << "\n\n1. Don't know what you're going on about - there's nothing in that pond.\n2. I have. Wasn't impressed.";
			switch (readInput())
			{
			case 49:
				clrscr();
				animate_text("Oh, but I bet there is. Why don't you go take a look?\nI'll wait for you here.");
				break;

			case 50:
				clrscr();
				animate_text("Lies will get you nowhere, flotsam.\nGo get a look at that pond again.\nI'll wait for you here.");
				break;
			}
			seen_m = true;
			current_map[12][12] = ' ';
			current_map[12][11] = ' ';
			current_map[12][10] = 'o';
		}

		else if (seen_m && current_map_title == "Silent Pond")
		{
			if (!seen_hole)
			{
				animate_text("The man stands with his arms crossed. He seems to be intrigued by what you will do next.");
			}
			else
			{
				play_sound("NPC theme");
				animate_text("The man turns to you right as you enter. He seems to be overjoyed by something.");
				clrscr();
				animate_text("You're back! Perfect, just perfect... yes, you will do nicely...");
				std::cout << "\n\n1. What the hell was that hole?\n2. Start explaining all of this. Now.\n3. Do nicely for what?";
				switch (readInput())
				{
				case 49:
					clrscr();
					animate_text("Haven't you guessed? It's your purpose! It's what you've been born for!\nThat hole is your beginning. It is your end!");
				break;
				case 50:
					clrscr();
					animate_text("No need to get impatient now, flotsam. You *will* learn of it.\nThat's your purpose, you know!");
				break;
				case 51:
					clrscr();
					animate_text("For your purpose! Your meaning! What you lacked all this time!");
				break;
				}
				clrscr();
				animate_text("The man seems to be filled with anticipation.");
				clrscr();
				animate_text("A-ah! I can finally see it. Yes! I can finally return...");
				std::cout << "\n\n1. You're not making any sense.";
				readInput();
				clrscr();
				animate_text("Forget sense! None of what you brought from outside matters now.\nNow go! There's festivities being prepared for you in the village.\nMake sure to talk to everybody before leaving, heh-heh-heh...");
				clrscr();
				animate_text("The man looks back at the fireplace. His face betrays regret, sorrow.\nHe starts walking towards the exit.");
				std::cout << "\n\n1. Hey! I'm not letting you out of here until I get some answers! (block exit).\n2. Can't say I enjoyed your company, but farewell.\n3. (remain silent)";
				switch (readInput())
				{
				case 49:
					clrscr();
					if (player_title == "Veteran")
					{
						animate_text("The man stops about a step away from you.\nJust as you are about to start talking, the man throws a hook.\nYears of brawling on the frontlines allow you to duck under the punch at the last moment.");
						clrscr();
						animate_text("Not bad, flotsam.\nStill, I *really* do need to get going.\nI'd say that you have your hands full already, don't you think?");
						clrscr();
						animate_text("You agree with the man.\nNo need to spill any blood just yet.");
					}
					else
					{
						animate_text("The man stops about a step away from you.\nJust as you are about to start talking, the man throws a hook.\nThe speed of the punch is too much for you, and you fail to react.");
						clrscr();
						animate_text("Don't think you're the only one who can dish out some pain, flotsam.\nLet it go - for your own sake.");
						clrscr();
						animate_text("You agree with the man.\nBest you keep risks to a minimum.");
						player_hp_current -= 2;
						clrscr();
					}
				break;
				case 50:
					clrscr();
					animate_text("Don't get too lively there, flotsam.\nYou have a bleak road ahead of you.\nAnd I *will* see you again, heh-heh.");
				break;
				case 51:
					clrscr();
					animate_text("Well, don't swallow your tongue just yet!\nSince we *will* be seeing each other again, I'd like you to be able to talk properly, when we do.");
				break;
				}
				clrscr();
				animate_text("The man steps out of the hut, and it feels like he never was in it.\nYou get outside. The man is nowhere to be found.\nYou hear something off in the village to the west.");
				current_map[2][20] = ' ';
				x = 17;
				y = 2;
				seen_m = false;
				abandoned_village[10][18] = '*';
				abandoned_village[1][8] = '*';
				abandoned_village[5][1] = '*';
				abandoned_village[15][3] = '%';
				abandoned_village[5][3] = '&';
				abandoned_village[5][9] = '&';
				
			}
		}
		else if (current_map_title == "Graveyard")
		{
			if (!seen_m)
			{
				play_sound("NPC theme");
				animate_text("You look like you got into a fight with a monster or something, hah-hah!\nDidn't fancy the welcoming festivities, I reckon?");
				std::cout << "\n\n1. You're some kind of a monster too, aren't you?\n2. Tell me how I can get out of this place or I'll rip your head off.\n3. Had some good time. The company was a bit rough, though.";
				switch (readInput())
				{
				case 49:
					clrscr();
					animate_text("Do I look like one?");
					clrscr();
					animate_text("The man's expression puzzles you.\nIt's as if the features of his face aren't the same they were a moment ago.");
					clrscr();
					animate_text("Didn't think so, heh-heh.\nYou'll find that monsters aren't the scaries thing in the world, flotsam.\nYou just need to go a bit deeper.");
					break;
				case 50:
					clrscr();
					animate_text("Frightening words!\n");
					clrscr();
					animate_text("The man breaks into a fit of crackling laughter.");
					clrscr();
					animate_text("O-oh, that was good.\nYou'd make a fine jester, flotsam.\nDon't go saying things like that again, though - a friendly warning.");
					break;
				case 51:
					clrscr();
					animate_text("Heh-heh-heh, staying bright is good, flotsam.\nYou keep that up and maybe you won't mind what's in stock for you that much! Hah-hah-hah!");
					break;
				}
				clrscr();
				animate_text("Still, you *do* need to know how to get out of here, though.\nThere's a passageway further east of the graveyard.\nIt was blocked with some pretty heavy boulders, so I reckon you won't be able to get through without some black powder.\nGood thing you have some on you, then!\nNow you just need a flint and some fuse.\nI'd look around the graveyard, if I was you, heh-heh.");
				clrscr();
				animate_text("The man's expression changes, betraying a sliver of sanity; it turns sorrowful.");
				clrscr();
				animate_text("Now go, leave me be.\nI need to say some goodbyes here.");
				seen_m = true;
			}
			else
			{
				clrscr();
				animate_text("... the man doesn't appear to react to your presence in any way.");
			}
		}
	}
	

	//wall interactions
	else if (npc == '-' || npc == '|')
	{
		if (wall_interact == 0)
		{
			clrscr();
			animate_text("A wall.");
			wall_interact++;
		}
		else if (wall_interact == 1)
		{
			clrscr();
			animate_text("This is a wall.");
			wall_interact++;
		}
		else if (wall_interact == 2)
		{
			clrscr();
			animate_text("The wall looks simply made with stones laid on top of each other.");
			wall_interact++;
		}
		else if (wall_interact == 3)
		{
			clrscr();
			animate_text("There's some clay-like material bonding the rocks and pebbles used to make the wall.\nIt glistens as if sprinkled with water, although the air is fairly dry.");
			wall_interact++;
		}
		else if (wall_interact == 4)
		{
			clrscr();
			animate_text("Looking at the wall, you realize that - maybe - sometimes you can make all your troubles go away by doing nothing.\nMaybe all it takes is a little patience and a bit of idle-handedness.\nYou wonder if you can find a way out of this place by just looking at this wall.\n.\n.\n.\nBut nothing happens...");
			wall_interact++;
		}
		else
		{
			clrscr();
			animate_text("The wall looks back at you without provoking any profound ponderings.");
		}

	}

	//tree interactions
	else if (npc == '^')
	{
		if (tree_interact == 0)
		{
			clrscr();
			animate_text("A tree.");
			tree_interact++;
		}
		else if (tree_interact == 1)
		{
			clrscr();
			animate_text("This tree looks like a regular weeping elm.");
			tree_interact++;
		}
		else if (tree_interact == 2)
		{
			clrscr();
			animate_text("On second inspection, the leaves seem to be dark as if the tree deprived its subjects of any color.");
			tree_interact++;
		}
		else if (tree_interact == 3)
		{
			clrscr();
			animate_text("While staring at the leaves, you suddenly notice your hand placed on the tree's trunk.\nIt seems to be... beating.\nThe rhythm is steady, much like that of your own heart.\nConfused, you take your hand off.");
			tree_interact++;
		}
		else if (tree_interact == 4)
		{
			clrscr();
			animate_text("You place your hand against the tree's bark again.\nThe steady beat of the bark's surface calms you, resonating with your heartbeat.\nThe edges of your vision become hazy, uneven.\nParticulars of your surroundings fade until only the outline of reality is present.\nYou hear a faint chanting.\nLeaning closer to the bark makes the chanting clearer, but you still can't make out any words.");
			std::cout << "\n\n1. Lean closer.\n2. Take your hand off.";
			switch (readInput())
			{
			case 49:
				clrscr();
				animate_text("The chanting grows clearer.\nYou now hear the voices as if standing next to the congregation producing the chant.\nNeither words, nor the melody of the chant make sense to you.\nConfused at first, you soon realize that your mind is being filled with visions of overgrown bogs, rotted trees, swarms of insects covering the sky until no light is left.\nDespite the imagery, you feel calm. There's oneness inside you.\nYou fall asleep.");
				game_over = true;
				game_over_screen("trees");
				break;
			case 50:
				clrscr();
				animate_text("You take your hand off the bark.\nYou feel as if a piece of your own self was absorbed by the tree.\nSuch carelessness might cost you your life, you reckon.");
				tree_interact++;
				player_san_current -= 5;
				tree_color = 2;
				break;
			}
		}
		else
		{
			clrscr();
			animate_text("The tree makes you feel unsafe. You decide not to stare at it for too long.");
		}
	}
	if (player_san_current <= 0)
	{
		game_over = true;
		game_over_screen("sanity loss exploration");
	}
}


//all interactions are managed here
std::vector<std::string> interact(char object, std::vector<std::string> inventory, int x, int y, std::string map_title)
{

	//NPC interactions
	if (object == 'M' || object == 'o' || object == '#' || object == '@' || object == 't' || object == '<' || object == '~' || object == '|' || object == '-' || object == '^')
	{
		npc_interaction(object, x, y);
	}

	//item pickups
	else if (object == 'x')
	{
		inventory = pickup(inventory, x, y, map_title);
	}
	return inventory;
}



//drawing maps (printing the map values, coloring them also)
void draw_map(std::vector<std::vector<char>> map)
{
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	for (int i = 0; i < map.size(); i++)
	{
		for (int j = 0; j < map[i].size(); j++)
		{
			switch (map[i][j])
			{
			case 94:
				SetConsoleTextAttribute(hConsole, 10);
				break;
			case 126:
				SetConsoleTextAttribute(hConsole, tree_color);
				break;
			case 37:
				SetConsoleTextAttribute(hConsole, 4);
				break;
			case 38:
				SetConsoleTextAttribute(hConsole, 4);
				break;
			case 42:
				SetConsoleTextAttribute(hConsole, 4);
				break;
			case 40:
				SetConsoleTextAttribute(hConsole, 8);
				break;
			case 41:
				SetConsoleTextAttribute(hConsole, 8);
				break;
			case 61:
				SetConsoleTextAttribute(hConsole, 8);
				break;
			case 64:
				SetConsoleTextAttribute(hConsole, 12);
				break;
			case 35:
				SetConsoleTextAttribute(hConsole, 6);
				break;
			case 120:
				SetConsoleTextAttribute(hConsole, 11);
				break;
			case 111:
				SetConsoleTextAttribute(hConsole, 4);
				break;
			default:
				SetConsoleTextAttribute(hConsole, 15);
			}
			std::cout << map[i][j] << " ";
		}
		std::cout << std::endl;
	}
}



//the "main" function of functions
bool gameplay()
{
	
	current_map = abandoned_village;
	current_map_title = "Abandoned Village";
	current_map[19][10] = player_model;
	y = 19;
	x = 10;

	while (!game_over)
	{
		clrscr();
		std::cout << current_map_title << std::endl << std::endl;
		std::cout << "Health " << player_hp_current << "/" << player_hp_max << std::endl;
		std::cout << "Sanity " << player_san_current << "/" << player_san_max << std::endl << std::endl;
		draw_map(current_map);

		switch (readInput())
		{
			//up
		case 72:
			if (y > 0 && (current_map[y - 1][x] == ' ' || current_map[y - 1][x] == player_model || current_map[y - 1][x] == '%' || current_map[y - 1][x] == '&' || current_map[y - 1][x] == '*' || (current_map[y - 1][x] == 'x' && inventory.size() < 6)))
			{
				play_sound("Step");
				if (current_map[y - 1][x] == '&' || current_map[y - 1][x] == '%' || current_map[y - 1][x] == '*')
				{
					combat(current_map[y - 1][x]);
				}
				else if (current_map[y - 1][x] == 'x' && inventory.size() <= 6)
				{
					inventory = interact(current_map[y - 1][x], inventory, x, y - 1, current_map_title);
				}
				y--;
				current_map[y][x] = player_model;
				current_map[y + 1][x] = ' ';
			}
			else if (current_map_title == "Silent Pond" && y == 0)
			{
				silent_pond = current_map;
				current_map = graveyard;
				current_map_title = "Graveyard";
				current_map[23][3] = player_model;
				y = 23;
				x = 3;
				wall_interact = 0;
			}
			break;

			//down
		case 80:
			if (y < current_map.size() - 1 && (current_map[y + 1][x] == ' ' || current_map[y + 1][x] == player_model || current_map[y + 1][x] == '%' || current_map[y + 1][x] == '&' || current_map[y + 1][x] == '*' || (current_map[y + 1][x] == 'x' && inventory.size() < 6)))
			{
				play_sound("Step");
				if (current_map[y + 1][x] == '&' || current_map[y + 1][x] == '%' || current_map[y + 1][x] == '*')
				{
					combat(current_map[y + 1][x]);
				}
				else if (current_map[y + 1][x] == 'x' && inventory.size() <= 6)
				{
					inventory = interact(current_map[y + 1][x], inventory, x, y + 1, current_map_title);
				}
				y++;
				current_map[y][x] = player_model;
				current_map[y - 1][x] = ' ';
			}
			else if (current_map_title == "Graveyard" && y == 23)
			{
				graveyard = current_map;
				current_map = silent_pond;
				current_map_title = "Silent Pond";
				current_map[0][8] = player_model;
				y = 0;
				x = 8;
				wall_interact = 0;
			}
			break;

			//left
		case 75:
			if (x > 0 && (current_map[y][x - 1] == ' ' || current_map[y][x - 1] == player_model || current_map[y][x - 1] == '%' || current_map[y][x - 1] == '&' || current_map[y][x - 1] == '*' || (current_map[y][x - 1] == 'x' && inventory.size() < 6)))
			{
				play_sound("Step");
				if (current_map[y][x - 1] == '&' || current_map[y][x - 1] == '%' || current_map[y][x - 1] == '*')
				{
					combat(current_map[y][x - 1]);
				}
				else if (current_map[y][x - 1] == 'x' && inventory.size() <= 6)
				{
					inventory = interact(current_map[y][x - 1], inventory, x - 1, y, current_map_title);
				}
				x--;
				current_map[y][x] = player_model;
				current_map[y][x + 1] = ' ';
			}
			else if (current_map_title == "Silent Pond" && x == 0)
			{
				silent_pond = current_map;
				current_map = abandoned_village;
				current_map_title = "Abandoned Village";
				current_map[10][current_map.size() - 1] = player_model;
				y = 10;
				x = current_map.size() - 1;
				wall_interact = 0;
			}
			break;

			//right
		case 77:
			if (current_map_title == "Graveyard" && x == 22)
			{
				game_over = true;
				game_over_screen("demo-end");
			}
			else if (current_map_title == "Abandoned Village" && x == current_map.size() - 1)
			{
				abandoned_village = current_map;
				current_map = silent_pond;
				current_map_title = "Silent Pond";
				current_map[11][0] = player_model;
				y = 11;
				x = 0;
				wall_interact = 0;
			}
			else if (x < current_map.size() - 1 && (current_map[y][x + 1] == ' ' || current_map[y][x + 1] == player_model || current_map[y][x + 1] == '%' || current_map[y][x + 1] == '&' || current_map[y][x + 1] == '*' || (current_map[y][x + 1] == 'x' && inventory.size() < 6)))
			{
				play_sound("Step");
				if (current_map[y][x + 1] == '&' || current_map[y][x + 1] == '%' || current_map[y][x + 1] == '*')
				{
					combat(current_map[y][x + 1]);
				}
				else if (current_map[y][x + 1] == 'x' && inventory.size() <= 6)
				{
					inventory = interact(current_map[y][x + 1], inventory, x + 1, y, current_map_title);
				}
				x++;
				current_map[y][x] = player_model;
				current_map[y][x - 1] = ' ';
			}
			break;

			//interact
		case 101:
			if (current_map[y][x - 1] == 'M' || current_map[y][x - 1] == 'o' || current_map[y][x - 1] == 't' || current_map[y][x - 1] == '@' || current_map[y][x - 1] == '#' || current_map[y][x - 1] == '<' || current_map[y][x - 1] == '~' || current_map[y][x - 1] == '-' || current_map[y][x - 1] == '|' || current_map[y][x - 1] == '^')
			{
				inventory = interact(current_map[y][x - 1], inventory, x - 1, y, current_map_title);
			}
			else if (current_map[y - 1][x] == 'M' || current_map[y - 1][x] == 'o' || current_map[y - 1][x] == 't' || current_map[y - 1][x] == '@' || current_map[y - 1][x] == '#' || current_map[y - 1][x] == '<' || current_map[y - 1][x] == '~' || current_map[y - 1][x] == '-' || current_map[y - 1][x] == '|' || current_map[y - 1][x] == '^')
			{
				inventory = interact(current_map[y - 1][x], inventory, x, y - 1, current_map_title);
			}
			else if (current_map[y + 1][x] == 'M' || current_map[y + 1][x] == 'o' || current_map[y + 1][x] == 't' || current_map[y + 1][x] == '@' || current_map[y + 1][x] == '#' || current_map[y + 1][x] == '<' || current_map[y + 1][x] == '~' || current_map[y + 1][x] == '-' || current_map[y + 1][x] == '|' || current_map[y + 1][x] == '^')
			{
				inventory = interact(current_map[y + 1][x], inventory, x, y + 1, current_map_title);
			}
			else if (current_map[y][x + 1] == 'M' || current_map[y][x + 1] == 'o' || current_map[y][x + 1] == 't' || current_map[y][x + 1] == '@' || current_map[y][x + 1] == '#' || current_map[y][x + 1] == '<' || current_map[y][x + 1] == '~' || current_map[y][x + 1] == '-' || current_map[y][x + 1] == '|' || current_map[y][x + 1] == '^')
			{
				inventory = interact(current_map[y][x + 1], inventory, x + 1, y, current_map_title);
			}
			break;

			//inventory
		case 105:
			inventory = manage_inventory(inventory);
		}

		clrscr();
	}

	if (game_over == true)
	{
		game_over_screen(game_over_text);
		return false;
	}
}


void clrscr()
{
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X * csbi.dwSize.Y;

	if (!FillConsoleOutputCharacter(
		hStdOut,
		(TCHAR)' ',
		cellCount,
		homeCoords,
		&count
	)) return;

	if (!FillConsoleOutputAttribute(
		hStdOut,
		csbi.wAttributes,
		cellCount,
		homeCoords,
		&count
	)) return;

	SetConsoleCursorPosition(hStdOut, homeCoords);
}

int readInput()
{
	int input = _getch();
	switch (input)
	{
	case 224:
		input = _getch();
		break;
	}
	return input;
}