#pragma once
#include "game.h"

bool displayMenu = true;
void menuInput(const char* input)
{
	if (input[0] == '/' && input[1] == 'p')
	{
		gameReset();
		runGame = true;
	}
	else if (input[0] == '/' && input[1] == 'e')
	{
		play = false;
	}
	else if (input[0] == '/' && input[1] == 's')
	{
		displayMenu = false;
	}
	else if (input[0] == '/' && input[1] == 'm')
	{
		displayMenu = true;
	}
	else
	{
		return;
	}
}

char username[17] = "";
int score = 0;
char scoreStr[11] = "";
void readScore()
{
	FILE* fp;
	fp = fopen("scoreRecord.txt", "r");
	fscanf(fp, "%s,%d\n", &username, &score);
	fclose(fp);
	//_itoa_s(score, scoreStr, 10);
	//scoreStr[0] = (score % 10) + '0';
}

void menuUpdate()
{
	GetNumberOfConsoleInputEvents(console.rHnd, &numEvents);
	if (numEvents != 0) {
		INPUT_RECORD* eventBuffer = new INPUT_RECORD[numEvents];
		ReadConsoleInput(console.rHnd, eventBuffer, numEvents, &numEventsRead);
		for (DWORD i = 0; i < numEventsRead && i < numEvents; ++i)
		{
			if (eventBuffer[i].EventType == KEY_EVENT && eventBuffer[i].Event.KeyEvent.bKeyDown == true)
			{
				if (type_command(eventBuffer[i].Event.KeyEvent.uChar.AsciiChar, eventBuffer[i].Event.KeyEvent.wVirtualKeyCode))
				{
					menuInput(cmd);
					clear_command();
				}
			}
		}
		delete[] eventBuffer;
	}
}

void menuRender()
{
	if (displayMenu)
	{
		draw_console(menu, { 0,0 }, 10);
		gotoxy(9 + strlen(cmd), 21);
		draw_console(cmd, { 9,21 }, 10);
	}
	else
	{
		draw_console(scoreboard, { 0,0 }, 10);
		draw_console(username, { 7,3 }, 10);
		//draw_console(scoreStr, { 10,3 }, 10);
		gotoxy(9 + strlen(cmd), 21);
		draw_console(cmd, { 9,21 }, 10);
	}
}
