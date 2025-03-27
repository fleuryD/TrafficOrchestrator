#include "../include/traffic-orchestrator.hpp"




// *****	CONTRUCTOR:

Node::Node(int x, int y) : x(x), y(y) {
	isHovered = false;
	isSelected = false;
	size = 20;	// * 20px	// radius

	float dim = 20;
	rec = {x-dim, y-dim, dim*2, dim*2};
	//rec = {100, 100,200,200};
}


// *****	METHODES:




// *****	METHODES: DRAW

void	Node::draw(Color col) {
	int margin = 1;
	if (isHovered) {
		DrawCircle(x, y, 24, WHITE);
	}
	if (isSelected) {
		DrawCircle(x, y, 22, GREEN);
	}


	DrawCircle(x, y, 20, col);
}



// *****