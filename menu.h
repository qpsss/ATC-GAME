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
		gotoScoreboard = false;
	}
	else
	{
		return;
	}
}

struct Player
{
	char username[14];
	int score;
};
Player player[100];
int totalRec = 0;
char scoreStr[11] = "";

void readRecord()
{
	FILE* fp;
	fp = fopen("scoreRecord.txt", "r");
	int i = 0;
	while (!feof(fp))
	{
		fscanf(fp, "%s , %d", &player[i].username, &player[i].score);
		i++;
	}
	totalRec = i - 1;
	fclose(fp);
}

void sortRecord()
{
	int i, j, tempScore;
	char tempName[14] = "";
	for (i = 0; i < totalRec; i++)
	{
		for (j = i + 1; j < totalRec; j++)
		{
			if (player[i].score < player[j].score)
			{
				tempScore = player[i].score;
				strcpy_s(tempName, 14, player[i].username);
				player[i].score = player[j].score;
				strcpy_s(player[i].username, 14, player[j].username);
				player[j].score = tempScore;
				strcpy_s(player[j].username, 14, tempName);
			}
		}
	}
	FILE* fp;
	fp = fopen("sortedScore.txt", "w");
	for (int i = 0; i < totalRec; i++)
	{
		fprintf(fp, "%s , %d\n", player[i].username, player[i].score);
	}
	fclose(fp);
}

void readSortedRec()
{
	FILE* fp;
	fp = fopen("sortedScore.txt", "r");
	int i = 0;
	while (!feof(fp))
	{
		fscanf(fp, "%s , %d", &player[i].username, &player[i].score);
		i++;
	}
	totalRec = i - 1;
	fclose(fp);
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
		/*draw_console(fakemap, {0,0}, 10);
		draw_console("p7", {6,7}, 160);
		draw_console("E9", { 38,10 }, 160);
		draw_console("a2", {24,6}, 160);
		draw_console("R7", { 8,15 }, 160);
		draw_console("&", { 16,18 }, 175);*/
		draw_console(menu, {0,0}, 10);
		draw_console("/play      ", {68,11}, 160);
		draw_console("/scoreboard", { 68,12 }, 160);
		draw_console("/exit      ", {68,13}, 160);
		gotoxy(9 + strlen(cmd), 21);
		draw_console(cmd, { 9,21 }, 10);
	}
	else
	{
		draw_console(scoreboard, { 0,0 }, 10);
		draw_console("    SCOREBOARD                                                                               ", { 0,1 }, 160);
		short posy = 3;
		if (totalRec < 9)
		{
			for (int i = 0; i < totalRec; i++)
			{
				draw_console(player[i].username, { 7,posy }, 10);
				_itoa_s(player[i].score, scoreStr, 10);
				draw_console(scoreStr, { 21,posy }, 10);
				posy += 2;
			}
		}
		else
		{
			for (int i = 0; i < 9; i++)
			{
				draw_console(player[i].username, { 7,posy }, 10);
				_itoa_s(player[i].score, scoreStr, 10);
				draw_console(scoreStr, { 21,posy }, 10);
				posy += 2;
			}
		}
		gotoxy(9 + strlen(cmd), 21);
		draw_console(cmd, { 9,21 }, 10);
	}
}
