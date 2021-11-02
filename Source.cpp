#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <string>
#include<string.h>
#define screen_x 90 //85
#define screen_y 35 //22
#define dt 100
bool play = true;
unsigned int safe = 0;

int randrange(int start, int stop)
{
	return rand() % (stop - start + 1) + start;
}
#pragma region console
struct Console
{
	HANDLE wHnd;
	HANDLE rHnd;
	CHAR_INFO buffer[screen_x * screen_y];
	COORD bufferSize = { screen_x,screen_y };
	COORD characterPos = { 0,0 };
	SMALL_RECT windowSize = { 0,0,screen_x - 1,screen_y - 1 };
};
Console console;

void init_console()
{
	/*int newWidth = 9, newHeight = 20;
	CONSOLE_FONT_INFOEX fontStructure = { 0 };
	fontStructure.cbSize = sizeof(fontStructure);
	fontStructure.dwFontSize.X = newWidth;
	fontStructure.dwFontSize.Y = newHeight;
	wcscpy(fontStructure.FaceName, L"Consolas");
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetCurrentConsoleFontEx(hConsole, true, &fontStructure); */
	console.wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	console.rHnd = GetStdHandle(STD_INPUT_HANDLE);
	SetConsoleWindowInfo(console.wHnd, TRUE, &console.windowSize);
	SetConsoleScreenBufferSize(console.wHnd, console.bufferSize);
	SetConsoleMode(console.rHnd, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT);
	SetConsoleTitleA("ATC");
	//system("pause");
}
void gotoxy(int x, int y)
{
	SetConsoleCursorPosition(console.wHnd, { short(x), short(y) });
}
void clear_console()
{
	for (int i = 0; i < screen_x * screen_y; i++)
	{
		console.buffer[i].Char.AsciiChar = ' ';
		console.buffer[i].Attributes = 7;
	}
}
void display_console()
{
	WriteConsoleOutputA(console.wHnd, console.buffer, console.bufferSize, console.characterPos, &console.windowSize);
}
void draw_console(const char* str, COORD pos, short color = 7)
{
	int x = pos.X;
	int y = pos.Y;
	for (int i = 0; i < strlen(str); i++)
	{
		if (str[i] == '\n')
		{
			x = pos.X;
			y++;
			continue;
		}
		if (0 <= x && x < screen_x && 0 <= y && y < screen_y)
		{
			console.buffer[x + screen_x * y].Char.AsciiChar = str[i];
			console.buffer[x + screen_x * y].Attributes = color;
		}
		x++;
	}
}

long inGameTime = 0;
void displayTime()
{
	inGameTime += dt;
	char draw_time[10] = "";
	if ((inGameTime) / 1000 <= 60)
	{
		draw_time[0] = '0' + inGameTime / 10000;
		draw_time[1] = '0' + (inGameTime / 1000) % 10;
	}
	else if ((inGameTime) / 1000 <= 3600)
	{
		draw_time[0] = '0' + inGameTime / 600000;
		draw_time[1] = '0' + (inGameTime / 60000) % 10;
		draw_time[2] = ':';
		draw_time[3] = '0' + (inGameTime / 10000) % 6;
		draw_time[4] = '0' + (inGameTime / 1000) % 10;
	}
	else if ((inGameTime) / 1000 > 3600)
	{
		draw_time[0] = '0' + inGameTime / 36000000;
		draw_time[1] = '0' + (inGameTime / 3600000) % 10;
		draw_time[2] = ':';
		draw_time[3] = '0' + (inGameTime / 600000) % 6;
		draw_time[4] = '0' + (inGameTime / 60000) % 10;
		draw_time[5] = ':';
		draw_time[6] = '0' + (inGameTime / 10000) % 6;
		draw_time[7] = '0' + (inGameTime / 1000) % 10;
	}
	draw_console(draw_time, { 67,0 }, 10);
}

#pragma endregion

#pragma region command
char cmd[7];
void clear_command()
{
	strcpy_s(cmd, 7, ""); //Copies one string to another
}
bool type_command(char ch, WORD vkCode)
{
	if (vkCode == VK_RETURN)
	{
		return true;
	}
	else if (vkCode == VK_BACK)
	{
		if (strlen(cmd) > 0)
		{
			cmd[strlen(cmd) - 1] = '\0';
		}
	}
	else if (strlen(cmd) < 6 && isprint(ch))
	{
		strncat_s(cmd, &ch, 1); //Concatenates a certain amount of characters of two strings
	}
	return false;
}
void processCommand(const char* input);
#pragma endregion

#pragma region plane
struct Plane
{
	bool active = false;
	COORD pos;
	COORD dir;
	char name;
	int altitude;
	int target_altitude;
	int dest;
	char type;
	// At beacon
	COORD beaconPoint;
	char instruction[4];
};
int airliner_count = 26;
int jet_count = 26;
Plane airliner[26];
Plane jet[26];
void init_plane(Plane* p, COORD pos, COORD dir, int dest, char name = '\0', int altitude = 7, char type = '\0')
{
	p->pos = pos;
	p->dir = dir;
	if (name != '\0')
	{
		p->name = name;
	}
	p->altitude = altitude;
	if (type != '\0')
	{
		p->type = type;
	}
	p->target_altitude = 7;
	p->beaconPoint = { -1,-1 };
	p->dest = dest;
}
void init_airliner_jet()
{
	for (int i = 0; i < 26; i++)
	{
		init_plane(airliner + i, { 0,0 }, { 0,0 }, 0, 'A' + i, 7, 'A');
		init_plane(jet + i, { 0,0 }, { 0,0 }, 0, 'a' + i, 7, 'J');
	}
}

COORD E_Point[8] = { COORD({24,0}), COORD({58,0}), COORD({58,7}), COORD({58,17}), COORD({18,20}), COORD({0,13}), COORD({0,7}), COORD({0,0}) };
COORD airport[2] = { COORD({40,15}), COORD({40, 18}) };
COORD E_Point_Dir[8] = { { 0,1 }, { -2,1 }, { -2,0 }, { -2,0 }, { 2,-1 }, { 2,0 }, { 2,0 }, { 2,1 } };
COORD airport_Dir[2] = { {0,-1}, {2,0} };
void spawn_plane()
{
	int point = randrange(0, 7);
	COORD dir = E_Point_Dir[point];
	int dest;

	int E_airport = 0; //randrange(0, 3);
	if (E_airport != 0)
	{
		int dest_E = randrange(0, 7);
		if (dest_E == point)
		{
			dest_E += randrange(1, 7);
			dest_E %= 8;
		}
		dest = dest_E;
	}
	else
	{
		int dest_airport = randrange(0, 1);
		dest = dest_airport + 8;
	}
	if (randrange(0, 1))
	{
		if (airliner_count != 0)
		{
			int index = rand() % airliner_count + 1;
			for (int i = 0; i < 26 && index > 0; i++)
			{
				if (!airliner[i].active)
				{
					index--;
					if (index == 0)
					{
						airliner_count--;
						COORD spawn_point = { E_Point[point].X + dir.X, E_Point[point].Y + dir.Y };
						init_plane(airliner + i, spawn_point, dir, dest);
						airliner[i].active = true;
					}
				}
			}
		}
	}
	else
	{
		if (jet_count != 0)
		{
			int index = rand() % jet_count + 1;
			for (int i = 0; i < 26 && index > 0; i++)
			{
				if (!jet[i].active)
				{
					index--;
					if (index == 0)
					{
						jet_count--;
						init_plane(jet + i, E_Point[point], dir, dest);
						jet[i].active = true;
					}
				}
			}
		}
	}
}
void isCollided()
{
	for (int i = 0; i < 26; i++)
	{
		if (airliner[i].active)
		{
			for (int j = 0; j < 26; j++)
			{
				if (i != j && airliner[j].active)
				{
					bool diffX = abs(airliner[i].pos.X - airliner[j].pos.X) < 4;
					bool diffY = abs(airliner[i].pos.Y - airliner[j].pos.Y) < 3;
					bool diffAl = abs(airliner[i].altitude - airliner[j].altitude) < 3;
					if (diffX && diffY && diffAl)
					{
						play = false;
					}
				}
			}
		}
	}
	for (int i = 0; i < 26; i++)
	{
		if (airliner[i].active)
		{
			for (int j = 0; j < 26; j++)
			{
				if (i != j && jet[j].active)
				{
					bool diffX = abs(airliner[i].pos.X - jet[j].pos.X) < 4;
					bool diffY = abs(airliner[i].pos.Y - jet[j].pos.Y) < 3;
					bool diffAl = abs(airliner[i].altitude - jet[j].altitude) < 3;
					if (diffX && diffY && diffAl)
					{
						play = false;
					}
				}
			}
		}
	}
	for (int i = 0; i < 26; i++)
	{
		if (jet[i].active)
		{
			for (int j = 0; j < 26; j++)
			{
				if (i != j && jet[j].active)
				{
					bool diffX = abs(jet[i].pos.X - jet[j].pos.X) < 4;
					bool diffY = abs(jet[i].pos.Y - jet[j].pos.Y) < 3;
					bool diffAl = abs(jet[i].altitude - jet[j].altitude) < 3;
					if (diffX && diffY && diffAl)
					{
						play = false;
					}
				}
			}
		}
	}
}
struct Item
{
	COORD pos;
	bool active = false;
};
Item timeStopper;
int freezedTime = 0;
#define air_speed 5000
#define jet_speed 2500
int currentTime = 0;
bool alreadyMove = false;
#define TO_ONE(x2, x1) ((x2 - x1) / abs(x2 - x1))
void plane_update(Plane* plane)
{
	if (!plane->active)
	{
		return;
	}
	plane->pos.X += plane->dir.X;
	plane->pos.Y += plane->dir.Y;
	if (plane->target_altitude - plane->altitude != 0)
	{
		plane->altitude += TO_ONE(plane->target_altitude, plane->altitude);
	}
	if (plane->beaconPoint.X > -1 && plane->beaconPoint.Y > -1)
	{
		if (plane->pos.X == plane->beaconPoint.X && plane->pos.Y == plane->beaconPoint.Y)
		{
			processCommand(plane->instruction);
			plane->beaconPoint.X = -1;
			plane->beaconPoint.Y = -1;
		}
	}
	if (plane->pos.X == 0 || plane->pos.X == 58 || plane->pos.Y == 0 || plane->pos.Y == 20)
	{
		plane->active = false;
		bool atExit = plane->pos.X == E_Point[plane->dest].X && plane->pos.Y == E_Point[plane->dest].Y;
		bool atAltitude9 = plane->altitude == 9;
		bool atDir = plane->dir.X == -E_Point_Dir[plane->dest].X && plane->dir.Y == -E_Point_Dir[plane->dest].Y;
		if (atExit && atAltitude9 && atDir)
		{
			safe++;
		}
		else
		{
			play = false;
		}
	}
	if (plane->altitude == 0)
	{
		bool atAirport = plane->pos.X == airport[(plane->dest) - 8].X && plane->pos.Y == airport[(plane->dest) - 8].Y;
		bool atDir = plane->dir.X == airport_Dir[plane->dest - 8].X && plane->dir.Y == airport_Dir[plane->dest - 8].Y;
		if (atAirport && atDir)
		{
			safe++;
			plane->active = false;

		}
		else
		{
			play = false;
		}
	}
	if (plane->pos.X == timeStopper.pos.X && plane->pos.Y == timeStopper.pos.Y && timeStopper.active)
	{
		timeStopper.active = false;
		freezedTime = 7000;
	}
	isCollided();
}
void flyPlane()
{
	currentTime += dt;
	if (currentTime >= air_speed)
	{
		currentTime -= air_speed;
		alreadyMove = false;
		// Move Air and Jet
		for (int i = 0; i < 26; i++)
		{
			plane_update(airliner + i);
			plane_update(jet + i);
		}
	}
	else if (currentTime >= jet_speed && !alreadyMove)
	{
		alreadyMove = true;
		for (int i = 0; i < 26; i++)
		{
			plane_update(jet + i);
		}
	}
}
int currentSpawnTime = 0;
int spawnTime = 3000;
void plane_spawner()
{
	currentSpawnTime += dt;
	if (currentSpawnTime >= spawnTime)
	{
		currentSpawnTime -= spawnTime;
		spawnTime = randrange(10, 100) * 1000;
		spawn_plane();
	}
}
#pragma endregion

void dropItem()
{
	if (!timeStopper.active)
	{
		short posx = randrange(1, 28) * 2;
		short posy = randrange(1, 19);
		timeStopper.active = true;
		timeStopper.pos = { posx, posy };
	}
}
int dropTime = 60000; 
int currentDropTime = 0;
void item_droper()
{
	currentDropTime += dt;
	if (currentDropTime >= dropTime)
	{
		currentDropTime -= dropTime;
		dropTime = randrange(60, 120) * 1000;
		dropItem();
	}
}
int planeColor = 160;
COORD beacon_points[] = { COORD({24,7}), COORD({24,17}) };
void processCommand(const char* input)
{
	Plane* plane;
	int cmdlen = strlen(input);
	if (cmdlen < 3)
	{
		return;
	}
	char name = input[0];
	if ('A' <= name && name <= 'Z')
	{
		plane = (airliner + (name - 'A'));
	}
	else if ('a' <= name && name <= 'z')
	{
		plane = (jet + (name - 'a'));
	}
	/*else if (name == '/')
	{
		std::string colorStr = input;
		planeColor = std::stoi(colorStr.substr(1, colorStr.length()-1));
		return;
	}*/
	else
	{
		return;
	}
	if (cmdlen == 6)
	{
		if (input[3] == 'a' && input[4] == 'b')
		{
			if (input[5] == '0' || input[5] == '1')
			{
				plane->beaconPoint = beacon_points[input[5] - '0'];
				plane->instruction[0] = input[0];
				plane->instruction[1] = input[1];
				plane->instruction[2] = input[2];
			}
		}
		return;
	}
	char action = input[1];
	if (action == 'a')
	{
		if (input[2] >= '0' && input[2] <= '9')
		{
			int altitude = input[2] - '0';
			plane->target_altitude = altitude;
		}
	}
	else if (action == 't')
	{
		char turn = input[2];
		COORD direction;
		switch (turn)
		{
		case 'q': // NW
			direction = { -2,-1 };
			break;
		case 'w': // N
			direction = { 0,-1 };
			break;
		case 'e': // NE
			direction = { 2,-1 };
			break;
		case 'a': // W
			direction = { -2,0 };
			break;
		case 'd': // E
			direction = { 2,0 };
			break;
		case 'z': // SW
			direction = { -2,1 };
			break;
		case 'x': // S
			direction = { 0,1 };
			break;
		case 'c': // SE
			direction = { 2,1 };
			break;
		default:
			direction = plane->dir;
			break;
		}
		plane->dir = direction;
	}
}

void displayPlaneDestination()
{
	COORD posN = { 61,3 };
	COORD posE = { 70,3 };
	for (int i = 0; i < 26; i++)
	{
		if (airliner[i].active)
		{
			char displayName[3] = "";
			displayName[0] = airliner[i].name;
			displayName[1] = '0' + airliner[i].altitude;
			draw_console(displayName, posN, 10);
			posN.Y++;

			if (airliner[i].dest < 8)
			{
				char displayExit[3] = "E0";
				displayExit[1] += airliner[i].dest;
				draw_console(displayExit, posE, 10);
				posE.Y++;
			}
			else
			{
				char displayExit[3] = "A0";
				displayExit[1] += airliner[i].dest - 8;
				draw_console(displayExit, posE, 10);
				posE.Y++;
			}
		}
	}
	for (int i = 0; i < 26; i++)
	{
		if (jet[i].active)
		{
			char displayName[3] = "";
			displayName[0] = jet[i].name;
			displayName[1] = '0' + jet[i].altitude;
			draw_console(displayName, posN, 10);
			posN.Y++;

			if (jet[i].dest < 8)
			{
				char displayExit[3] = "E0";
				displayExit[1] += jet[i].dest;
				draw_console(displayExit, posE, 10);
				posE.Y++;
			}
			else
			{
				char displayExit[3] = "A0";
				displayExit[1] += jet[i].dest - 8;
				draw_console(displayExit, posE, 10);
				posE.Y++;
			}
		}
	}
}
void displayInfo()
{
	displayTime();
	displayPlaneDestination();

	char safeStr[11];
	_itoa_s(safe, safeStr, 10);
	draw_console(safeStr, { 67, 1 }, 10);
}

int main()
{
	srand(time(NULL));
	init_console();
	init_airliner_jet();
	DWORD numEvents = 0;
	DWORD numEventsRead = 0;
	while (play)
	{
		// Event part
		GetNumberOfConsoleInputEvents(console.rHnd, &numEvents);
		if (numEvents != 0) {
			INPUT_RECORD* eventBuffer = new INPUT_RECORD[numEvents];
			ReadConsoleInput(console.rHnd, eventBuffer, numEvents, &numEventsRead);
			for (DWORD i = 0; i < numEventsRead && i < numEvents; ++i)
			{
				if (eventBuffer[i].EventType == KEY_EVENT && eventBuffer[i].Event.KeyEvent.bKeyDown == true)
				{
					if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE)
					{
						play = false;
					}
					if (type_command(eventBuffer[i].Event.KeyEvent.uChar.AsciiChar, eventBuffer[i].Event.KeyEvent.wVirtualKeyCode))
					{
						processCommand(cmd);
						clear_command();
					}
				}
			}
			delete[] eventBuffer;
		}
		// Update part
		if (freezedTime > 0)
			freezedTime -= dt;
		if (freezedTime <= 0)
		{
			plane_spawner();
			item_droper();
			flyPlane();
		}
		// Drawing part
		clear_console();
		draw_console(
			"7-----------------------0---------------------------------1  Time: \n"
			"| + . . . . . . . . . . + . . . . . . . . . . . . . . . + |  Safe: \n"
			"| . + . . . . . . . . . + . . . . . . . . . . . . . . + . |  Plane:   Destination:\n"
			"| . . + . . . . . . . . + . . . . . . . . . . . . . + . . |\n"
			"| . . . + . . . . . . . + . . . . . . . . . . . . + . . . |\n"
			"| . . . . + . . . . . . + . . . . . . . . . . . + . . . . |\n"
			"| . . . . . + . . . . . + . . . . . . . . . . + . . . . . |\n"
			"6 + + + + + + + + + + + *0+ + + + + + + + + + + + + + + + 2\n"
			"| . . . . . . . . . . . + . . . . . . . . + . . . . . . . |\n"
			"| . . . . . . . . . . . + . . . . . . . + . . . . . . . . |\n"
			"| . . . . . . . . . . . + . . . . . . + . . . . . . . . . |\n"
			"| . . . . . . . . . . . + . . . . . + . . . . . . . . . . |\n"
			"| . . . . . . . . . . . + . . . . + . . . . . . . . . . . |\n"
			"5 + + + + + + + + + + + + . . . + . . . . . . . . . . . . |\n"
			"| . . . . . . . . . . . + . . + . . . . . . . . . . . . . |\n"
			"| . . . . . . . . . . . + . + . . . . . ^0. . . . . . . . |\n"
			"| . . . . . . . . . . . + + . . . . . . . . . . . . . . . |\n"
			"| . . . . . . . . . . . *1+ + + + + + + + + + + + + + + + 3\n"
			"| . . . . . . . . . . + . . . . . . . . >1. . . . . . . . |\n"
			"| . . . . . . . . . + . . . . . . . . . . . . . . . . . . |\n"
			"------------------4----------------------------------------\n"
			"command: ", { 0,0 }, 10);

		gotoxy(9 + strlen(cmd), 21);
		draw_console(cmd, { 9,21 }, 10);
		if (timeStopper.active)
		{
			draw_console("&", timeStopper.pos, 175);
		}
		if (freezedTime > 0)
		{
			draw_console("Time Stopped!", { 0, 22 }, 12);
		}
		for (int i = 0; i < 26; i++)
		{
			char displayName[3] = "";
			if (airliner[i].active)
			{
				displayName[0] = airliner[i].name;
				displayName[1] = '0' + airliner[i].altitude;
				draw_console(displayName, airliner[i].pos, planeColor);
			}
			if (jet[i].active)
			{
				displayName[0] = jet[i].name;
				displayName[1] = '0' + jet[i].altitude;
				draw_console(displayName, jet[i].pos, planeColor);
			}
		}
		displayInfo();
		display_console();
		Sleep(dt);
	}
	return 0;
}