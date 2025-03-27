#pragma once

#include <string>
#include <random>
#include <iostream>
#include <fstream>	// * pour lire un fichier
#include <sstream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <map>
#include <unordered_map>
#include <memory>	// * Pour std::unique_ptr
#include <ctime>
#include <filesystem>

#include "../raylib/src/raylib.h"


struct Config {
	int	windowX = 100;	// Valeurs par défaut
	int	windowY = 100;
	int	windowW = 800;
	int	windowH = 600;
	int	fps = 60;

	std::string										lastOpenFilename = "";
	std::unordered_map<std::string, KeyboardKey>	keyBindings = {
		{"Up", KEY_W},
		{"Down", KEY_S},
		{"Left", KEY_A},
		{"Right", KEY_D},
		{"ZoomIn", KEY_P},
		{"ZoomOut", KEY_L},
		{"ZoomReset", KEY_Z},
		{"CreateRoad", KEY_R},
		{"Help", KEY_H},
		{"Debug", KEY_D},
		{"GridCoords", KEY_C},
		{"RoadsInfos", KEY_I},
		{"Quit", KEY_Q},
		// {"MapNew", KEY_N},
		// {"MaOpen", KEY_O},
		// {"MapSave", KEY_S},
		// {"MapSaveAs", KEY_X},
	};
};


struct UIPanelRoad {
	Rectangle panelRec = { 650, 60, 220, 130 };
	bool reverseRoad = true;
	int speedLimit = 80;
	int lanesCount = 3;
};


struct Btn {
	Rectangle	rec;
	std::string	text;
	bool		isOn = false;
};

struct Ui {
	int		navHeaderHeight;
	int		btnFontSize;
	Btn		btnMapNew;
	Btn		btnMapOpen;
	Btn		btnMapSave;
	Btn		btnMapSaveAs;
	Btn		btnAddRoad;

	// !! UNUSED POUR LE MOMENT	// * UIPanelRoad :
	Rectangle	panelRoadRec;
	bool		reverseRoad;
	int			speedLimit;
	int			lanesCount;

};

struct IPoint {
	int x;
	int y;
};


// * Terminal colors:
#define C_NOR "\x1b[m"
#define C_GRA "\x1b[30m"
#define C_RED "\x1b[31m"
#define C_GRE "\x1b[32m"
#define C_YEL "\x1b[33m"
#define C_BLU "\x1b[34m"
#define C_MAG "\x1b[35m"
#define C_CYA "\x1b[36m"
#define C_WHI "\x1b[37m"
#define B_GRA "\x1b[40m"
#define B_RED "\x1b[41m"
#define B_GRE "\x1b[42m"
#define B_YEL "\x1b[43m"
#define B_BLU "\x1b[44m"
#define B_MAG "\x1b[45m"
#define B_CYA "\x1b[46m"
#define B_WHI "\x1b[47m"
#define S_BOLD "\x1b[1m"
#define S_UNDER "\x1b[4m"
#define S_REV "\x1b[7m"
// * Orientations:
#define HORIZONTAL 221
#define VERTICAL 222
// * Directions:
#define D_LEFT 331
#define D_RIGHT 332
#define D_UP 333
#define D_DOWN 334
// * Modes:
#define M_NONE 441
#define M_CREATE_ROAD 442
#define M_MAP_OPEN 443

#define SCALE_METERS_PER_PX 1		// deprecated ???
#define LINE_WIDTH 3				// deprecated ???

#define CONFIG_FILENAME "config.txt"
#define MAPS_FOLDER "./maps/"		// UNUSED ???

#include "./Node.hpp"
#include "./Road.hpp"
#include "./Car.hpp"
#include "./Lane.hpp"
#include "./Game.hpp"


// *****	utils_config.cpp
int			getKeyFromKeyName(const std::string& keyName);
std::string getKeyNameFromKey(KeyboardKey key);

// *****	utils_draw.cpp
void	drawRectWithBorder(Rectangle rec, Color fillColor, Color borderColor);
void	drawCross(int x, int y, int size, Color color);
void	drawBtn(Rectangle rec, const std::string &text, int fontSize, bool isOn);
bool	recIsHovered(Rectangle rec);

// *****	utils_files.cpp
std::vector<std::string>	getTxtFiles(const std::string& folderPath) ;
Config						loadConfigFromFile() ;
void						updateConfigValue(const std::string& key, const std::string& newValue);

// *****	utils_maths.cpp
int		getRandomNumberMinMax(int min, int max);
int		roundToHundred(int value);

// *****	xxx.cpp

Color	getRandomCarColor();
