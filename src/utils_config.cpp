#include "../include/traffic-orchestrator.hpp"

int	getKeyFromKeyName(const std::string& keyName) {
	static const std::unordered_map<std::string, KeyboardKey> keyMap = {
		{"NULL", KEY_NULL}, {"APOSTROPHE", KEY_APOSTROPHE}, {"COMMA", KEY_COMMA}, {"MINUS", KEY_MINUS},
		{"PERIOD", KEY_PERIOD}, {"SLASH", KEY_SLASH}, {"ZERO", KEY_ZERO}, {"ONE", KEY_ONE},
		{"TWO", KEY_TWO}, {"THREE", KEY_THREE}, {"FOUR", KEY_FOUR}, {"FIVE", KEY_FIVE},
		{"SIX", KEY_SIX}, {"SEVEN", KEY_SEVEN}, {"EIGHT", KEY_EIGHT}, {"NINE", KEY_NINE},
		{"SEMICOLON", KEY_SEMICOLON}, {"EQUAL", KEY_EQUAL}, {"A", KEY_A}, {"B", KEY_B},
		{"C", KEY_C}, {"D", KEY_D}, {"E", KEY_E}, {"F", KEY_F}, {"G", KEY_G},
		{"H", KEY_H}, {"I", KEY_I}, {"J", KEY_J}, {"K", KEY_K}, {"L", KEY_L},
		{"M", KEY_M}, {"N", KEY_N}, {"O", KEY_O}, {"P", KEY_P}, {"Q", KEY_Q},
		{"R", KEY_R}, {"S", KEY_S}, {"T", KEY_T}, {"U", KEY_U}, {"V", KEY_V},
		{"W", KEY_W}, {"X", KEY_X}, {"Y", KEY_Y}, {"Z", KEY_Z}, {"LEFT_BRACKET", KEY_LEFT_BRACKET},
		{"BACKSLASH", KEY_BACKSLASH}, {"RIGHT_BRACKET", KEY_RIGHT_BRACKET}, {"GRAVE", KEY_GRAVE},
		{"SPACE", KEY_SPACE}, {"ESCAPE", KEY_ESCAPE}, {"ENTER", KEY_ENTER}, {"TAB", KEY_TAB},
		{"BACKSPACE", KEY_BACKSPACE}, {"INSERT", KEY_INSERT}, {"DELETE", KEY_DELETE},
		{"RIGHT", KEY_RIGHT}, {"LEFT", KEY_LEFT}, {"DOWN", KEY_DOWN}, {"UP", KEY_UP},
		{"PAGE_UP", KEY_PAGE_UP}, {"PAGE_DOWN", KEY_PAGE_DOWN}, {"HOME", KEY_HOME}, {"END", KEY_END},
		{"CAPS_LOCK", KEY_CAPS_LOCK}, {"SCROLL_LOCK", KEY_SCROLL_LOCK}, {"NUM_LOCK", KEY_NUM_LOCK},
		{"PRINT_SCREEN", KEY_PRINT_SCREEN}, {"PAUSE", KEY_PAUSE}, {"F1", KEY_F1}, {"F2", KEY_F2},
		{"F3", KEY_F3}, {"F4", KEY_F4}, {"F5", KEY_F5}, {"F6", KEY_F6}, {"F7", KEY_F7},
		{"F8", KEY_F8}, {"F9", KEY_F9}, {"F10", KEY_F10}, {"F11", KEY_F11}, {"F12", KEY_F12},
		{"LEFT_SHIFT", KEY_LEFT_SHIFT}, {"LEFT_CONTROL", KEY_LEFT_CONTROL}, {"LEFT_ALT", KEY_LEFT_ALT},
		{"LEFT_SUPER", KEY_LEFT_SUPER}, {"RIGHT_SHIFT", KEY_RIGHT_SHIFT}, {"RIGHT_CONTROL", KEY_RIGHT_CONTROL},
		{"RIGHT_ALT", KEY_RIGHT_ALT}, {"RIGHT_SUPER", KEY_RIGHT_SUPER}, {"KB_MENU", KEY_KB_MENU},
		{"KP_0", KEY_KP_0}, {"KP_1", KEY_KP_1}, {"KP_2", KEY_KP_2}, {"KP_3", KEY_KP_3},
		{"KP_4", KEY_KP_4}, {"KP_5", KEY_KP_5}, {"KP_6", KEY_KP_6}, {"KP_7", KEY_KP_7},
		{"KP_8", KEY_KP_8}, {"KP_9", KEY_KP_9}, {"KP_DECIMAL", KEY_KP_DECIMAL}, {"KP_DIVIDE", KEY_KP_DIVIDE},
		{"KP_MULTIPLY", KEY_KP_MULTIPLY}, {"KP_SUBTRACT", KEY_KP_SUBTRACT}, {"KP_ADD", KEY_KP_ADD},
		{"KP_ENTER", KEY_KP_ENTER}, {"KP_EQUAL", KEY_KP_EQUAL}
	};

	auto it = keyMap.find(keyName);
	return (it != keyMap.end()) ? it->second : KEY_NULL;
}

std::string	getKeyNameFromKey(KeyboardKey key) {
	static const std::unordered_map<KeyboardKey, std::string> keyNameMap = {
		{KEY_NULL, "NULL"}, {KEY_APOSTROPHE, "APOSTROPHE"}, {KEY_COMMA, "COMMA"}, {KEY_MINUS, "MINUS"},
		{KEY_PERIOD, "PERIOD"}, {KEY_SLASH, "SLASH"}, {KEY_ZERO, "ZERO"}, {KEY_ONE, "ONE"},
		{KEY_TWO, "TWO"}, {KEY_THREE, "THREE"}, {KEY_FOUR, "FOUR"}, {KEY_FIVE, "FIVE"},
		{KEY_SIX, "SIX"}, {KEY_SEVEN, "SEVEN"}, {KEY_EIGHT, "EIGHT"}, {KEY_NINE, "NINE"},
		{KEY_SEMICOLON, "SEMICOLON"}, {KEY_EQUAL, "EQUAL"}, {KEY_A, "A"}, {KEY_B, "B"},
		{KEY_C, "C"}, {KEY_D, "D"}, {KEY_E, "E"}, {KEY_F, "F"}, {KEY_G, "G"},
		{KEY_H, "H"}, {KEY_I, "I"}, {KEY_J, "J"}, {KEY_K, "K"}, {KEY_L, "L"},
		{KEY_M, "M"}, {KEY_N, "N"}, {KEY_O, "O"}, {KEY_P, "P"}, {KEY_Q, "Q"},
		{KEY_R, "R"}, {KEY_S, "S"}, {KEY_T, "T"}, {KEY_U, "U"}, {KEY_V, "V"},
		{KEY_W, "W"}, {KEY_X, "X"}, {KEY_Y, "Y"}, {KEY_Z, "Z"}, {KEY_LEFT_BRACKET, "LEFT_BRACKET"},
		{KEY_BACKSLASH, "BACKSLASH"}, {KEY_RIGHT_BRACKET, "RIGHT_BRACKET"}, {KEY_GRAVE, "GRAVE"},
		{KEY_SPACE, "SPACE"}, {KEY_ESCAPE, "ESCAPE"}, {KEY_ENTER, "ENTER"}, {KEY_TAB, "TAB"},
		{KEY_BACKSPACE, "BACKSPACE"}, {KEY_INSERT, "INSERT"}, {KEY_DELETE, "DELETE"},
		{KEY_RIGHT, "RIGHT"}, {KEY_LEFT, "LEFT"}, {KEY_DOWN, "DOWN"}, {KEY_UP, "UP"},
		{KEY_PAGE_UP, "PAGE_UP"}, {KEY_PAGE_DOWN, "PAGE_DOWN"}, {KEY_HOME, "HOME"}, {KEY_END, "END"},
		{KEY_CAPS_LOCK, "CAPS_LOCK"}, {KEY_SCROLL_LOCK, "SCROLL_LOCK"}, {KEY_NUM_LOCK, "NUM_LOCK"},
		{KEY_PRINT_SCREEN, "PRINT_SCREEN"}, {KEY_PAUSE, "PAUSE"}, {KEY_F1, "F1"}, {KEY_F2, "F2"},
		{KEY_F3, "F3"}, {KEY_F4, "F4"}, {KEY_F5, "F5"}, {KEY_F6, "F6"}, {KEY_F7, "F7"},
		{KEY_F8, "F8"}, {KEY_F9, "F9"}, {KEY_F10, "F10"}, {KEY_F11, "F11"}, {KEY_F12, "F12"},
		{KEY_LEFT_SHIFT, "LEFT_SHIFT"}, {KEY_LEFT_CONTROL, "LEFT_CONTROL"}, {KEY_LEFT_ALT, "LEFT_ALT"},
		{KEY_LEFT_SUPER, "LEFT_SUPER"}, {KEY_RIGHT_SHIFT, "RIGHT_SHIFT"}, {KEY_RIGHT_CONTROL, "RIGHT_CONTROL"},
		{KEY_RIGHT_ALT, "RIGHT_ALT"}, {KEY_RIGHT_SUPER, "RIGHT_SUPER"}, {KEY_KB_MENU, "KB_MENU"},
		{KEY_KP_0, "KP_0"}, {KEY_KP_1, "KP_1"}, {KEY_KP_2, "KP_2"}, {KEY_KP_3, "KP_3"},
		{KEY_KP_4, "KP_4"}, {KEY_KP_5, "KP_5"}, {KEY_KP_6, "KP_6"}, {KEY_KP_7, "KP_7"},
		{KEY_KP_8, "KP_8"}, {KEY_KP_9, "KP_9"}, {KEY_KP_DECIMAL, "KP_DECIMAL"}, {KEY_KP_DIVIDE, "KP_DIVIDE"},
		{KEY_KP_MULTIPLY, "KP *"}, {KEY_KP_SUBTRACT, "KP -"}, {KEY_KP_ADD, "KP +"},
		{KEY_KP_ENTER, "KP_ENTER"}, {KEY_KP_EQUAL, "KP ="}
	};

	auto it = keyNameMap.find(key);
	return (it != keyNameMap.end()) ? it->second : "UNKNOWN";
}
