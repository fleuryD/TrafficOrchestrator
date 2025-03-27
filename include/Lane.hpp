#ifndef LANE_HPP
#define LANE_HPP


class Car;
class Road;
class DrawRectangleRec;


class Lane {

	public:
		static int	LANE_WIDTH;

		Road		&road;
		int			id;
		Rectangle	rec;
		Color		color;
		std::vector<std::unique_ptr<Car>>	cars;

		// * CONSTRUCTORS:
		Lane(Road &road, int id);

		// * METHODS:
		void	update(float deltaTime, float laneEnd);
		void	addCar(float position, float speed);
		void	sortCars();
		void	draw();
		bool	canEnterLaneLane();
		Road&	getRoad();	// ! unused
};

#endif	// LANE_HPP