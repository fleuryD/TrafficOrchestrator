#include "../include/traffic-orchestrator.hpp"




void	debugBasculerEcran2(){
	int monitorCount = GetMonitorCount();
	if (monitorCount > 1) {
		//int secondMonitorX = -GetMonitorWidth(1) +20;	// Position X du 2ème écran
		//int secondMonitorY = - GetMonitorHeight(0) + GetMonitorHeight(1) +20;
		int secondMonitorX = -GetMonitorWidth(1) + 15;	// Position X du 2ème écran
		int secondMonitorY = - GetMonitorHeight(0) + GetMonitorHeight(1) + 15;
		SetWindowPosition(secondMonitorX, secondMonitorY);
	//	SetWindowSize(GetMonitorWidth(1)-15, GetMonitorHeight(1)-15);
	} else {
		std::cout << "Seulement un écran détecté." << std::endl;
	}
}




// *****	CONTRUCTOR:



Game::Game(float scale, float timeScale):
	timeScale(timeScale),
	mouse({0, 0}), cursor({0, 0}), openFilename(""),
	newNodeStart(nullptr),
	minDistBetweenCarsInLane(10),
	showHelp(true), showDebug(false), showGridCoords(true), showRoadsInfos(true),
	asUnsavedChanges(false)
{

	config = loadConfigFromFile();
	screenWidth = config.windowW;
	screenHeight = config.windowH;
	mode = M_NONE;

	frameCounter = 0;

	std::cout << C_CYA << std::endl;
	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);
	InitWindow(screenWidth, screenHeight, "TrafficOrchestrator by dFleury");
	debugBasculerEcran2();


	SetTargetFPS(config.fps);

	initUi();

	int oldWidth = GetScreenWidth();
	int oldHeight = GetScreenHeight();
	int maximizeFrameCount = 60;
	MaximizeWindow();
	while (GetScreenWidth() == oldWidth && GetScreenHeight() == oldHeight && maximizeFrameCount > 0) {
		BeginDrawing();
		ClearBackground(BLACK);
		DrawText(TextFormat("Waiting for window resize... %d", maximizeFrameCount), 10, 10, 40, RAYWHITE);
		EndDrawing();
		maximizeFrameCount--;
	}
	screenWidth = GetScreenWidth();
	screenHeight = GetScreenHeight();


	SetExitKey(KEY_NULL);
	std::cout << C_NOR << std::endl;


	if (!config.lastOpenFilename.empty()){
		generateFromFile(config.lastOpenFilename);
	}

	newNodeStart = nullptr;


	selectedRoadId = -1;
	selectedNodeId = -1;
	hoveredRoadId = -1;
	hoveredNodeId = -1;

	centerCameraOnNodes();

}

// *****	METHODES:


void	Game::toggleAddRoadMode() {
	mode == M_CREATE_ROAD ? mode = M_NONE : mode = M_CREATE_ROAD;
	newNodeStart = nullptr;
	if (selectedRoadId != -1){
		roads[selectedRoadId]->isSelected = false;
		selectedRoadId = -1;
	}
	if (selectedNodeId != -1){
		nodes[selectedNodeId]->isSelected = false;
		selectedNodeId = -1;
	}
}

/*
void	Game::selectRoad(Road &road) {
	selectedRoad = &road;
}

void	Game::selectNode(Node &node) {
	selectedNode = &node;
}
*/

bool	Game::nodeExists(int x, int y) {
	auto it = std::find_if(nodes.begin(), nodes.end(), [x, y](const std::unique_ptr<Node> &node) {
		return node->x == x && node->y == y;
	});
	return it != nodes.end();
}

bool	Game::roadExists(Node &nStart, Node &nEnd) {
	auto it = std::find_if(roads.begin(), roads.end(), [nStart, nEnd](const std::unique_ptr<Road> &road) {
		return (road->start.x == nStart.x) && (road->start.y == nStart.y) &&
			(road->end.x == nEnd.x) && (road->end.y == nEnd.y);
		// return (road.start == nStart) && (road.end == nEnd);
	});
	return it != roads.end();
	//auto it = std::find_if(roads.begin(), roads.end(), [x, y](const Road &road) {
	//	return road.start.x == x && road.start.y == y && road.end.x == x && road.end.y == y;
	//});
}

Node*	Game::findNode(int x, int y) {
	for (auto &nodePtr : nodes) {
		Node &node = *nodePtr;
		if(node.x == x && node.y == y) return &node;
	}
	return nullptr;
	//auto it = std::find_if(nodes.begin(), nodes.end(), [x, y](const std::unique_ptr<Node> &node) {
	//	return node->x == x && node->y == y;
	//});
	//return nodes[0].get();
	//return (it != nodes.end()) ? &(*it) : nullptr;
}

void	Game::centerCameraOnNodes() {
	if (nodes.empty()){
		camera.target = (Vector2){ 0.0f, 0.0f };	// Position ciblée par la caméra
		camera.offset = (Vector2){ 100.0f, 150.0f };	// Offset de la caméra
		camera.rotation = 0.0f;
		camera.zoom = 1.0f;	// Niveau de zoom initial
		return;
	}

	float minX = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::lowest();
	float minY = std::numeric_limits<float>::max();
	float maxY = std::numeric_limits<float>::lowest();

	for (const auto &node : nodes) {
		if (node->x < minX) minX = node->x;
		if (node->x > maxX) maxX = node->x;
		if (node->y < minY) minY = node->y;
		if (node->y > maxY) maxY = node->y;
	}
	float width = maxX - minX;
	float height = maxY - minY;
	float ZoomW = GetScreenWidth() / (width + 100*2);
	float ZoomH = GetScreenHeight() / (height + 150*2);
	float cameraZoom = std::min(ZoomW, ZoomH);

	camera.target = (Vector2){minX, minY};	// Position ciblée par la caméra
	camera.offset = (Vector2){ 100.0f, 150.0f };	// Offset de la caméra
	camera.rotation = 0.0f;
	camera.zoom = cameraZoom;	// Niveau de zoom initial


}


// *****	METHODES: EVENTS / MOUSE / CURSOR / CAMERA

void	Game::updateMouseAndCursor() {
	mouse = { GetMouseX(), GetMouseY() };
	Vector2 mousePosition = GetMousePosition();
	Vector2 worldPosition = GetScreenToWorld2D(mousePosition, camera);
	cursor.x = roundToHundred(worldPosition.x);
	cursor.y = roundToHundred(worldPosition.y);

	if (mode == M_CREATE_ROAD) {
		cursor.x = roundToHundred(worldPosition.x);
		cursor.y = roundToHundred(worldPosition.y);
		if ((newNodeStart != nullptr)) {
			int disX = (newNodeStart->x - cursor.x);
			int disY = (newNodeStart->y - cursor.y);
			if (abs(disX) > abs(disY) ) {
				cursor.y = newNodeStart->y;
			}
			else{
				cursor.x = newNodeStart->x;
			}
		}
	}
}

void	Game::handleZoom(float wheel){


	// ** v1
	if (wheel != 0){
		//camera.zoom += wheel*0.05f;
		camera.zoom += wheel*0.15f;
		// Limites du zoom
		if (camera.zoom > 20.0f) camera.zoom = 20.0f;
		else if (camera.zoom < 0.1f) camera.zoom = 0.1f;
	}

	/******************

	// ** v2
	if (wheel != 0){

		Vector2 mousePosition = GetMousePosition();

		// Conversion de la position de la souris en coordonnées monde avant le zoom
		Vector2 worldPosBeforeZoom = GetScreenToWorld2D(mousePosition, camera);
		camera.zoom += wheel * 0.05f;

		// Limites du zoom
		if (camera.zoom > 3.0f) camera.zoom = 3.0f;
		else if (camera.zoom < 0.1f) camera.zoom = 0.1f;

		// Conversion de la position de la souris en coordonnées monde après le zoom
		Vector2 worldPosAfterZoom = GetScreenToWorld2D(mousePosition, camera);

		// Calcul du décalage causé par le zoom
		Vector2 offset = {
			worldPosBeforeZoom.x - worldPosAfterZoom.x,
			worldPosBeforeZoom.y - worldPosAfterZoom.y
		};

		// Ajustement de la cible de la caméra pour compenser le décalage
		camera.target.x -= offset.x * 2;
		camera.target.y -= offset.y * 2;
	}
	**************/
}

void	Game::handleEvents(UIPanelRoad &uiPanelRoad){

	if (IsKeyPressed(KEY_ESCAPE)) {
		if (mode == M_CREATE_ROAD) toggleAddRoadMode();
		else if (mode == M_MAP_OPEN) mode = M_NONE;
		if (selectedRoadId != -1){
			roads[selectedRoadId]->isSelected = false;
			selectedRoadId = -1;
		}
		if (selectedNodeId != -1){
			nodes[selectedNodeId]->isSelected = false;
			selectedNodeId = -1;
		}
	}

	if (mode == M_MAP_OPEN) return;



	if (hoveredRoadId != -1){
		roads[hoveredRoadId]->isHovered = false;
	}
	if (hoveredNodeId != -1){
		nodes[hoveredNodeId]->isHovered = false;
	}
	hoveredRoadId = -1;
	hoveredNodeId = -1;

	Vector2 worldPos = GetScreenToWorld2D(GetMousePosition(), camera);


	for (size_t i = 0; i < nodes.size(); i++) {
		Node &node = *nodes[i];
		//Road &road = roads[i];
		if (CheckCollisionPointRec(worldPos, nodes[i]->rec)){
		//if (CheckCollisionPointRec(worldPos, node->rec)){
			hoveredNodeId = i;
			nodes[i]->isHovered = true;
			break;
		}
	}

	if (hoveredNodeId == -1){
		for (size_t i = 0; i < roads.size(); i++) {
			Road &road = *roads[i];
			if (CheckCollisionPointRec(worldPos, road.rec)){
				hoveredRoadId = i;
				roads[i]->isHovered = true;
				break;
			}
		}
	}


	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		// * Le bouton gauche est pressé
		if (recIsHovered(ui.btnMapNew.rec)) {
			newMapClick();
		} else if (recIsHovered(ui.btnMapOpen.rec)) {
			openMapClick();
		} else if (recIsHovered(ui.btnMapSave.rec)) {
			saveMapClick();
		} else if (recIsHovered(ui.btnMapSaveAs.rec)) {
			saveMapAsClick();
		} else if (recIsHovered(ui.btnAddRoad.rec)) {
			toggleAddRoadMode();
			newNodeStart = nullptr;
		} else if (((mode == M_CREATE_ROAD) || (selectedRoadId != -1)) && CheckCollisionPointRec(GetMousePosition(), uiPanelRoad.panelRec)) {
			// geré ailleur
			std::cout << C_RED << "Click on uiPanelRoad" << C_NOR << std::endl;
		} else if (mode == M_CREATE_ROAD) {
			// * Mode ajout de route
			if (newNodeStart == nullptr) {
				newNodeStart = new Node(cursor.x, cursor.y);
			} else {
				createNodesAndRoad(*newNodeStart, Node(cursor.x, cursor.y), uiPanelRoad);
			}
		} else if ((hoveredRoadId != -1) || (hoveredNodeId != -1)) {
			if (selectedRoadId != -1){
				roads[selectedRoadId]->isSelected = false;
			}
			if (selectedNodeId != -1){
				nodes[selectedNodeId]->isSelected = false;
			}
				selectedNodeId = -1;
				selectedRoadId = -1;

			if (hoveredRoadId != -1){
				roads[hoveredRoadId]->isSelected = true;
				selectedRoadId = hoveredRoadId;
				uiPanelRoad.speedLimit = roads[hoveredRoadId]->speedLimit;
				uiPanelRoad.lanesCount = roads[hoveredRoadId]->lanesPtr.size();
			}
			else if(hoveredNodeId != -1){
				nodes[hoveredNodeId]->isSelected = true;
				selectedNodeId = hoveredNodeId;
			}
		}
		else{
			if (selectedRoadId != -1){
				roads[selectedRoadId]->isSelected = false;
			}
			if (selectedNodeId != -1){
				nodes[selectedNodeId]->isSelected = false;
			}
			selectedRoadId = -1;
			selectedNodeId = -1;
		}
	}
	else if (IsKeyDown(config.keyBindings["Right"]))		camera.target.x += 10 / camera.zoom;
	else if (IsKeyDown(config.keyBindings["Left"]))			camera.target.x -= 10 / camera.zoom;
	else if (IsKeyDown(config.keyBindings["Up"]))			camera.target.y -= 10 / camera.zoom;
	else if (IsKeyDown(config.keyBindings["Down"]))			camera.target.y += 10 / camera.zoom;
	else if (IsKeyDown(config.keyBindings["ZoomIn"]))		handleZoom(1);
	else if (IsKeyDown(config.keyBindings["ZoomOut"]))		handleZoom(-1);
	else if (IsKeyDown(config.keyBindings["ZoomReset"]))	centerCameraOnNodes();
	else if (IsKeyPressed(config.keyBindings["CreateRoad"]))toggleAddRoadMode();
	else if (IsKeyPressed(config.keyBindings["Help"]))		showHelp = !showHelp;
	else if (IsKeyPressed(config.keyBindings["Debug"]))		showDebug = !showDebug;
	else if (IsKeyPressed(config.keyBindings["GridCoords"]))showGridCoords = !showGridCoords;
	else if (IsKeyPressed(config.keyBindings["RoadsInfos"]))showRoadsInfos = !showRoadsInfos;


	// Gestion du zoom avec la molette de la souris
	float wheel = GetMouseWheelMove();
	handleZoom(wheel);

}

// *****	METHODES: CREATE : Nodes / Roads / Lanes / Cars

void	Game::addCarsOnLane(Lane &lane, int carsPerLane, bool atStart, Color color){
	for (int i = 0; i < carsPerLane; i++) {

		float speedLimitFactor = 1 + (getRandomNumberMinMax(-5, 5) / 10.0f);
		int direction = lane.road.direction;
		int x,y;
		if (lane.road.orientation == HORIZONTAL) {
			if (atStart)
				x = lane.road.start.x;
			else
				x = getRandomNumberMinMax(std::min(lane.road.start.x, lane.road.end.x), std::max(lane.road.start.x,lane.road.end.x));
			y = lane.rec.y + 1;
		}
		else if(lane.road.orientation == VERTICAL){	// * Si la route est verticale
			if (atStart)
				y = lane.road.start.y;
			else
				y = getRandomNumberMinMax(std::min(lane.road.start.y, lane.road.end.y), std::max(lane.road.start.y, lane.road.end.y));
			x = lane.rec.x + 1;
			//y = getRandomNumberMinMax(std::min(lane.road.start.y, lane.road.end.y), std::max(lane.road.start.y, lane.road.end.y));
		}
		else{
			std::cout << C_RED << "ERROR: Game::addCarsOnLane() : lane.road.orientation not valid" << C_NOR << std::endl;
			continue;
		}
		lane.cars.push_back(std::make_unique<Car>(x, y, speedLimitFactor, direction, color));
	}
	lane.sortCars();
}

/**
* @brief:
*		Créer une Road entre deux noeuds
*		et ajouter les Lanes correspondantes
*		et ajouter les Cars sur les Lanes
*/
void	Game::createRoad(Node &node1, Node &node2, int lanesCount, int speedLimit) {
	//Road *newRoad =new Road(&node1, &node2, lanesCount, speedLimit);
	auto newRoad = std::make_unique<Road>(&node1, &node2, lanesCount, speedLimit);


	// * ADD LANES to newRoad->lanesPtr
	for (int id = 0; id < lanesCount; id++) {
		newRoad->lanesPtr.push_back(std::make_unique<Lane>(*newRoad, id));	// roads[0]
	}

	newRoad->caclRec();

	// * ADD CARS to lanes
	for (auto &lanePtr : newRoad->lanesPtr) {
		Lane &lane = *lanePtr;
		addCarsOnLane(lane, 5, false, getRandomCarColor());
	}

	roads.push_back(std::move(newRoad));	// * A LA FIN !!!!
}


void	Game::createNodesAndRoad(Node nodeS, Node nodeE, UIPanelRoad &uiPanelRoad) {


	if (nodeS == nodeE) {
		std::cout << C_RED << "ERROR: Game::createNodesAndRoad() : nodeS == nodeE" << C_NOR << std::endl;
		newNodeStart = nullptr;
		return;
	}

	if (roadExists(nodeS, nodeE)) {
		std::cout << C_RED << "Road already exists between nodes (" << nodeS.x << ", " << nodeS.y << ") and (" << nodeE.x << ", " << nodeE.y << ")." << C_NOR << std::endl;
		newNodeStart = nullptr;
		return;
	}

	Node* node1 = nullptr;
	Node* node2 = nullptr;

	// Vérifier si nodeS existe déjà
	if (nodeExists(nodeS.x, nodeS.y)) {
		node1 = findNode(nodeS.x, nodeS.y);
	} else {
		nodes.push_back(std::make_unique<Node>(nodeS));
		node1 = nodes.back().get();	// Récupérer le pointeur vers le Node stocké
	}

	// Vérifier si nodeE existe déjà
	if (nodeExists(nodeE.x, nodeE.y)) {
		node2 = findNode(nodeE.x, nodeE.y);
	} else {
		nodes.push_back(std::make_unique<Node>(nodeE));
		node2 = nodes.back().get();
	}

	// Créer la route
	createRoad(*node1, *node2, uiPanelRoad.lanesCount, uiPanelRoad.speedLimit);
	asUnsavedChanges = true;
	newNodeStart = nullptr;

	// * create reverse direction road if needed
	if (!uiPanelRoad.reverseRoad)
		return;

	if (roadExists(*node2, *node1)) {
		std::cout << C_RED << "REVERSE Road already exists between nodes (" << node1->x << ", " << node1->y << ") and (" << node2->x << ", " << node2->y << ")." << C_NOR << std::endl;
		return;
	}
	createRoad(*node2, *node1, uiPanelRoad.lanesCount, uiPanelRoad.speedLimit);
}


// *****
