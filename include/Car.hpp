#ifndef CAR_HPP
#define CAR_HPP

class Road;
class Lane;

class Car {

	public:
		float	x, y;
		float	speedLimitFactor;	// * 0.5f to 1.5f * roadSpeedLimit
		int		direction;
		Color	color;
		bool	isDeleted;
		int		nextRoadId;	// none = -1
		int		nextLaneId;	// none = -1

		// * CONSTRUCTORS:
		Car(float x, float y, float speedLimitFactor, int direction, Color color);

		// * METHODS:
		void move(float deltaTime, float cameraZoomFactor, Lane &lane, Car *nextCar);
		void draw();

};

#endif	// CAR_HPP
