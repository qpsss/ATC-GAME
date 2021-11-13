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
			if (loseGame)
			{
				loseUpdate();
				loseRender();
				if (gotoScoreboard)
				{
					displayMenu = false;
				}
			}
			else
			{
				gameUpdate();
				gameRender();
			}
		}
		else
		{
			menuUpdate();
			readRecord();
			sortRecord();
			readSortedRec();
			menuRender();
		}
		display_console();
		Sleep(dt);
	}
	return 0;
}