#include "../include/traffic-orchestrator.hpp"

namespace{

}

// *****	CONTRUCTORS:

Road::Road(Node *start, Node *end, int lanesCount, int speedLimit)
	: start(*start), end(*end), lanesCount(lanesCount), speedLimit(speedLimit){

	setOrientationAndDirectionFromNodes();
	caclRec();	// after addLanes

	isHovered = false;
	isSelected = false;


}

// *****	METHODES:

void	Road::caclRec(){
	float x = std::min(start.x, end.x);
	float y = std::min(start.y, end.y);
	//float w = std::max(20, abs(start.x - end.x));
	float size = lanesPtr.size() * (Lane::LANE_WIDTH) + 3 ;
	float w = (orientation == HORIZONTAL) ? abs(start.x - end.x) : size;
	float h = (orientation == VERTICAL)   ? abs(start.y - end.y) : size;
	//float h = std::max(20, abs(start.y - end.y));
	if (direction == D_LEFT) {
		y -= size;
	}
	if ( direction == D_DOWN) {
		x -= size;
	}
	rec = {x, y, w, h};
}

//std::vector<Road*>	Road::getNextPossibleRoads(const std::vector<Road>& allRoads) {
std::vector<Road*>	Road::getNextPossibleRoads(const std::vector<std::unique_ptr<Road>>& allRoads) {
	std::vector<Road*> nextRoads;

	for (auto& roadt : allRoads) {

		 Road* roadPtr = roadt.get();  // Récupère le pointeur brut

		if (roadPtr == this) continue;   // Compare directement les pointeurs
		//if (&roadt == this) continue;	// Ignore la route actuelle

		// Vérifie si la route commence là où la route actuelle se termine
		//if (roadt.start == this->end && roadt.end == this->end){
		if (roadt->start == this->end){
		//if ((roadt.start.x == this->end.x) && (roadt.start.y == this->end.y)) {	// && || roadt.end == this->end
			//nextRoads.push_back(const_cast<Road*>(&roadt));
			nextRoads.push_back(roadPtr);    // Ajoute le pointeur sans conversion illégale
		}
	}
	return nextRoads;
}


void	Road::setOrientationAndDirectionFromNodes(){
	// * set direction and orientation
	if (start.y == end.y) {
		orientation = HORIZONTAL;
		if (start.x < end.x)
			direction = D_RIGHT;
		else if (start.x > end.x)
			direction = D_LEFT;
		else
			std::cerr << C_RED << "Error: Road direction is not valid" << std::endl;
	}
	else if (start.x == end.x) {
		orientation = VERTICAL;
		if (start.y < end.y)
			direction = D_DOWN;
		else if (start.y > end.y)
			direction = D_UP;
		else
			std::cerr << C_RED << "Error: Road direction is not valid" << std::endl;
	}
	else {
		orientation = -1;
		std::cerr << C_RED << "Error: Road orientation is not valid" << std::endl;
	}

}

// *****	METHODES: EVENTS / MOUSE / CURSOR / CAMERA


// *****	METHODES: DRAW

void	drawRoadSpeedLimit(int x, int y, int speedLimit) {
	int radiusRed = 10;	// Rayon Total
	int radiusWhite = 8;	// Rayon du cercle blanc
	int fontSize = 9;

	Color circleColor = YELLOW;
	if (speedLimit >= 120) circleColor = RED;
	else if (speedLimit >= 80) circleColor = ORANGE;
	else if (speedLimit >= 50) circleColor = GREEN;
	else circleColor = BLUE;

	DrawCircle(x, y, radiusRed, circleColor);
	DrawCircle(x, y, radiusWhite, WHITE);

	std::string speedText = std::to_string(speedLimit);
	Vector2 textSize = MeasureTextEx(GetFontDefault(), speedText.c_str(), fontSize, 1);

	DrawText(speedText.c_str(), x - textSize.x / 2, y - textSize.y / 2, fontSize, BLACK);
}


void	Road::drawInfos() {
	// * set coordonnées pour afficher le texte
	int textX, textY;
	if (orientation == HORIZONTAL) {
		textX = std::min(start.x, end.x) + abs(start.x - end.x) / 2;
		textY = start.y + (direction == D_LEFT ? -20 : 20);
	}
	else {
		textX = start.x + (direction == D_UP ? 20 : -20);
		textY = std::min(start.y, end.y) + abs(start.y - end.y) / 2;
	}

	// * vitesse
	//DrawText(TextFormat("%d km/h", speedLimit), textX, textY + 10, 8, GRAY);
	drawRoadSpeedLimit(textX, textY, speedLimit);
	/*
	int carsCount = 0;
	for (auto &lanePtr : lanesPtr) {
		Lane &lane = *lanePtr;
		carsCount += lane.cars.size();
	}
	if (orientation == HORIZONTAL) {
		textX += 15;
		textY -= 5;
	}
	else {
		textX -= 10;
		textY += 15;
	}
	DrawText(TextFormat("%d cars", carsCount), textX, textY, 8, GRAY);
	*/
}


void	Road::draw() {

	// * Afficher ligne noir pour masquer la grid
	//DrawLine(start.x, start.y, end.x, end.y, BLACK);

	// * par dessus les lanes !!!
	int margin = 1;
	if (isHovered) {
		DrawRectangle(rec.x-margin, rec.y-margin, rec.width+margin*2, rec.height+margin*2,  Fade(WHITE, 0.8f));
	}
	if (isSelected) {
		DrawRectangle(rec.x, rec.y, rec.width, rec.height, Fade(GREEN, 0.7f));
	}




	//DrawText(TextFormat("rec.x= %f, .y= %f, .w= %f, .h= %f", this->rec.x, rec.y, rec.width, rec.height), textX, textY+20, 8, GRAY);


	/*
	if (CheckCollisionPointRec(GetMousePosition(), rec)){
		DrawText("Mouse is on Road", textX, textY+30, 8, RED);
	}
	else{
		DrawText("Mouse is NOT on Road", textX, textY+30, 8, GREEN);
	}
	*/


	// * Lanes
	/*
	if (orientation == HORIZONTAL) {
		textX += 20;
		textY += direction == D_LEFT ? -30 : 0;
	}
	else {
		textY += 20;
		textX += direction == D_UP ? -5 : -30;
	}
	for (auto &lanePtr : lanesPtr) {
		Lane &lane = *lanePtr;

		DrawText(TextFormat("L%d: %d cars", lane.id, lane.cars.size()), textX, textY, 4, GRAY);
		textY += 8;
	}
	*/
}

// *****