#include "../include/traffic-orchestrator.hpp"


void	Game::drawGrid() {
	Color gridColor = { 20, 20, 20, 255 };
	Color grid0Color = { 100, 100, 20, 255 };
	Color coorsColor = { 200, 200, 100, 255 };
	Color coorsNegativeColor = { 255, 100, 50, 255 };

	// Définir l'espacement de la grille en unités du monde
	const float gridSpacing = 100.0f;

	// Calculer les limites de la vue actuelle en coordonnées du monde
	Vector2 topLeft = GetScreenToWorld2D({0, 0}, camera);
	Vector2 bottomRight = GetScreenToWorld2D({(float)screenWidth, (float)screenHeight}, camera);

	// Ajuster les limites pour couvrir toute la zone visible
	int startX = (int)(floor(topLeft.x / gridSpacing) * gridSpacing);
	int endX = (int)(ceil(bottomRight.x / gridSpacing) * gridSpacing);
	int startY = (int)(floor(topLeft.y / gridSpacing) * gridSpacing);
	int endY = (int)(ceil(bottomRight.y / gridSpacing) * gridSpacing);

	// Dessiner les lignes verticales de la grille
	for (int x = startX; x <= endX; x += gridSpacing) {
		Vector2 start = GetWorldToScreen2D({(float)x, topLeft.y}, camera);
		Vector2 end = GetWorldToScreen2D({(float)x, bottomRight.y}, camera);
		DrawLineV(start, end, showGridCoords && x==0 ? grid0Color : gridColor);

		// *Afficher les coordonnées sur l'axe X
		if (showGridCoords && (camera.zoom >= 0.5f || (x % 1000 == 0)))
			DrawText(TextFormat("%d m", x), start.x + 5, ui.navHeaderHeight + 10, 10, x>=0?coorsColor:coorsNegativeColor);	// !!! y doit etre > hauteur de l'UI
	}

	// Dessiner les lignes horizontales de la grille
	for (int y = startY; y <= endY; y += gridSpacing) {
		Vector2 start = GetWorldToScreen2D({topLeft.x, (float)y}, camera);
		Vector2 end = GetWorldToScreen2D({bottomRight.x, (float)y}, camera);
		DrawLineV(start, end, showGridCoords && y==0 ? grid0Color : gridColor);

		// *Afficher les coordonnées sur l'axe Y
		if (showGridCoords && (camera.zoom >= 0.5f || (y % 1000 == 0)))
			DrawText(TextFormat("%d m", y), start.x + 5, start.y + 5, 10, y>=0?coorsColor:coorsNegativeColor);
	}



}



void	Game::drawMap() {

	if (showGridCoords)
		drawCross(0, 0, 40, YELLOW);

	// * DRAW ROADS & LANES
	for (auto &roadPrt : roads) {
		Road &road = *roadPrt;
		for (auto &lanePtr : road.lanesPtr) {
			Lane &lane = *lanePtr;
			lane.draw();
		}
		if (showRoadsInfos || road.isSelected || road.isHovered) {
			road.drawInfos();
		}
		road.draw();
	}

	// * DRAW NODES
	for (auto &nodePtr : nodes) {
		Node &node = *nodePtr;
		node.draw(GRAY);
	}

	// * DRAW CARS
	for (auto &roadPrt : roads) {
		Road &road = *roadPrt;
		for (auto &lanePtr : road.lanesPtr) {
			Lane &lane = *lanePtr;
			for (auto &carPtr : lane.cars) {
				Car &car = *carPtr;
				car.draw();
			}

		}
	}

	// * DRAW CURSOR & ADD ROAD
	if (mode == M_CREATE_ROAD) {
		drawCross(cursor.x, cursor.y, 20, GREEN);
		if (newNodeStart != nullptr) {
			newNodeStart->draw(GREEN);

			DrawCircle(cursor.x, cursor.y, 10, GREEN);
			DrawLine(newNodeStart->x, newNodeStart->y, cursor.x, cursor.y, GREEN);
		}
	}

}



void	Game::drawHelpPanel() {

	if (!showHelp) return;

	const int panelWidth = 200;
	const int panelHeight = 440;
	int panelX = screenWidth - panelWidth - 10;
	const int panelY = 100;
	const int fontSize = 16;

	// Fond du panneau
	DrawRectangle(panelX, panelY, panelWidth, panelHeight, Fade(BLACK, 0.5f));
	DrawRectangleLines(panelX, panelY, panelWidth, panelHeight, DARKGRAY);


	int x = panelX + 20;
	int y = panelY + 10;

	// Titre
	DrawText(TextFormat("Help  (%s)", getKeyNameFromKey(config.keyBindings["Help"]).c_str()),	x-10, y, fontSize+4, RAYWHITE);	y += 40;

	DrawText(TextFormat("Show this Help    : %s", getKeyNameFromKey(config.keyBindings["Help"]).c_str()),	x, y, fontSize, RAYWHITE);	y += 20;
	DrawText(TextFormat("Show GridCoords : %s", getKeyNameFromKey(config.keyBindings["GridCoords"]).c_str()),	x, y, fontSize, RAYWHITE);	y += 20;
	DrawText(TextFormat("Show RoadsInfos : %s", getKeyNameFromKey(config.keyBindings["RoadsInfos"]).c_str()),	x, y, fontSize, RAYWHITE);	y += 20;
	DrawText(TextFormat("Show Debug        : %s", getKeyNameFromKey(config.keyBindings["Debug"]).c_str()),	x, y, fontSize, RAYWHITE);	y += 40;

	DrawText(TextFormat("Up        : %s", getKeyNameFromKey(config.keyBindings["Up"]).c_str()),		x, y, fontSize, RAYWHITE);	y += 20;
	DrawText(TextFormat("Down     : %s",  getKeyNameFromKey(config.keyBindings["Down"]).c_str()),	x, y, fontSize, RAYWHITE);	y += 20;
	DrawText(TextFormat("Left      : %s", getKeyNameFromKey(config.keyBindings["Left"]).c_str()),	x, y, fontSize, RAYWHITE);	y += 20;
	DrawText(TextFormat("Right     : %s", getKeyNameFromKey(config.keyBindings["Right"]).c_str()),	x, y, fontSize, RAYWHITE);	y += 40;

	DrawText(TextFormat("ZoomIn      : %s", getKeyNameFromKey(config.keyBindings["ZoomIn"]).c_str()),		x, y, fontSize, RAYWHITE);	y += 20;
	DrawText(TextFormat("ZoomOut    : %s",  getKeyNameFromKey(config.keyBindings["ZoomOut"]).c_str()),		x, y, fontSize, RAYWHITE);	y += 20;
	DrawText(TextFormat("ZoomReset : %s",   getKeyNameFromKey(config.keyBindings["ZoomReset"]).c_str()),	x, y, fontSize, RAYWHITE);	y += 30;

	DrawText(TextFormat("Add Road : %s",   getKeyNameFromKey(config.keyBindings["CreateRoad"]).c_str()),	x, y, fontSize, RAYWHITE);	y += 40;
	DrawText(TextFormat("Quit Game : %s",   getKeyNameFromKey(config.keyBindings["Quit"]).c_str()),	x, y, fontSize, RAYWHITE);	y += 40;

	DrawText("See config.txt",	x-10, y, fontSize+2, RAYWHITE);	y += 40;



}


void	Game::drawDebug() {
	if (!showDebug) return;

	const int panelWidth = 200;
	const int panelHeight = 200;
	const int panelX = screenWidth - panelWidth - 10;
	const int panelY = 600;
	const int fontSize = 14;
	const Color color = RAYWHITE;

	int x = panelX + 20;
	int y = panelY + 10;

	// Fond du panneau
	DrawRectangle(panelX, panelY, panelWidth, panelHeight, Fade(BLACK, 0.5f));
	DrawRectangleLines(panelX, panelY, panelWidth, panelHeight, Fade(RED, 0.5f));

	DrawText(TextFormat("Debug  (%s)", getKeyNameFromKey(config.keyBindings["Debug"]).c_str()),	x-10, y, fontSize+4, RAYWHITE);	y += 40;

	DrawText(TextFormat("Mouse:  x=%i,  y=%i", mouse.x, mouse.y),	x, y, fontSize, color); y += 20;
	DrawText(TextFormat("Cursor: x=%i,  y=%i", cursor.x, cursor.y), x, y, fontSize, color); y += 20;
	DrawText(TextFormat("camera.zoom: %f", camera.zoom),			x, y, fontSize, color); y += 20;
	DrawText(TextFormat("FPS: %i", GetFPS()),						x, y, fontSize, color); y += 20;
	DrawText(TextFormat("nodes.size: %i", nodes.size()),			x, y, fontSize, color); y += 20;

}

