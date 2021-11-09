#include "menu.h"

int main()
{
	srand(time(NULL));
	init_console();
	init_airliner_jet();
	while (play)
	{
		clear_console();
		if (runGame)
		{
			gameUpdate();
			gameRender();
		}
		else
		{
			menuUpdate();
			menuRender();
		}
		display_console();
		Sleep(dt);
	}
	return 0;
}