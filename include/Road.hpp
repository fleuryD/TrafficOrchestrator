#ifndef ROAD_HPP
#define ROAD_HPP



class Lane;

class Road {

	public:
		Node		&start;
		Node		&end;
		int			lanesCount;	// DEPRECATED : use lanes.size()
		int			speedLimit;		// * (km/h)
		int			orientation;	// * HORIZONTAL | VERTICAL
		int			direction;		// * D_LEFT | D_RIGHT | D_UP | D_DOWN
		Rectangle	rec;
		bool		isHovered;
		bool		isSelected;
		std::vector<std::unique_ptr<Lane>>	lanesPtr;

		// * CONSTRUCTORS:
		Road(Node *start, Node *end, int Lines, int speed);

		// * METHODS:
		void draw();
		void drawInfos();
		void caclRec();
		//std::vector<Road*> getNextPossibleRoads(const std::vector<Road>& allRoads);
		std::vector<Road*> getNextPossibleRoads(const std::vector<std::unique_ptr<Road>>& allRoads);
		int getOrientation() { return orientation; }

		Road& operator=(Road&&) = delete;
	private:
		void setOrientationAndDirectionFromNodes();
};

#endif	// ROAD_HPP
