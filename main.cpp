#include "fnc.h"
#include <iostream>
#include <chrono>

int main()
{
	//animate_text("Testing. Testing. Testing.");
	bool game{ true };
	bool show{ false };
	bool option_chosen{ false };
	std::string intro_select{"Play intro? (y/n)"};

	play_sound("Theme 1");
	std::cout << "Anaphora.\n\n1. New game\n\n2. Exit\n";

	while (game)
	{
		if (readInput() == 49)
		{
			play_sound("Select");
			clrscr();
			for (int i = 0; i < intro_select.size(); i++)
			{
				std::cout << intro_select[i];
				if (intro_select[i] == '?')
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(600));
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(45));
			}

			while (!option_chosen)
			{
				switch (readInput())
				{
				case 121:
					option_chosen = true;
					play_sound("Select");
					show = true;
					break;

				case 110:
					option_chosen = true;
					play_sound("Select");
					show = false;
					break;
					play_sound("Select");
				default:
					option_chosen = false;
				}
			}
			intro(show);
			game = gameplay();
		}
		else if (readInput() == 50)
		{
			game = false;
			return 0;
		}
	}
}