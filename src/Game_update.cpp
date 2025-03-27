#include "../include/traffic-orchestrator.hpp"


// *****	METHODES: UPDATE

//
void	Game::updateCarAtEndOfRoad(std::vector<Road*> &nextPossibleRoads, Lane &lane, int carId) {
	if (nextPossibleRoads.size() > 0) {
		int nextRoadId = getRandomNumberMinMax(0, nextPossibleRoads.size() - 1);
		Road *nextRoad = nextPossibleRoads[nextRoadId];
		int nextLaneId = getRandomNumberMinMax(0, nextRoad->lanesPtr.size() - 1);
		if (nextRoad->lanesPtr[nextLaneId]->canEnterLaneLane()){
			addCarsOnLane(*nextRoad->lanesPtr[nextLaneId], 1, true, lane.cars[carId]->color);
			lane.cars[carId]->isDeleted = true;
		}
	}
	else{
		lane.cars[carId]->isDeleted = true;
	}

}

/**
* @brief:	Mettre à jour la logique du jeu
*
*/
void	Game::update() {

	float deltaTime = GetFrameTime();
	float cameraZoomFactor = camera.zoom;
	float moveDistFactor = 1000.0f / 3600.0f * deltaTime * cameraZoomFactor;	// a multiplier par road.speed et par car.speedLimitFactor
	//moveDistFactor /=10;// !! Pour tester



	// * TEST ; ajouter de nouvelles voitures
	/*
	if(frameCounter % 50 == 0){
		std::cout << C_GRE << "   %500==0  " << C_NOR << std::endl;
		// * Supprimer les voitures qui ont atteint la fin de la route
	//for (Road &road : roads) {
		for (auto &roadPrt : roads) {
			Road &road = *roadPrt;
			for (auto &lanePtr : road.lanesPtr) {
				Lane &lane = *lanePtr;
				addCarsOnLane(lane, 1, true, YELLOW);

			}
		}
	}
	*/

	// * Trier les cars sur les lanes
	//for (Road &road : roads) {
	for (auto &roadPrt : roads) {
		Road &road = *roadPrt;
		for (auto &lanePtr : road.lanesPtr) {
			Lane &lane = *lanePtr;
			lane.sortCars();
		}
	}

	int nodeMarge = 20;

	// * Deplacer les voitures
	//for (Road &road : roads) {
	for (auto &roadPrt : roads) {
		Road &road = *roadPrt;
		float roadMoveDistFactor = moveDistFactor * road.speedLimit;
		for (auto &lanePtr : road.lanesPtr) {
			Lane &lane = *lanePtr;

			lane.sortCars();

			std::vector<Road*> nextPossibleRoads = lane.road.getNextPossibleRoads(roads);

			for (size_t i = lane.cars.size(); i-- > 0;) {	// Parcours inversé
				float moveDist = lane.cars[i]->speedLimitFactor * roadMoveDistFactor;

				if (lane.road.direction == D_RIGHT) {
					lane.cars[i]->x += moveDist;
					// * si il y a une voiture devant à une disatance < minDistBetweenCarsInLane
					if ( (i < lane.cars.size() - 1)
						&& (lane.cars[i+1]->isDeleted != true)
						&& (lane.cars[i+1]->x - lane.cars[i]->x < minDistBetweenCarsInLane)
						){
							lane.cars[i]->x = lane.cars[i+1]->x - minDistBetweenCarsInLane;
					}
					else if ( lane.cars[i]->x >= lane.road.end.x - nodeMarge) {
						updateCarAtEndOfRoad(nextPossibleRoads, lane, i);
						if (!lane.cars[i]->isDeleted)
							lane.cars[i]->x = lane.road.end.x - nodeMarge;

					}
				}
				else if (lane.road.direction == D_LEFT){
					lane.cars[i]->x -= moveDist;
					if ( (i < lane.cars.size() - 1)
						&& (lane.cars[i+1]->isDeleted != true)
						&& (lane.cars[i]->x - lane.cars[i+1]->x  < minDistBetweenCarsInLane)) {
							lane.cars[i]->x = lane.cars[i+1]->x + minDistBetweenCarsInLane;
					}
					else if (lane.cars[i]->x < lane.road.end.x  + nodeMarge) {
						updateCarAtEndOfRoad(nextPossibleRoads, lane, i);
						if (!lane.cars[i]->isDeleted)
							lane.cars[i]->x = lane.road.end.x + nodeMarge;
					}
				}
				else if (lane.road.direction == D_DOWN) {
					lane.cars[i]->y += moveDist;
					if ( (i < lane.cars.size() - 1)
						&& (lane.cars[i+1]->isDeleted != true)
						&& (lane.cars[i+1]->y - lane.cars[i]->y < minDistBetweenCarsInLane)) {
							lane.cars[i]->y = lane.cars[i+1]->y - minDistBetweenCarsInLane;
					}
					else if (lane.cars[i]->y > lane.road.end.y - nodeMarge) {
						updateCarAtEndOfRoad(nextPossibleRoads, lane, i);
						if (!lane.cars[i]->isDeleted)
							lane.cars[i]->y = lane.road.end.y - nodeMarge;
					}
				}
				else if (lane.road.direction == D_UP){
					lane.cars[i]->y -= moveDist;
					if ( (i < lane.cars.size() - 1)
						&& (lane.cars[i+1]->isDeleted != true)
						&& (lane.cars[i]->y - lane.cars[i+1]->y < minDistBetweenCarsInLane)) {
							lane.cars[i]->y = lane.cars[i+1]->y + minDistBetweenCarsInLane;
					}
					else if (lane.cars[i]->y < lane.road.end.y + nodeMarge) {
						updateCarAtEndOfRoad(nextPossibleRoads, lane, i);
						if (!lane.cars[i]->isDeleted)
							lane.cars[i]->y = lane.road.end.y + nodeMarge;
					}
				}
				else{
					std::cout << C_RED << "ERROR: Game::update() : lane.road.direction not valid" << C_NOR << std::endl;
				}


			}
		}
	}

	// * si  (car.isDeleted) deplacer car sur Une autre lane

	/*
	//for (Road &road : roads) {
	for (auto &roadPrt : roads) {
		Road &road = *roadPrt;
		for (auto &lanePtr : road.lanesPtr) {
			Lane &lane = *lanePtr;

			if (lane.road.direction == D_RIGHT || lane.road.direction == D_LEFT) continue;	// !! géré ci dessus

			std::vector<Road*> nextPossibleRoads = lane.road.getNextPossibleRoads(roads);
			if (nextPossibleRoads.size() > 0) {
				for (auto &carPtr : lane.cars) {
					Car &car = *carPtr;
					if (!car.isDeleted) continue;

					int randomRoadId = getRandomNumberMinMax(0, nextPossibleRoads.size() - 1);
					Road *nextRoad = nextPossibleRoads[randomRoadId];
					int laneId = getRandomNumberMinMax(0, nextRoad->lanesPtr.size() - 1);
					if (canEnterLaneLane(*nextRoad->lanesPtr[laneId]))
						addCarsOnLane(*nextRoad->lanesPtr[laneId], 1, true, car.color);


				}
			}
		}
	}
	*/


	// * Supprimer les voitures qui ont atteint la fin de la route (car.isDeleted)
	//for (Road &road : roads) {
	for (auto &roadPrt : roads) {
		Road &road = *roadPrt;
		for (auto &lanePtr : road.lanesPtr) {
			Lane &lane = *lanePtr;
			lane.cars.erase(
				std::remove_if(
					lane.cars.begin(),
					lane.cars.end(),
					[](const std::unique_ptr<Car>& car) { return car->isDeleted; }	// On accède au Car via le unique_ptr
				),
				lane.cars.end()
			);
		}
	}




}