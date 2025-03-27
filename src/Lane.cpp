#include "../include/traffic-orchestrator.hpp"

namespace{
}


int Lane::LANE_WIDTH = 4;

// *****	CONTRUCTOR:


Lane::Lane(Road &roaddd, int id) : road(roaddd), id(id) {
	rec= {0, 0, 0, 0};
	color = GRAY;



	if (road.orientation == HORIZONTAL) {
		rec.width = std::abs(road.end.x - road.start.x);
		rec.height = Lane::LANE_WIDTH;
		if (road.direction == D_RIGHT) {
			rec.x = road.start.x;
			rec.y = road.start.y + rec.height * (id) + 1;
			//color = GRAY;
		}
		else{	// * vers la gauche
			rec.x = road.end.x;
			rec.y = road.end.y - rec.height * (id+1) - 1;
			//color = GRAY;
		}
	}
	else {
		rec.height = std::abs(road.end.y - road.start.y);
		rec.width = Lane::LANE_WIDTH;
		if (road.direction == D_DOWN) {	// * vers le bas
			rec.x = road.start.x - rec.width * (id+1) -1;
			rec.y = road.start.y;
			//color = GRAY;
		}
		else{	// * vers la haut
			rec.x = road.end.x + rec.width * (id) +1;
			rec.y = road.end.y;
			//color = GRAY;
		}
	}
	///std::cout << C_GRE << "LANE created" << C_NOR << std::endl;
	///std::cout << "   LANE.rec.x=" << rec.x << std::endl;
	///std::cout << "   LANE.rec.y=" << rec.y << std::endl;
	///std::cout << "   LANE.rec.width =" << rec.width << std::endl;
	///std::cout << "   LANE.rec.height=" << rec.height << std::endl;
}


// *****	METHODES:

void	Lane::update(float deltaTime, float laneEnd) {
	// TODO
}

void	Lane::addCar(float position, float speed) {
	// TODO
}

void	Lane::sortCars() {
	if (road.direction == D_RIGHT) {
		std::sort(cars.begin(), cars.end(), [](const std::unique_ptr<Car>&  a, const std::unique_ptr<Car>&  b) {
			return a->x < b->x ;
		});
	}
	else if (road.direction == D_LEFT) {
		std::sort(cars.begin(), cars.end(), [](const std::unique_ptr<Car>&  a, const std::unique_ptr<Car>&  b) {
			return a->x > b->x ;
		});
	}
	else if (road.direction == D_DOWN) {
		std::sort(cars.begin(), cars.end(), [](const std::unique_ptr<Car>&  a, const std::unique_ptr<Car>&  b) {
			return a->y < b->y ;
		});
	}
	else if (road.direction == D_UP) {
		std::sort(cars.begin(), cars.end(), [](const std::unique_ptr<Car>&  a, const std::unique_ptr<Car>&  b) {
			return a->y > b->y ;
		});
	}
}

bool Lane::canEnterLaneLane(){
	if (cars.size() == 0) return true;int marge = 30;	// ! la meme que dans draw()

	if (road.direction == D_DOWN){
		for (size_t i = 0; i < cars.size(); i++) {
			if (cars[i]->y <  road.start.y + marge)
				return false;
		}
	}
	else if (road.direction == D_UP){
		for (size_t i = 0; i < cars.size(); i++) {
			if (cars[i]->y >  road.start.y - marge)
				return false;
		}
	}
	else if (road.direction == D_RIGHT){
		for (size_t i = 0; i < cars.size(); i++) {
			if (cars[i]->x <  road.start.x + marge)
				return false;
		}
	}
	else if (road.direction == D_LEFT){
		for (size_t i = 0; i < cars.size(); i++) {
			if (cars[i]->x >  road.start.x - marge)
				return false;
		}
	}

	return true;
}


// *****	METHODES: EVENTS / MOUSE / CURSOR / CAMERA


// *****	METHODES: DRAW
void	Lane::draw() {

	//bool can = canEnterLaneLane();

	Color interLaneColor = LIGHTGRAY;
	//Color laneColor = can ? color : BLUE;

	int marge = 30;	// ! la meme que dans canEnterLaneLane()

	DrawRectangle(rec.x, rec.y, rec.width, rec.height, color);

	if (road.orientation == VERTICAL) {
		//if (!can)
		//	DrawRectangle(rec.x, rec.y + (road.direction==D_UP ? rec.height - marge : 0), rec.width, marge, RED);
		DrawLine(rec.x, rec.y, rec.x , rec.y+ rec.height, interLaneColor);
		DrawLine(rec.x +rec.width, rec.y, rec.x+rec.width, rec.y+ rec.height, interLaneColor);
	}	else {
		//if (!can)
		//	DrawRectangle(rec.x + (road.direction==D_LEFT ? rec.width - marge : 0), rec.y, marge, rec.height, RED);
		DrawLine(rec.x, rec.y,				rec.x + rec.width, rec.y,				interLaneColor);
		DrawLine(rec.x, rec.y+rec.height,	rec.x + rec.width, rec.y+rec.height,	interLaneColor);
	}



}



// *****


 Road& Lane::getRoad() { return road; }