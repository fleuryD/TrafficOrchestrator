#include "../include/traffic-orchestrator.hpp"

std::string	generateFilename() {
	// Obtenir l'heure actuelle
	std::time_t now = std::time(nullptr);
	std::tm* localTime = std::localtime(&now);

	// Formatter la date et l'heure
	char buffer[30];
	std::strftime(buffer, sizeof(buffer), "newMap-%Y%m%d-%H%M%S", localTime);
	return"./maps/" + std::string(buffer) + ".txt";
}

void	Game::generateFromFile(const std::string& filename) {

	openFilename = "";
	if (filename.empty()){
		std::cerr <<  C_RED << "No file to open " << C_NOR << std::endl;
		centerCameraOnNodes();
		return;
	}
	std::ifstream file(filename);
	if (!file) {
		std::cerr <<  C_RED << "Erreur d'ouverture du fichier " << filename << C_NOR << std::endl;
		centerCameraOnNodes();
		return;
	}

	std::string line;
	bool readingNodes = false, readingRoads = false;

	while (std::getline(file, line)) {
		if (line.empty() || line[0] == '#') continue;	// Ignore commentaires et lignes vides

		std::stringstream ss(line);

		if (line == "Nodes") {
			std::cout <<  C_GRE << "Nodes................ " <<  std::endl;	// !!!!!!!!!!!!!
			readingNodes = true;
			readingRoads = false;
			continue;
		} else if (line == "Roads") {
			readingNodes = false;
			readingRoads = true;
			continue;
		}

		if (readingNodes) {
			int x, y;
			if (ss >> x >> y) {
				nodes.push_back(std::make_unique<Node>(x, y));
			}
		} else if (readingRoads) {
			int index1, index2, lanes, speed;
			if (ss >> index1 >> index2 >> lanes >> speed) {
				if (index1 < nodes.size() && index2 < nodes.size()) {
					createRoad(*nodes[index1], *nodes[index2], lanes, speed);
				} else {
					std::cerr << C_RED << "Erreur: Index de nœud invalide" << std::endl;
				}
			}
		}
	}


	openFilename = filename;
	updateConfigValue("LastOpenFilename", filename);
	SetWindowTitle(("TrafficOrchestrator. (by dFleury) :   " + filename).c_str());
	centerCameraOnNodes();
	asUnsavedChanges = false;
	printMapInfos();


}

bool	Game::saveMapToFile(const std::string& filename) {
	std::ofstream file(filename);

	if (!file.is_open()) {
		std::cerr << "Erreur : Impossible d'ouvrir le fichier " << filename << std::endl;
		return false;
	}

	// Écriture des Nodes
	file << "Nodes\n";
	for (const auto& node : nodes) {
		file << node->x << " " << node->y << "\n";
	}

	// Écriture des Roads
	file << "\nRoads\n";
	for (const auto& road : roads) {
		int startIdx = -1, endIdx = -1;
		for (size_t i = 0; i < nodes.size(); i++) {
			if (nodes[i].get() == &road->start) startIdx = i;
			if (nodes[i].get() == &road->end) endIdx = i;
		}
		file << startIdx << " " << endIdx << " " << road->lanesPtr.size() << " " << road->speedLimit << "\n";
	}

	file.close();
	openFilename = filename;
	updateConfigValue("LastOpenFilename", filename);
	SetWindowTitle(("TrafficOrchestrator. (by dFleury) :   " + filename).c_str());
	centerCameraOnNodes();
	asUnsavedChanges = false;
	printMapInfos();
	return true;
}

void	Game::printMapInfos(){


	std::cout << C_GRE << "  map filenamae " << 42 << C_NOR << std::endl;
	int totalRoadLanes = 0;
	int toralRoadLaneCars = 0;
	//for (Road &road : roads) {
	for (auto &roadPrt : roads) {
		Road &road = *roadPrt;
		totalRoadLanes += road.lanesPtr.size();
		for (int id = 0; id < road.lanesPtr.size(); id++) {
			toralRoadLaneCars += road.lanesPtr[id]->cars.size();
		}
	}
	std::cout << C_GRE << "  nodes.size() = " << nodes.size() << C_NOR << std::endl;
	std::cout << C_GRE << "  roads.size() = " << roads.size() << C_NOR << std::endl;
	std::cout << C_GRE << "  totalRoadLanes = " << totalRoadLanes << C_NOR << std::endl;
	std::cout << C_GRE << "  toralRoadLaneCars = " << toralRoadLaneCars << C_NOR << std::endl;

}

bool	Game::saveMapClick(){
	std::string filename = openFilename.empty() ? generateFilename() : openFilename;
	return saveMapToFile(filename);
	//return saveMapToFile("./maps/openFilename.txt");
}

bool	Game::saveMapAsClick(){
	return saveMapToFile(generateFilename());
}

void	Game::newMapClick(){
	// todo: confirmation
	roads.clear();
	nodes.clear();
	openFilename = "";
	updateConfigValue("LastOpenFilename", "");
	SetWindowTitle("TrafficOrchestrator. (by dFleury)");
	centerCameraOnNodes();
	asUnsavedChanges = false;
	printMapInfos();
}



// ! Draw and HANDLE  !!!!!
void	Game::drawMapSelector() {

	Rectangle	modalRec = { 20, 50, 400,  static_cast<float>(screenHeight - 100) };
	Rectangle	btnCancelRec = { modalRec.x+modalRec.width - 100, 68, 80, 28 };
	const int	itemHeight = 30;
	int			selectedIndex = -1;

	int			maxVisibleItems = (modalRec.height - 120) / itemHeight;
	static int	pageIndex = 0;
	int			totalPages = (filenames.size() + maxVisibleItems - 1) / maxVisibleItems;
	int			startIdx = pageIndex * maxVisibleItems;
	int			endIdx = std::min(startIdx + maxVisibleItems, (int)filenames.size());

	drawRectWithBorder(modalRec, LIGHTGRAY, BLACK);

	DrawText("Open map:", modalRec.x+20, modalRec.y+20, 25, BLACK);


	// * btn Cancel:
	Color btnCancelTextColor = RED;
	Color btnCancelBgColor = LIGHTGRAY;
	if (CheckCollisionPointRec(GetMousePosition(), btnCancelRec)) {
		btnCancelTextColor = WHITE;
		btnCancelBgColor = RED;
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			mode = M_NONE;
		}
	}
	drawRectWithBorder(btnCancelRec, btnCancelBgColor, RED);
	DrawText("Cancel", btnCancelRec.x+4, btnCancelRec.y+4, 20, btnCancelTextColor);



	// * btns fichiers:
	for (int i = startIdx; i < endIdx; i++) {
		int y = modalRec.y+60 + (i - startIdx) * itemHeight;
		Rectangle btnFileRec = { modalRec.x+20, (float)y, 360, (float)itemHeight };

		// Détection du survol
		Color textColor = BLACK;
		if (CheckCollisionPointRec(GetMousePosition(), btnFileRec)) {
			textColor = DARKGRAY;
			drawRectWithBorder(btnFileRec, LIGHTGRAY, BLACK);
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				selectedIndex = i;
				mode = M_NONE;
				roads.clear();
				nodes.clear();
				if (selectedIndex != -1) {
					generateFromFile("./maps/" + filenames[selectedIndex]);
					return;
				}
			}
		}

		DrawText(filenames[i].c_str(), btnFileRec.x+10, btnFileRec.y + 5, 20, textColor);
	}

	// * Bouton "Précédent"
	Rectangle btnPrevRec = { modalRec.x + 20, modalRec.y + modalRec.height - 40, 80, 30 };
	Color btnPrevTextColor = BLACK;
	Color btnPrevBgColor = LIGHTGRAY;
	if (pageIndex > 0) {
		if (CheckCollisionPointRec(GetMousePosition(), btnPrevRec)) {
			btnPrevTextColor = WHITE;
			btnPrevBgColor = DARKGRAY;
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				pageIndex--;
			}
		}
		drawRectWithBorder(btnPrevRec, btnPrevBgColor, BLACK);
		DrawText("< Prev", btnPrevRec.x + 10, btnPrevRec.y + 5, 20, btnPrevTextColor);
	}

	// * Bouton "Suivant"
	Rectangle btnNextRec = { modalRec.x + modalRec.width - 100, modalRec.y + modalRec.height - 40, 80, 30 };
	Color btnNextTextColor = BLACK;
	Color btnNextBgColor = LIGHTGRAY;
	if (pageIndex < totalPages - 1) {
		if (CheckCollisionPointRec(GetMousePosition(), btnNextRec)) {
			btnNextTextColor = WHITE;
			btnNextBgColor = DARKGRAY;
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				pageIndex++;
			}
		}
		drawRectWithBorder(btnNextRec, btnNextBgColor, BLACK);
		DrawText("Next >", btnNextRec.x + 10, btnNextRec.y + 5, 20, btnNextTextColor);
	}

}


void	Game::openMapClick(){
	// todo: confirmation
	filenames = getTxtFiles("./maps/");
	std::cout << "Fichiers .txt dans le dossier maps/ :" << std::endl;
	for (const std::string &filename : filenames) {
		std::cout << "  - " << filename << std::endl;
	}

	mode = M_MAP_OPEN;
}
