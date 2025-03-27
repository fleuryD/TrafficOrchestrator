#ifndef NODE_HPP
#define NODE_HPP

#include "../include/traffic-orchestrator.hpp"

class Node {

	public:
		int		x, y;
		bool	isHovered;
		bool	isSelected;
		Rectangle	rec;	// ou circle ?????
		int		size;	// calculated from maxLaneCount

		// TODO:
		// int		genre;	// N_ROUND_ABOUT, N_CROSSING, N_TURN, N_STRAIGHT, N_ENTRY, N_EXIT
		// bool		isExit;
		// bool		isEntry;
		// int		sppedLimit;
		// int		maxLaneCount;
		// cars;
		// roadsFrom;
		// roadsTo;
		// if (isExit) cars can't turn around to their initial road

		// * CONSTRUCTORS:
		Node(int x = 0, int y = 0);


		// * METHODS:
		void draw(Color col=ORANGE);

		// * Surcharge de l'opérateur ==
		bool operator==(const Node& other) const {
			return (x == other.x && y == other.y);
		}



};

#endif	// NODE_HPP
