#pragma once
#include "game.h"

void menuInput(const char* input)
{
	if (input[0] == '/' && input[1] == 'p')
	{
		runGame = true;
	}
	else if (input[0] == '/' && input[1] == 'e')
	{
		play = false;
	}
	else
	{
		return;
	}
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
	draw_console(menu, { 0,0 }, 10);
	gotoxy(9 + strlen(cmd), 21);
	draw_console(cmd, { 9,21 }, 10);
}