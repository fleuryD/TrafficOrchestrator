#include "../include/traffic-orchestrator.hpp"


#include <filesystem>

namespace fs = std::filesystem;

// * Obtenir la liste des noms des fichiers .txt du dossier "maps/"
std::vector<std::string>	getTxtFiles(const std::string& folderPath) {
	std::vector<std::string> files;
	for (const auto& entry : fs::directory_iterator(folderPath)) {
		if (entry.is_regular_file() && entry.path().extension() == ".txt") {
			files.push_back(entry.path().filename().string());
		}
	}
	std::sort(files.begin(), files.end());
	return files;
}


// * Mettre à jour la valeur d'une clé dans le fichier config.txt
void	updateConfigValue(const std::string& key, const std::string& newValue) {
	std::ifstream fileIn(CONFIG_FILENAME);
	std::vector<std::string> lines;
	bool found = false;

	if (!fileIn) {
		std::cerr << "Erreur : Impossible d'ouvrir " << CONFIG_FILENAME << std::endl;
		return;
	}

	std::string line;
	while (std::getline(fileIn, line)) {
		if (line.rfind(key, 0) == 0) {	// Vérifie si la ligne commence par la clé
			line = key + "\t" + newValue;
			found = true;
		}
		lines.push_back(line);
	}
	fileIn.close();

	// Si la clé n'existe pas, on l'ajoute
	if (!found) {
		lines.push_back(key + "\t" + newValue);
	}

	// Réécriture du fichier
	std::ofstream fileOut(CONFIG_FILENAME);
	if (!fileOut) {
		std::cerr << "Erreur : Impossible d'écrire dans " << CONFIG_FILENAME << std::endl;
		return;
	}

	for (const auto& l : lines) {
		fileOut << l << '\n';
	}
}

// * Charger la configuration depuis le fichier de config.txt
Config	loadConfigFromFile() {

	Config config;
	config.lastOpenFilename = "";

	std::ifstream file(CONFIG_FILENAME);
	if (!file) {
		std::cout << "Fichier " << CONFIG_FILENAME << " introuvable, utilisation des valeurs par défaut.\n";
		std::ofstream newFile(CONFIG_FILENAME); // Crée le fichier
        if (newFile) {
            std::cout << "Fichier créé : " << CONFIG_FILENAME << std::endl;
            newFile << "Window 100 100 900 800\n";
            newFile << "Fps 60\n";
            newFile << "\n";
			newFile << "Key_Up		UP\n";
			newFile << "Key_Down	DOWN\n";
			newFile << "Key_Left	LEFT\n";
			newFile << "Key_Right	RIGHT\n";
			newFile << "Key_ZoomIn		KP_ADD\n";
			newFile << "Key_ZoomOut		KP_SUBTRACT\n";
			newFile << "Key_ZoomReset	KP_MULTIPLY # or ZoomAll\n";
			newFile << "Key_CreateRoad	R\n";
			newFile << "Key_Help		H\n";
			newFile << "Key_Debug		D\n";
			newFile << "Key_GridCoords	C\n";
			newFile << "Key_RoadsInfos	I\n";
			newFile << "Key_Quit		Q\n";
            newFile.close();

			return loadConfigFromFile();

        } else {
            std::cerr << "Erreur lors de la création du fichier " <<  CONFIG_FILENAME << std::endl;
        }

		return config;
	}

	std::string line;
	while (std::getline(file, line)) {
		// Supprimer les commentaires (tout après #)
		size_t commentPos = line.find('#');
		if (commentPos != std::string::npos) {
			line = line.substr(0, commentPos);
		}

		// Supprimer les espaces en début et fin de ligne
		line.erase(0, line.find_first_not_of(" \t"));
		line.erase(line.find_last_not_of(" \t") + 1);

		// Ignorer les lignes vides
		if (line.empty()) continue;

		std::istringstream iss(line);
		std::string key;
		iss >> key;

		if (key == "Window") {
			iss >> config.windowX >> config.windowY >> config.windowW >> config.windowH;
		} else if (key == "Fps") {
			iss >> config.fps;
		} else if (key == "LastOpenFilename") {
			iss >> config.lastOpenFilename;
		} else if (key.rfind("Key_", 0) == 0) {	// Vérifie si la ligne commence par "Key_"
			std::string action = key.substr(4);	// Supprime "Key_"
			std::string keyName;
			iss >> keyName;

			// Conversion du nom de touche en `KeyboardKey`
			KeyboardKey keyCode = (KeyboardKey)getKeyFromKeyName(keyName);
			config.keyBindings[action] = keyCode;
		}
	}

	return config;
}

