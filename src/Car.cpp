#include "../include/traffic-orchestrator.hpp"

std::vector<Color>	CarColors = {
	RED,
	BLUE,
	GREEN,
	BLACK,
	DARKGRAY,
	DARKBLUE,
	DARKGREEN,
	DARKPURPLE,
	DARKBROWN,
	YELLOW,
	GOLD,
	ORANGE,
	PINK,
	MAROON,
	LIME,
	DARKBLUE,
	PURPLE,
	BEIGE,
	BROWN,
	DARKBROWN,
	MAGENTA,
};

Color	getRandomCarColor() {

	if (CarColors.empty()) {
		throw std::invalid_argument("La liste des couleurs est vide.");
	}

	// Initialisation du générateur de nombres aléatoires
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(0, CarColors.size() - 1);

	// Sélection aléatoire d'un indice
	int indiceAleatoire = distrib(gen);

	// Retourne la couleur correspondante
	return CarColors[indiceAleatoire];
}

// *****	CONTRUCTOR:


Car::Car(float x, float y, float speedLimitFactor, int direction, Color color)
	: x(x), y(y), speedLimitFactor(speedLimitFactor), direction(direction), color(color), isDeleted(false) {
}


// *****	METHODES:


void	Car::move(float deltaTime, float cameraZoomFactor, Lane &lane, Car *nextCar) {
	// TODO
}





// *****	METHODES: DRAW


void	Car::draw() {
	if (direction == 0) {
		return;
	}
	if (direction == D_UP || direction == D_DOWN) {
		DrawRectangle(x, y, 2, 5, color);
	}
	else {
		DrawRectangle(x, y, 5, 2, color);
	}
}



// *****